#pragma once
#include <vector>

#include "Sphere.h"
#include "Light.h"

class Scene
{
public:
    void AddObject(const Sphere& sphere)
    {
        m_objects.push_back(sphere);
    }

    void AddLight(const Light& light)
    {
        m_lights.push_back(light);
    }

    const std::vector<Sphere>& GetObjects() const { return m_objects; }
    const std::vector<Light>& GetLights() const { return m_lights; }

private:
    std::vector<Sphere> m_objects;
    std::vector<Light>  m_lights;
};
