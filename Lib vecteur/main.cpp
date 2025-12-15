#include <iostream>
#include <algorithm>
#include <chrono>
#include <execution>
#include <thread>

#include "Operators.h"
#include "Globals.h"
#include "benchmark.h"
#include "Point.h"
#include "Direction.h"
#include "Color.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"

template <typename T>
std::vector<std::vector<T>> chunksOf(size_t n, const std::vector<T>& data)
{
    std::vector<std::vector<T>> result;
    for (size_t i = 0; i < data.size(); i += n)
    {
        size_t end = std::min(i + n, data.size());
        result.emplace_back(data.begin() + i, data.begin() + end);
    }
    return result;
}


template <typename Func>
Image withImageParallel(int w, int h, Func f)
{
    Image img(w, h, Color(0, 0, 0, 0));

    std::vector<int> ys(h);
    for (int i = 0; i < h; ++i)
    {
        ys[i] = i;
    }

    auto chunks = chunksOf(10, ys);

    std::for_each(std::execution::par, chunks.begin(), chunks.end(),
        [&](const std::vector<int>& ysChunk)
        {
            for (int y : ysChunk)
            {
                for (int x = 0; x < w; ++x)
                {
                    img.SetPixel(x, y, f(x, y));
                }
            }
        });

    return img;
}

/// <summary>
/// Entry point of the program.
/// Demonstrates the creation of a Point and a Direction.
/// </summary>
/// <returns>Program exit code (0 = success).</returns>
int main()
{
    const int width = 500;
    const int height = 500;

    Scene scene;
    double sphereRadius = 5000;

    scene.AddObject(Sphere(Point(sphereRadius + 500, 250, 0), sphereRadius, Material(0.6, 0.6, 0, MaterialBehavior::Diffuse))); // Right
    scene.AddObject(Sphere(Point(-sphereRadius, 250, 0), sphereRadius, Material(0, 0.6, 0.6, MaterialBehavior::Diffuse))); // Left
    scene.AddObject(Sphere(Point(250, -sphereRadius, 0), sphereRadius, Material(0.6, 0.6, 0.6, MaterialBehavior::Diffuse))); // Top
    scene.AddObject(Sphere(Point(250, sphereRadius + 500, 0), sphereRadius, Material(0.6, 0.6, 0.6, MaterialBehavior::Diffuse))); // Bottom
    scene.AddObject(Sphere(Point(250, 250, sphereRadius + 500), sphereRadius, Material(0.6, 0.6, 0.6, MaterialBehavior::Diffuse))); // Back
    scene.AddObject(Sphere(Point(150, 350, 250), 100, Material(1, 1, 1, MaterialBehavior::Mirror))); // Sphere 1
    scene.AddObject(Sphere(Point(350, 350, 250), 100, Material(1, 1, 1, MaterialBehavior::Glass, 2.4))); // Sphere 2

    scene.AddLight(Light(Point(250, 250, 250), Color(30000, 30000, 30000)));
    scene.AddLight(Light(Point(50, 250, 250), Color(30000, 10000, 10000)));
    scene.AddLight(Light(Point(450, 250, 250), Color(0, 10000, 30000)));

    auto start = std::chrono::high_resolution_clock::now();

    auto pixelFunc = [&](int x, int y) -> Color
    {
        RNG rng(PixelSeed(x, y)); // seed per pixel

        Color contrib(0, 0, 0);
        int validSamples = 0;

        for (int i = 0; i < RayTracer::SAMPLES_PER_PIXEL; ++i)
        {
            double dx = rng.uniform() - 0.5;
            double dy = rng.uniform() - 0.5;

            double coeffOpening = 1.001;
            Point pNear(x + dx, y + dy, 0.0);
            double xPrime = pNear.GetX() - 250.0;
            double yPrime = pNear.GetY() - 250.0;

            Point pFar(coeffOpening * xPrime + 250.0,
                coeffOpening * yPrime + 250.0,
                1.0);

            Direction dir = (pFar - pNear).Normalized();
            Ray ray(pNear, dir);

            auto colOpt = Sphere::RadianceSpheres(ray, scene, 0, rng);

            if (colOpt.has_value())
            {
                contrib += *colOpt;
                validSamples++;
            }
        }

        double validFactor = double(validSamples) / double(RayTracer::SAMPLES_PER_PIXEL);
        Color average = contrib * (1.0 / double(RayTracer::SAMPLES_PER_PIXEL));
        return Color::Tonemap(average * validFactor);
    };

    Image img = withImageParallel(width, height, pixelFunc);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    img.SavePPM("first_image.ppm");
    system("start first_image.ppm");

    std::cout << "Image saved to first_image.ppm\n";
    std::cout << "Render time: " << elapsed.count() << " seconds\n";

    return 0;
}
