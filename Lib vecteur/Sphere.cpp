#include <algorithm>
#include <utility>
#include <numbers>

#include "Scene.h"
#include "Sphere.h"
#include "Ray.h"
#include "Globals.h"
#include "Operators.h"

/// <summary>
/// Constructor for Sphere.
/// Initializes the sphere with a center point and radius.
/// </summary>
/// <param name="center">Center point of the sphere.</param>
/// <param name="radius">Radius of the sphere.</param>
/// <param name="material">Material of the sphere.</param>
Sphere::Sphere(const Point& center, double radius, Material material) : m_material(material)
{
    m_center = center;
    m_radius = radius;
    m_isLightSphere = false;
}

/// <summary>
/// Checks for intersection between a ray and a sphere.
/// </summary>
/// <param name="sphere">The sphere to check for intersection.</param>
/// <param name="ray">The ray to test for intersection.</param>
/// <returns>The distance to the intersection, or -1.0 if no intersection.</returns>
double Sphere::RayIntersectSphere(const Ray& ray) const
{
    Direction OC = this->GetCenter() - ray.GetOrigin();

    double radiusSquared = pow(this->GetRadius(), 2);

    double b = ray.GetDirection().Dot(OC);
    double c = OC.Dot(OC) - radiusSquared;

    double delta = std::pow(b,2) - c;
    if (delta < 0)
    {
        return RayTracer::NO_INTERSECTION; // No intersection
    }

	// Can be avoided cuz if delta < 0 sqrt(delta)  is NaN

    double sqrtDelta = sqrt(delta);
    double intersection0 = b - sqrtDelta;
    double intersection1 = b + sqrtDelta;

    if (intersection0 >= 0)
    {
        return intersection0;
    }
    else if (intersection1 >= 0)
    {
        return intersection1;
    }
    else
	{
        return RayTracer::NO_INTERSECTION; // Intersection behind the camera
    }


	// return nothing instead of NO_INTERSECTION
	// le code haskell écris | otherwise -> Nothing
}

/// <summary>
/// Checks for intersection between a ray and multiple spheres.
/// </summary>
/// <param name="spheres">Standard vector of spheres to check for intersection.</param>
/// <param name="ray">The ray to test for intersection.</param>
/// <returns>A pair containing the closest intersection distance and a pointer to the hit sphere (or nullptr if no intersection).</returns>
std::pair<double, Sphere*> Sphere::RayIntersectSpheres(
    const std::vector<Sphere>& spheres,
    const Ray& ray)
{
    std::pair<double, Sphere*> closest{ RayTracer::NO_INTERSECTION, nullptr };

    for (const Sphere& sphere : spheres)
    {
        double t = sphere.RayIntersectSphere(ray);
        if (t != RayTracer::NO_INTERSECTION &&
            (closest.first == RayTracer::NO_INTERSECTION || t < closest.first))
        {
            closest = { t, const_cast<Sphere*>(&sphere) };
        }
    }
    return closest;
}


/// <summary>
/// Computes the radiance color for a ray intersecting multiple spheres using Russian Roulette.
/// </summary>
/// <param name="ray">The ray to test for intersection.</param>
/// <param name="spheres">Standard vector of spheres to check for intersection.</param>
/// <param name="lightPos">Position of the light source.</param>
/// <param name="lightEmission">Color of the light source.</param>
/// <param name="depth">Current recursion depth.</param>
/// <returns>>The color based on the closest intersection distance.</returns>
std::optional<Color> Sphere::RadianceSpheres(
    const Ray& ray,
    Scene& scene,
    int depth,
    bool lastWasSpecular,
	RNG& rng
)
{
    if (depth >= RayTracer::MAX_DEPTH)
    {
        return Color(0, 0, 0);
    }

    double r = rng.uniform();
    double coefRR = (depth == 0) ? 1.0 : 0.5;

    if (r < coefRR)
    {
        auto colOpt = SubRadiance(ray, scene, depth, lastWasSpecular, rng);
        if (!colOpt)
        {
            return std::nullopt;
        }

        Color col = *colOpt;
        return col * (1.0 / coefRR); // adjustment Russian Roulette
    }
    else
    {
        return Color(0, 0, 0); // terminate the ray
    }
}

