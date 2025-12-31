#pragma once

#include "Point.h"
#include <vector>
#include "Ray.h"
#include "Color.h"
#include "Material.h"
#include "RNG.h"

class Scene;
class Light;

class Sphere
{
private:
    Point m_center;
    double m_radius;
    Material m_material;
    bool m_isLightSphere;

public:
    Sphere(const Point& center, double radius, Material material);

    Point GetCenter() const { return m_center; }
    double GetRadius() const { return m_radius; }
    Material GetMaterial() const { return m_material; }

    double RayIntersectSphere(const Ray& ray) const;

    void SetAsLight(bool isLightSphere) { m_isLightSphere = isLightSphere; }
    bool IsLightSphere() const { return m_isLightSphere; }

    static std::pair<double, Sphere*> RayIntersectSpheres(const std::vector<Sphere>& spheres, const Ray& ray);
    static std::optional<Color> RadianceSpheres(
        const Ray& ray,
        Scene& scene,
        int depth,
        bool lastWasSpecular,
        RNG& rng
    );
    static std::optional<Color> SubRadiance(
        const Ray& ray,
        Scene& scene,
        int depth,
        bool lastWasSpecular,
        RNG& rng
    );
    static std::optional<Color> MirrorContribution(const Point& intersectionPoint,
        const Direction& normal,
        const Ray& ray, Scene& scene, Sphere* hitSphere, int depth, double epsilon, RNG& rng);
    static std::pair<Direction, double> SampleCosinus(RNG& rng);
    static Color DirectLighting(const Point& intersectionPoint, const Direction& normal,
        const Material& material, const Light& light, const std::vector<Sphere>& spheres, RNG& rng);
};
