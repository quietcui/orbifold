#include "orbifold/OrbifoldSymbol.h"

#include <iostream>
#include <vector>

int main() {
    using orbifold::OrbifoldSymbol;

    std::vector<std::string> examples = {"*632", "4*2", "333", "*x", "o"};

    for (const auto &symbol : examples) {
        OrbifoldSymbol orb(symbol);
        std::cout << "Symbol: " << symbol << "\n";
        try {
            std::cout << "  Wallpaper group: " << orb.wallpaperGroup() << "\n";
        } catch (const std::exception &ex) {
            std::cout << "  Wallpaper group: " << ex.what() << "\n";
        }
        std::cout << "  Euler characteristic (approx): " << orb.eulerCharacteristic() << "\n\n";
    }

    return 0;
}

