#pragma once

#include <vector>
#include <string>

#include "Ray.h"
#include "Sphere.h"

void BenchmarkRay(const Ray& ray, std::vector<Sphere>& spheres, const std::string& name);
