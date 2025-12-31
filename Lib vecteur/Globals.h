#pragma once

#include <cstdint>

namespace RayTracer
{
    constexpr auto NO_INTERSECTION = -1;
	constexpr int MAX_DEPTH = 5;
	constexpr int SAMPLES_PER_PIXEL = 100;
	constexpr double EPSILON = 0.0001;
}

namespace ColorConstants
{
    constexpr auto MAX_VAL = 255;
}

inline uint32_t PixelSeed(int x, int y)
{
    uint32_t h = 2166136261u;
    h = (h ^ x) * 16777619u;
    h = (h ^ y) * 16777619u;
    return h;
}
