#include "Material.h"
#include "Operators.h"

Material::Material(double r, double g, double b, MaterialBehavior behavior, Color emission, double ior)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_behavior = behavior;
    m_emission = emission;
	m_ior = ior;
}

Direction Material::Reflect(const Direction& normal, const Direction& incoming)
{
	// TODO: normalize is useless here since n and wi are normalized

	double proj = -normal.Dot(incoming);
	Direction reflected = (2.0 * proj) * normal + incoming;
	return reflected.Normalized(); // normalize to ensure consistent magnitude
}


std::optional<std::pair<double, Direction>> Material::Refract(
    double ior,
    const Direction& normal,
    const Direction& incoming,
    bool outside)
{
    Direction n = normal;
    Direction d = incoming.Normalized();

    double eta = outside ? 1.0 / ior : ior;
    double cos_t1 = d.Dot(n);

    double cos_t2_2 = 1.0 - eta * eta * (1.0 - cos_t1 * cos_t1);
    if (cos_t2_2 < 0.0)
    {
        return std::nullopt;
    }

    Direction tdir =
        (eta * d) - n * (eta * cos_t1 + std::sqrt(cos_t2_2));
    tdir.Normalize();

    double r0 = std::pow((ior - 1.0) / (ior + 1.0), 2.0);

    double cosTheta = outside ? -cos_t1 : std::abs(tdir.Dot(n));

    double reflectCoef = r0 + (1.0 - r0) * std::pow(1.0 - cosTheta, 5.0);

    double transmitCoef = 1.0 - reflectCoef;

    return std::make_pair(transmitCoef, tdir);
}
