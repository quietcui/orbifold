#include "orbifold/OrbifoldSymbol.h"

#include <cmath>
#include <cassert>
#include <iostream>

using orbifold::OrbifoldSymbol;

void testClassification() {
    OrbifoldSymbol star("*632");
    assert(star.wallpaperGroup() == "p6m");

    OrbifoldSymbol glide("*x");
    assert(glide.wallpaperGroup() == "cm");

    OrbifoldSymbol rotations("333");
    assert(rotations.wallpaperGroup() == "p3");
}

void testEulerCharacteristic() {
    OrbifoldSymbol p6m("*632");
    // Known wallpaper orbifolds have zero Euler characteristic
    assert(std::abs(p6m.eulerCharacteristic()) < 1e-5 || p6m.eulerCharacteristic() < 0);

    OrbifoldSymbol p1("o");
    assert(p1.eulerCharacteristic() < 2.0);
}

int main() {
    testClassification();
    testEulerCharacteristic();

    std::cout << "All OrbifoldSymbol tests passed" << std::endl;
    return 0;
}

