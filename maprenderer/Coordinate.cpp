#include "Coordinate.h"
#include <iostream>
Coordinate::Coordinate(double longitude, double latitude)
    : m_longitude(longitude),
      m_latitude(latitude)
{
}

Coordinate Coordinate::fromJson(const QJsonValue &json)
{
    double longitude = json.toArray().at(0).toDouble();
    double latitude = json.toArray().at(1).toDouble();
    return Coordinate(longitude, latitude);
}

glm::vec2 Coordinate::lonLat() const
{
    return glm::vec2(m_longitude, m_latitude);
}

/**
 * Perform a linear transformation and map an equirectangular projection by normalizing
 *  the longitude and latitude to the range [-1, 1] and mapping them to 2D coordinates.
 */
glm::vec2 Coordinate::normalizeAndMapTo2D(int width, int height) const
{
    // Normalize longitude and latitude to the range [-1, 1]
    double normalizedLongitude = m_longitude / DEGREES_IN_HALF_CIRCLE;
    double normalizedLatitude = m_latitude / DEGREES_IN_QUARTER_CIRCLE;

    // Map normalized coordinates to the range [0, width] for x and [0, height] for y
    double x = (normalizedLongitude + 1.0) * 0.5 * width;
    double y = (normalizedLatitude + 1.0) * 0.5 * height;

    return glm::vec2(x, y);
}

glm::vec2 Coordinate::stereographicProjection(double minX, double maxX, double minY, double maxY, double width, double height) const
{

    double lon = glm::radians(m_longitude);
    double lat = glm::radians(m_latitude);
    double k = 2 / (1 + sin(glm::radians(DEGREES_IN_QUARTER_CIRCLE)) + sin(lat));
    double x = k * cos(lat) * sin(lon);
    double y = k * cos(lat) * cos(lon);

    // Normalize x and y to the range [0, 1]
    double normalizedX = (x - minX) / (maxX - minX);
    double normalizedY = (y - minY) / (maxY - minY);

    // Map normalized coordinates to the range [0, width] for x and [0, height] for y
    double mappedX = normalizedX * width;
    double mappedY = normalizedY * height;

    return glm::vec2(mappedX, mappedY);
}

glm::vec2 Coordinate::stereographicProjection() const
{
    double lon = glm::radians(m_longitude);
    double lat = glm::radians(m_latitude);
    double k = 2 / (1 + sin(glm::radians(DEGREES_IN_QUARTER_CIRCLE)) + sin(lat));
    double x = k * cos(lat) * sin(lon);
    double y = k * cos(lat) * cos(lon);
    return glm::vec2(x, y);
}

constexpr glm::vec2 Coordinate::cartesian(double lon, double lat, bool scaleAndTranslate) const
{
    // Convert longitude and latitude to radians
    float lonRad = glm::radians(lon);
    float latRad = glm::radians(lat);

    // Convert to Cartesian coordinates
    float x = glm::cos(latRad) * glm::cos(lonRad);
    float y = glm::cos(latRad) * glm::sin(lonRad);

    if (scaleAndTranslate)
    {
        // Scale and translate the coordinates so they fit in the range [-1, 1]
        x = x / M_PI + 0.5f;
        y = y / (2.0f * M_PI) + 0.5f;
    }
    return glm::vec2(x, y);
}
