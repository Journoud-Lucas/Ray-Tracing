#pragma once

#include <vector>
#include <string>

#include "Color.h"

class Image
{
public:
    // Constructor with background color
    Image(int width, int height, const Color& background = Color(0, 0, 0));
    // Constructor that takes a vector of pixels
    Image(int width, int height, const std::vector<Color>& pixels);

    void SetPixel(int x, int y, const Color& c);
    Color GetPixel(int x, int y) const;

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    void SavePPM(const std::string& filename) const;

private:
    int m_width;
    int m_height;
    std::vector<Color> m_pixels;
};
