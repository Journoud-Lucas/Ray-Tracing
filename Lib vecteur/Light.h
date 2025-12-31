#pragma once

#include "Point.h"
#include "Color.h"

class Sphere;

enum class LightBehaviorType { PointLight, SphereLight };

class LightBehavior
{
public:
    LightBehaviorType type;
    Point position;
    Sphere* sphere;

    LightBehavior(Point pos) : type(LightBehaviorType::PointLight), position(pos), sphere(nullptr) {}
    LightBehavior(Sphere* s) : type(LightBehaviorType::SphereLight), sphere(s) {}
};

class Light
{
public:
    Light(LightBehavior behavior, const Color& emission)
        : m_behavior(behavior), m_emission(emission) {}

    const LightBehavior& GetBehavior() const { return m_behavior; }
    const Color& GetEmission() const { return m_emission; }
    Point GetPosition() const
    {
        switch (m_behavior.type)
        {
        case LightBehaviorType::PointLight:
            return m_behavior.position;
        case LightBehaviorType::SphereLight:
            return Point(0, 0, 0);
        default:
            return Point(0, 0, 0); // fallback
        }
    }

private:
    LightBehavior m_behavior;
    Color m_emission;
};
