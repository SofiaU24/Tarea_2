#pragma once
#include <vector>
#include "Rng.h"
#include "Types.h"

struct BspResult{
    std::vector<Rect> houses;
    std::vector<std::pair<Rect, Rect>> streets;
};

BspResult generateBsp(int w, int h, int minSize, Rng& rng); 