Color Sphere::DirectLighting(
    const Point& intersectionPoint,
    const Direction& normal,
    const Material& material,
    const Light& light,
    const std::vector<Sphere>& spheres,
    RNG& rng
)
{
    Direction directionToLight;
    double coefNormLight = 1.0;

    if (light.GetBehavior().type == LightBehaviorType::PointLight)
    {
        directionToLight = light.GetPosition() - intersectionPoint;
        coefNormLight = 1.0;
    }
    else
    {
        Sphere* lightSphere = light.GetBehavior().sphere;
        double radius = lightSphere->GetRadius();
        Point center = lightSphere->GetCenter();

        auto [localDir, pdf] = Sphere::SampleCosinus(rng);

        Direction axis = (center - intersectionPoint).Normalized();
        Direction sampledDir = Direction::RotateVector(axis, localDir);

        Point pointOnLight = center + radius * sampledDir;

        directionToLight = pointOnLight - intersectionPoint;
        Direction normalLight = (pointOnLight - center).Normalized();
        Direction wi = directionToLight.Normalized();

        double cosLight = std::max(0.0, -normalLight.Dot(wi));

        double coefNormLight = cosLight / pdf;
    }

    double lightDistanceSquared = directionToLight.Dot(directionToLight);
    Direction wi = directionToLight.Normalized();

    double sqrtD = std::sqrt(lightDistanceSquared);
    //TODO: Optimize this
    double maxDistance = lightDistanceSquared + 4 * RayTracer::EPSILON * RayTracer::EPSILON - 4 * RayTracer::EPSILON * sqrtD;

    Ray shadowRay(intersectionPoint + RayTracer::EPSILON * wi, wi);
    for (const Sphere& sphere : spheres)
    {
        if (&sphere == light.GetBehavior().sphere || sphere.IsLightSphere())
            continue;
        double t = sphere.RayIntersectSphere(shadowRay);
        if (t != RayTracer::NO_INTERSECTION && t * t < maxDistance)
        {
            return Color(0, 0, 0);
        }
    }

    double cosTheta = std::max(0.0, normal.Dot(wi));
    if (cosTheta <= 0.0)
        return Color(0, 0, 0);

    double coeff = coefNormLight * cosTheta / (std::numbers::pi * lightDistanceSquared);

    return Color(
        coeff * material.GetR() * light.GetEmission().GetR(),
        coeff * material.GetG() * light.GetEmission().GetG(),
        coeff * material.GetB() * light.GetEmission().GetB()
    );
}


/// <summary>
/// Computes the sub-radiance color for a ray intersecting multiple spheres.
/// </summary>
/// <param name="ray">The ray to test for intersection.</param>
/// <param name="spheres">Standard vector of spheres to check for intersection.</param>
/// <param name="lightPos">Position of the light source.</param>
/// <param name="lightEmission">Color of the light source.</param>
/// <param name="depth">Current recursion depth.</param>
/// <returns>The color based on the closest intersection distance.</returns>
std::optional<Color> Sphere::SubRadiance(const Ray& ray, Scene& scene, int depth, bool lastWasSpecular, RNG& rng)
{
    if (depth > RayTracer::MAX_DEPTH)
    {
        return std::nullopt;
    }

    auto [closestIntersection, hitSphere] = Sphere::RayIntersectSpheres(scene.GetObjects(), ray);

    if (closestIntersection == RayTracer::NO_INTERSECTION)
    {
		return std::nullopt;
    }
    else
    {
        Point intersectionPoint = ray.GetOrigin() + closestIntersection * ray.GetDirection();
        Direction directionToLight = scene.GetLights()[0].GetPosition() - intersectionPoint;
        Direction directionToLightNormalized = directionToLight.Normalized();
        Direction normal = (intersectionPoint - hitSphere->GetCenter()).Normalized();

        auto mirrorContribution = [&]() -> std::optional<Color>
        {
            return MirrorContribution(intersectionPoint, normal, ray, scene, hitSphere,
                depth, RayTracer::EPSILON, rng);
        };


        switch (hitSphere->GetMaterial().GetBehavior())
        {
            case (MaterialBehavior::Diffuse):
            {
                int lu = int(rng.uniform() * scene.GetLights().size());
                Color directLightContrib = Sphere::DirectLighting(
                    intersectionPoint,
                    normal,
                    hitSphere->GetMaterial(),
                    scene.GetLights()[lu],
                    scene.GetObjects(),
                    rng
                ) * static_cast<double>(scene.GetLights().size());

                auto [indirectDirLocal, pdf] = SampleCosinus(rng);

                Direction indirectDir = Direction::RotateVector(normal, indirectDirLocal);

                Ray indirectRay(intersectionPoint + RayTracer::EPSILON * indirectDir, indirectDir);
                std::optional<Color> indirectOpt = Sphere::RadianceSpheres(
                    indirectRay, scene, depth + 1, false, rng
                );

                Color indirect = indirectOpt.value_or(Color(0, 0, 0));

                Color indirectFinal(
                    hitSphere->m_material.GetR() * indirect.GetR(),
                    hitSphere->m_material.GetG() * indirect.GetG(),
                    hitSphere->m_material.GetB() * indirect.GetB()
                );

                double coefIndirect = indirectDir.Dot(normal) / (std::numbers::pi * pdf);

                Color emission;
                if (lastWasSpecular)
                {
                    emission = hitSphere->GetMaterial().GetEmission();
                }
                else
                {
                    emission = Color(0, 0, 0);
                }
                return directLightContrib + indirectFinal * coefIndirect + emission;
            }
            case (MaterialBehavior::Glass):
            {
                bool outside = ray.GetDirection().Dot(normal) < 0;
                Direction biasNormal = outside ? normal : -normal;

                std::optional<std::pair<double, Direction>>
                    refractOpt = Material::Refract(
                    hitSphere->m_material.GetIor(),
                    biasNormal,
                    ray.GetDirection(),
                    outside
                );

                if (!refractOpt.has_value())
                {
                    return mirrorContribution();
                }

                double transmitProb = refractOpt->first;
                Direction transmittedDirection = refractOpt->second;

                Direction reflectedDirection =
                    Material::Reflect(normal, ray.GetDirection()).Normalized();

                double r = rng.uniform();

                if (r < transmitProb)
                {
                    Ray transmittedRay(
                        intersectionPoint + RayTracer::EPSILON * 3 * transmittedDirection,
                        transmittedDirection
                    );

                    std::optional<Color> colOpt = Sphere::RadianceSpheres(
                        transmittedRay,
						scene,
                        depth + 1,
                        true,
                        rng
                    );

                    if (!colOpt)
                    {
                        return std::nullopt;
                    }

                    Color transmittedColor = *colOpt;

                    return Color(
                        hitSphere->GetMaterial().GetR() * transmittedColor.GetR(),
                        hitSphere->GetMaterial().GetG() * transmittedColor.GetG(),
                        hitSphere->GetMaterial().GetB() * transmittedColor.GetB()
                    );
                }

                return mirrorContribution();
            }
            case (MaterialBehavior::Mirror):
            {
                return mirrorContribution();
            }
        }
    }
    return std::nullopt;
}

