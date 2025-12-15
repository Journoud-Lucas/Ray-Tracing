#pragma once

#include "Direction.h"
#include <utility>
#include <optional>

// Enum for material behavior
enum class MaterialBehavior
{
    Diffuse,
    Glass,
    Mirror
};

class Material
{
private:
    double m_r;
    double m_g;
    double m_b;
	MaterialBehavior m_behavior;
    double m_ior;

public:
    Material(double r, double g, double b, MaterialBehavior behavior, double ior = 1.5);

	double GetR() const { return m_r; }
	double GetG() const { return m_g; }
	double GetB() const { return m_b; }
	MaterialBehavior GetBehavior() const { return m_behavior; }
	double GetIor() const { return m_ior; }

    static Direction Reflect(const Direction& normal, const Direction& incoming);
    static std::optional<std::pair<double, Direction>> Refract(double ior, const Direction& normal, const Direction& incoming, bool outside);
};
