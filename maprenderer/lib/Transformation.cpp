#include "Transformation.h"
#include <iostream>

glm::vec2 Transformation::stereographic(const glm::vec2 &radians)
{
    double lonRad = radians.x;
    double latRad = radians.y;

    double denom = 1 + glm::cos(latRad) * glm::cos(lonRad / 2);
    double x = glm::cos(latRad) * glm::sin(lonRad / 2) / denom;
    double y = glm::sin(latRad) / denom;

    return glm::vec2(x, y);
}

glm::vec2 Transformation::orthographic(const glm::vec2 &radians)
{
    double lonRad = radians.x;
    double latRad = radians.y;

    double x = glm::cos(latRad) * glm::sin(lonRad);
    double y = glm::sin(latRad);

    return glm::vec2(x, y);
}
glm::vec2 Transformation::mercator(const glm::vec2 &radians)
{
    double lonRad = radians.x;
    double latRad = radians.y;

    double x = lonRad;
    double y = glm::log(glm::tan(glm::pi<double>() / 4.0 + latRad / 2.0));

    return glm::vec2(x, y);
}
glm::vec2 Transformation::normalize(const glm::vec2 &radians)
{
    double x = radians.x;
    double y = radians.y;
    // Normalize x and y to the range [-1, 1]
    double normalizedX = x / glm::pi<double>();
    double normalizedY = y / glm::pi<double>();
    return glm::vec2(normalizedX, normalizedY);
}
glm::vec2 Transformation::map(const glm::vec2 &normalized, int width, int height)
{
    double x = normalized.x;
    double y = normalized.y;
    // Map normalized coordinates to the range [0, width] for x and [0, height] for y
    double mappedX = (x + 1.0) * 0.5 * width;
    double mappedY = (y + 1.0) * 0.5 * height;
    return glm::vec2(mappedX, mappedY);
}

glm::vec2 Transformation::unmap(const glm::vec2& point, int width, int height) 
{
    // Reverse the mapping from screen coordinates to normalized coordinates
    double normalizedX = (double)point.x / width * 2.0 - 1.0;
    double normalizedY = (double)point.y / height * 2.0 - 1.0;
    return glm::vec2(normalizedX, normalizedY);
}

glm::vec2 Transformation::inverseMercator(const glm::vec2& point) 
{
    // Reverse the Mercator projection to get latitude and longitude in radians
    double lonRad = point.x;
    double latRad = glm::atan(glm::sinh(point.y));
    return glm::vec2(lonRad, latRad);
}