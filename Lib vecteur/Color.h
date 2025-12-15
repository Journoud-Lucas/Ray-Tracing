#pragma once

#include "Globals.h"

class Color
{
public:
    Color(double r = 0, double g = 0, double b = 0, double a = ColorConstants::MAX_VAL);
    
    void SetR(int r) { m_r = r; }
    void SetG(int g) { m_g = g; }
    void SetB(int b) { m_b = b; }
    void SetA(int a) { m_a = a; }
    
    double GetR() const { return m_r; }
    double GetG() const { return m_g; }
    double GetB() const { return m_b; }
    double GetA() const { return m_a; }

    static Color Tonemap(double r, double g, double b, double a);
    static Color Tonemap(Color color);
    static double LinearToSRGB(double c);

    Color operator+(const Color& other) const;
    Color operator*(int scalar) const;
    Color operator*(double scalar) const;
    Color operator*(const Color& other) const;
    Color& operator+=(const Color& other);
	Color& operator/=(int scalar);

private:
    double m_r, m_g, m_b, m_a;
};
