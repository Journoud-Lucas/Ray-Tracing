#include <fstream>
#include <stdexcept>
#include <algorithm>

#include "Image.h"
#include "Globals.h"

/// <summary>
/// Constructor that creates an image of the given size filled with a background color.
/// </summary>
/// <param name="width">Image width in pixels.</param>
/// <param name="height">Image height in pixels.</param>
/// <param name="background">Color to fill the image with.</param>
Image::Image(int width, int height, const Color& background)
{
    m_width = width;
    m_height = height;
    m_pixels = std::vector<Color>(width * height, background);
}

/// <summary>
/// Constructor that creates an image from a given vector of pixels.
/// </summary>
/// <param name="width">Image width in pixels.</param>
/// <param name="height">Image height in pixels.</param>
/// <param name="pixels">Vector of colors representing the image pixels (row-major order).</param>
/// <exception cref="std::runtime_error">Thrown if the size of the vector does not match width * height.</exception>
Image::Image(int width, int height, const std::vector<Color>& pixels)
{
    m_width = width;
    m_height = height;
    m_pixels = pixels;

    if (pixels.size() != width * height)
    {
        throw std::runtime_error("The vector size does not match width*height!");
    }
}

/// <summary>
/// Sets the color of a specific pixel in the image.
/// </summary>
/// <param name="x">X coordinate of the pixel (0-based).</param>
/// <param name="y">Y coordinate of the pixel (0-based).</param>
/// <param name="c">Color to set at the specified pixel.</param>
/// <exception cref="std::out_of_range">Thrown if the pixel coordinates are outside the image bounds.</exception>
void Image::SetPixel(int x, int y, const Color& c)
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        throw std::out_of_range("Out-of-bounds pixel");
    }

    m_pixels[y * m_width + x] = c;
}

/// <summary>
/// Returns the color of a specific pixel in the image.
/// </summary>
/// <param name="x">X coordinate of the pixel (0-based).</param>
/// <param name="y">Y coordinate of the pixel (0-based).</param>
/// <returns>The color at the specified pixel.</returns>
/// <exception cref="std::out_of_range">Thrown if the pixel coordinates are outside the image bounds.</exception>
Color Image::GetPixel(int x, int y) const
{
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
    {
        throw std::out_of_range("Out-of-bounds pixel");
    }

    return m_pixels[y * m_width + x];
}

/// <summary>
/// Saves the image to a PPM file in binary (P6) format.
/// </summary>
/// <param name="filename">Name of the file to save the image to.</param>
/// <exception cref="std::runtime_error">Thrown if the file cannot be opened for writing.</exception>
void Image::SavePPM(const std::string& filename) const
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs)
    {
        throw std::runtime_error("Unable to open the file " + filename);
    }

    ofs << "P6\n" << m_width << " " << m_height << "\n" << ColorConstants::MAX_VAL << "\n";

    for (const Color &c : m_pixels)
    {
        unsigned char r = static_cast<unsigned char>(std::clamp(c.GetR(), 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
        unsigned char g = static_cast<unsigned char>(std::clamp(c.GetG(), 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
        unsigned char b = static_cast<unsigned char>(std::clamp(c.GetB(), 0.0, static_cast<double>(ColorConstants::MAX_VAL)));
        ofs.write(reinterpret_cast<char*>(&r), 1);
        ofs.write(reinterpret_cast<char*>(&g), 1);
        ofs.write(reinterpret_cast<char*>(&b), 1);
    }
}
