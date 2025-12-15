#pragma once

#include <random>

class RNG {
public:
    explicit RNG(uint32_t seed)
        : gen(seed), dist(0.0, 1.0) {
    }

    double uniform() {
        return dist(gen);
    }

private:
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;
};
