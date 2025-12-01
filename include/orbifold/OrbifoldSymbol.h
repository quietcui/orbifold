#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace orbifold {

enum class ComponentType {
    ConePoint,
    CornerReflector,
    Crosscap,
    Handle,
    MirrorBoundary
};

struct Component {
    ComponentType type;
    int order{0};
};

class OrbifoldSymbol {
public:
    explicit OrbifoldSymbol(std::string symbol);

    [[nodiscard]] const std::string &symbol() const noexcept { return symbol_; }
    [[nodiscard]] const std::vector<Component> &components() const noexcept { return components_; }

    [[nodiscard]] bool hasMirrorBoundary() const noexcept { return hasMirrorBoundary_; }
    [[nodiscard]] std::size_t crosscaps() const noexcept { return crosscaps_; }
    [[nodiscard]] std::size_t handles() const noexcept { return handles_; }

    [[nodiscard]] double eulerCharacteristic() const;
    [[nodiscard]] std::string wallpaperGroup() const;

private:
    std::string symbol_;
    bool hasMirrorBoundary_{false};
    std::size_t crosscaps_{0};
    std::size_t handles_{0};
    std::vector<Component> components_;

    void parse();
    static std::optional<int> parseNumber(const std::string &text, std::size_t &index);
    static const std::unordered_map<std::string, std::string> &wallpaperLookup();
};

} // namespace orbifold

