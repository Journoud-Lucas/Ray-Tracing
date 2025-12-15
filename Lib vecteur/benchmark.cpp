#include <chrono>
#include <iostream>

#include "benchmark.h"

void BenchmarkRay(const Ray& ray, std::vector<Sphere>& spheres, const std::string& name)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::pair<double, Sphere*> hit = Sphere::RayIntersectSpheres(spheres, ray);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duration = end - start;

    std::cout << name << ": " << duration.count() << " microseconds\n";
}
