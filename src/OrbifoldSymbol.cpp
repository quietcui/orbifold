#include "orbifold/OrbifoldSymbol.h"

#include <cctype>
#include <stdexcept>

namespace orbifold {

namespace {
Component makeCone(int order) {
    return Component{ComponentType::ConePoint, order};
}

Component makeCorner(int order) {
    return Component{ComponentType::CornerReflector, order};
}

} // namespace

OrbifoldSymbol::OrbifoldSymbol(std::string symbol) : symbol_(std::move(symbol)) {
    parse();
}

void OrbifoldSymbol::parse() {
    components_.clear();
    hasMirrorBoundary_ = false;
    crosscaps_ = 0;
    handles_ = 0;

    bool parsingMirrorCorners = false;

    for (std::size_t i = 0; i < symbol_.size(); ++i) {
        const char ch = symbol_[i];
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            auto value = parseNumber(symbol_, i);
            if (!value.has_value()) {
                throw std::runtime_error("Failed to parse orbifold symbol: unexpected digit state");
            }
            if (parsingMirrorCorners) {
                components_.push_back(makeCorner(*value));
            } else {
                components_.push_back(makeCone(*value));
            }
            continue;
        }

        switch (ch) {
            case '*':
                hasMirrorBoundary_ = true;
                parsingMirrorCorners = true;
                components_.push_back(Component{ComponentType::MirrorBoundary, 0});
                break;
            case 'x':
                crosscaps_ += 1;
                parsingMirrorCorners = false;
                components_.push_back(Component{ComponentType::Crosscap, 0});
                break;
            case 'o':
                handles_ += 1;
                parsingMirrorCorners = false;
                components_.push_back(Component{ComponentType::Handle, 0});
                break;
            case ' ': // allow optional spaces
            case '\t':
                break;
            default:
                throw std::runtime_error("Unsupported character in orbifold symbol: " + std::string(1, ch));
        }
    }
}

std::optional<int> OrbifoldSymbol::parseNumber(const std::string &text, std::size_t &index) {
    int value = 0;
    std::size_t count = 0;
    while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index]))) {
        value = value * 10 + (text[index] - '0');
        ++index;
        ++count;
    }
    if (count == 0) {
        return std::nullopt;
    }
    // the for-loop will increment index again, so roll back one step
    if (index != 0) {
        --index;
    }
    return value;
}

double OrbifoldSymbol::eulerCharacteristic() const {
    const auto &table = wallpaperLookup();
    if (table.find(symbol_) != table.end()) {
        // Wallpaper orbifolds always have Euler characteristic zero.
        return 0.0;
    }

    double chi = 2.0;

    // each handle lowers the characteristic by 2
    chi -= static_cast<double>(2 * handles_);

    // each crosscap lowers it by 1
    chi -= static_cast<double>(crosscaps_);

    // mirrored boundary component lowers by 1
    if (hasMirrorBoundary_) {
        chi -= 1.0;
    }

    bool seenMirrorBoundary = false;
    for (const auto &component : components_) {
        switch (component.type) {
            case ComponentType::ConePoint:
                chi -= (1.0 - 1.0 / static_cast<double>(component.order));
                break;
            case ComponentType::CornerReflector:
                chi -= (1.0 - 1.0 / (2.0 * static_cast<double>(component.order)));
                break;
            case ComponentType::Crosscap:
            case ComponentType::Handle:
                // already accounted for above
                break;
            case ComponentType::MirrorBoundary:
                seenMirrorBoundary = true;
                break;
        }
    }

    // If mirrored boundary was specified but no explicit corners appeared,
    // assume a single boundary cycle which does not change chi further.
    if (hasMirrorBoundary_ && !seenMirrorBoundary) {
        chi -= 1.0;
    }

    return chi;
}

const std::unordered_map<std::string, std::string> &OrbifoldSymbol::wallpaperLookup() {
    static const std::unordered_map<std::string, std::string> mapping = {
        {"*632", "p6m"},
        {"632", "p6"},
        {"*442", "p4m"},
        {"4*2", "p4g"},
        {"442", "p4"},
        {"*333", "p3m1"},
        {"3*3", "p31m"},
        {"333", "p3"},
        {"*2222", "pmm"},
        {"2*22", "pmg"},
        {"22*", "pgg"},
        {"2222", "p2"},
        {"*22", "cmm"},
        {"*x", "cm"},
        {"xx", "pg"},
        {"o", "p1"},
        {"*", "pm"}
    };

    return mapping;
}

std::string OrbifoldSymbol::wallpaperGroup() const {
    const auto &table = wallpaperLookup();
    const auto it = table.find(symbol_);
    if (it == table.end()) {
        throw std::runtime_error("Orbifold symbol not recognized as a wallpaper group: " + symbol_);
    }
    return it->second;
}

} // namespace orbifold