/// <summary>
/// Computes the mirror contribution for a ray intersecting a sphere.
/// </summary>
/// <param name="intersectionPoint">The point of intersection.</param>
/// <param name="normal">The normal at the intersection point.</param>
/// <param name="ray">The incoming ray.</param>
/// <param name="spheres">Standard vector of spheres in the scene.</param>
/// <param name="hitSphere">The sphere that was hit.</param>
/// <param name="lightPos">Position of the light source.</param>
/// <param name="lightEmission">Color of the light source.</param>
/// <param name="depth">Current recursion depth.</param>
/// <param name="epsilon">Small offset to avoid self-intersection.</param>
/// <returns>The color contribution from the mirror reflection.</returns>
std::optional<Color> Sphere::MirrorContribution(const Point& intersectionPoint,
    const Direction& normal, const Ray& ray, Scene& scene, Sphere* hitSphere,
    int depth, double epsilon, RNG& rng)
{
    Direction reflectedDirection = Material::Reflect(normal, ray.GetDirection()).Normalized();
    Ray reflectedRay(intersectionPoint + epsilon * reflectedDirection, reflectedDirection);

    std::optional<Color> colOpt = Sphere::RadianceSpheres(
        reflectedRay,
		scene,
        depth + 1,
        true,
        rng
    );

    if (!colOpt)
    {
        return std::nullopt;
    }

    Color reflectedColor = *colOpt;

    return Color(
        hitSphere->GetMaterial().GetR() * reflectedColor.GetR(),
        hitSphere->GetMaterial().GetG() * reflectedColor.GetG(),
        hitSphere->GetMaterial().GetB() * reflectedColor.GetB()
    );
}

/// <summary>
/// Samples a direction on the hemisphere with cosine-weighted distribution.
/// </summary>
/// <param name="u">Random variables in [0, 1).</param>
/// <param name="v">Random variables in [0, 1).</param>
/// <returns>A pair containing the sampled direction and its PDF.</returns>
std::pair<Direction, double> Sphere::SampleCosinus(RNG& rng)
{
    double u = rng.uniform();
    double v = rng.uniform();

    double phi = 2.0 * std::numbers::pi * u;
    double sqrt_v = std::sqrt(v);
    double cosTheta = sqrt_v;
    double sinTheta = std::sqrt(1.0 - v);

    double x = cos(phi) * sinTheta;
    double y = sin(phi) * sinTheta;
    double z = cosTheta;

    Direction dir(x, y, z);
    dir = dir.Normalized();

    double pdf = cosTheta / std::numbers::pi;

    return { dir, pdf };
}
