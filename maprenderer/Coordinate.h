#ifndef COORDINATE_H
#define COORDINATE_H
#include <QJsonValue>
#include <QJsonArray>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

class Coordinate
{

public:
    static constexpr double DEGREES_IN_HALF_CIRCLE = 180.0;
    static constexpr double DEGREES_IN_QUARTER_CIRCLE = 90.0;

    Coordinate(double longitude, double latitude);

    static Coordinate fromJson(const QJsonValue &json);

    glm::vec2 lonLat() const;

    /**
     * Perform a linear transformation and map an equirectangular projection by normalizing
     *  the longitude and latitude to the range [-1, 1] and mapping them to 2D coordinates.
     */
    glm::vec2 normalizeAndMapTo2D(int width, int height) const;

    constexpr glm::vec2 cartesian(double lon, double lat, bool scaleAndTranslate) const;

    glm::vec2 stereographicProjection() const;

    glm::vec2 stereographicProjection(double minX, double maxX, double minY, double maxY, double width, double height) const;

    glm::vec2 orthographicProjection() const
    {
        double lon = glm::radians(m_longitude);
        double lat = glm::radians(m_latitude);
        double x = cos(lat) * sin(lon);
        double y = cos(lat) * cos(lon);
        return glm::vec2(x, y);
    }

    glm::vec2 mercatorProjection() const
    {
        double lon = glm::radians(m_longitude);
        double lat = glm::radians(m_latitude);
        double x = lon;

        double y = log(tan(glm::pi<double>() / 4 + lat / 2));
        return glm::vec2(x, y);
    }

private:
    double m_longitude{0.0};
    double m_latitude{0.0};
};
#endif