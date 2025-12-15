#pragma once

#include "Point.h"
#include "Color.h"

class Light
{
public:
    Light(const Point& pos, const Color& col)
        : m_position(pos), m_emission(col)
    {
    }

    const Point& GetPosition() const { return m_position; }
    const Color& GetEmission() const { return m_emission; }

private:
    Point m_position;
    Color m_emission;
};
