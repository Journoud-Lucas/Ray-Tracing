#include "Color.h"
#include "Operators.h"

#include <algorithm>
#include <cmath>

/// <summary>
/// Constructor for Color.
/// Initializes the color with the given RGB components.
/// </summary>
/// <param name="r">Red component.</param>
/// <param name="g">Green component.</param>
/// <param name="b">Blue component.</param>
/// <param name="a">Alpha component (default MAX_VALUE).</param>
Color::Color(double r, double g, double b, double a)
{
    m_r = r;
    m_g = g;
    m_b = b;
    m_a = a;
}

/// <summary>
/// Adds two colors component-wise and returns the result.
/// </summary>
/// <param name="other">The color to add.</param>
/// <returns>A new Color resulting from the addition.</returns>
Color Color::operator+(const Color& other) const
{
    return Color
    (
        m_r + other.GetR(),
        m_g + other.GetG(),
        m_b + other.GetB(),
        m_a + other.GetA()
    );
}

/// <summary>
/// Multiplies the color by a scalar.
/// </summary>
/// <param name="scalar">The scalar value to multiply with.</param>
/// <returns>A new Color scaled by the given value.</returns>
Color Color::operator*(int scalar) const
{
    return Color(m_r * scalar, m_g * scalar, m_b * scalar, m_a);
}

/// <summary>
/// Multiplies the color by a scalar (double).
/// </summary>
/// <param name="scalar">The scalar value to multiply with.</param>
/// <returns>A new Color scaled by the given value.</returns>
Color Color::operator*(double scalar) const
{
    return Color(m_r * scalar, m_g * scalar, m_b * scalar, m_a);
}

/// <summary>
/// Multiplies two colors component-wise and returns the result.
/// </summary>
/// <param name="other">The color to multiply with.</param>
/// <returns>A new Color resulting from the multiplication.</returns>
Color Color::operator*(const Color& other) const
{
    return Color(
        m_r * other.m_r,
        m_g * other.m_g,
        m_b * other.m_b,
        m_a
    );
}

/// <summary>
/// Adds another color to this one in place.
/// </summary>
/// <param name="other">The color to add.</param>
/// <returns>Reference to the modified color.</returns>
Color& Color::operator+=(const Color& other)
{
    m_r += other.GetR();
    m_g += other.GetG();
    m_b += other.GetB();
	// m_a remains unchanged

    return *this;
}

/// <summary>
/// Divides the color by a scalar in place.
/// </summary>
/// <param name="scalar">The scalar value to divide by.</param>
/// <returns>Reference to the modified color.</returns>
Color& Color::operator/=(int scalar)
{
    m_r /= scalar;
    m_g /= scalar;
    m_b /= scalar;
    // m_a remains unchanged
    return *this;
}

Color Color::Tonemap(double r, double g, double b, double a)
{
    // Gamma correction
    r = std::pow(r, 1.0 / 2.2);
    g = std::pow(g, 1.0 / 2.2);
    b = std::pow(b, 1.0 / 2.2);

    double R = static_cast<int>(std::clamp(r * ColorConstants::MAX_VAL, 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
    double G = static_cast<int>(std::clamp(g * ColorConstants::MAX_VAL, 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
    double B = static_cast<int>(std::clamp(b * ColorConstants::MAX_VAL, 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
	double A = static_cast<int>(std::clamp(a * ColorConstants::MAX_VAL, 0.0, static_cast<double>(ColorConstants::MAX_VAL)));

    return Color(R, G, B, A);
}

Color Color::Tonemap(Color color)
{
	return Tonemap(color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

bool Color::operator==(const Color& other) const
{
    return m_r == other.m_r &&
        m_g == other.m_g &&
        m_b == other.m_b &&
        m_a == other.m_a;
}

bool Color::operator!=(const Color& other) const
{
    return !(*this == other);
}
