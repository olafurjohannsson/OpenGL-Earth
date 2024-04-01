#include "Projection.h"
#include "Transformation.h"
#include <glm/gtc/matrix_transform.hpp>
Projection::Projection(ProjectionType projectionType) :  m_projectionType(projectionType)
{
    
}
void Projection::setScreenSize(int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    m_centerX = screenWidth / 2;
    m_centerY = screenHeight / 2;
    float aspectRatio = static_cast<float>(m_screenWidth) / m_screenHeight;
    m_projectionMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.f, 1.f);
}

glm::vec2 Projection::linear(double longitude, double latitude) const
{
    // Normalize longitude and latitude to the range [-1, 1]
    double normalizedLongitude = longitude / DEGREES_IN_HALF_CIRCLE;
    double normalizedLatitude = latitude / DEGREES_IN_QUARTER_CIRCLE;

    // Map normalized coordinates to the range [0, width] for x and [0, height] for y
    double x = (normalizedLongitude + 1.0) * 0.5 * m_screenWidth;
    double y = (normalizedLatitude + 1.0) * 0.5 * m_screenHeight;
    return glm::vec2(x, y);
}



std::optional<glm::vec2> Projection::unproject(double x, double y) const
{
    if(m_projectionType == ProjectionType::LinearTransformation)
    {
        throw std::runtime_error("Not implemented");
    }
    switch(m_projectionType)
    {
        case ProjectionType::Mercator:
        {

        }
        case ProjectionType::Stereographic:
        {

        }
        case ProjectionType::Orthographic:
        {

        }
    }
    return std::nullopt;
}
void Projection::setCenter(double lon, double lat)
{
    m_centerLongitude = lon;
    m_centerLatitude = lat;

    calculateRotationToOrigin();
}
void Projection::calculateRotationToOrigin()
{
    float lon = m_centerLongitude;
    float lat = m_centerLatitude;

    // Convert the center to Cartesian coordinates
    glm::vec3 centerPoint = glm::vec3(
        cos(lat) * cos(lon),
        cos(lat) * sin(lon),
        sin(lat)
    );

    // Calculate the rotation axis and angle
    glm::vec3 axis = glm::cross(centerPoint, glm::vec3(0.0f, 0.0f, 1.0f));
    float angle = glm::acos(glm::dot(centerPoint, glm::vec3(0.0f, 0.0f, 1.0f)));

    // Create the rotation matrix
    m_rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}
void Projection::setOffset(int dx, int dy)
{
    float scaleFactor = 1.f;
    // Convert delta from screen coordinates to degrees
    double dLon = -dx * DEGREES_IN_HALF_CIRCLE / m_screenWidth;
    double dLat = dy * DEGREES_IN_QUARTER_CIRCLE / m_screenHeight;

    dLon *= scaleFactor;
    dLat *= scaleFactor;

    // Adjust the center of the projection
    m_centerLongitude += dLon;
    m_centerLatitude += dLat;

    calculateRotationToOrigin();
}
Projection::ProjectionType Projection::projectionType() const
{
    return m_projectionType;
}
glm::mat4 Projection::rotationMatrix() const
{
    return m_rotationMatrix;

}
void Projection::zoom(float yAngle, const glm::vec2 &screenCoordinates)
{
    auto factor = glm::tan(yAngle / 2.0);
    m_scaleMatrix = glm::scale(m_scaleMatrix, glm::vec3(factor, factor, 1.0f));
}
std::optional<glm::vec2> Projection::project(double longitude, double latitude) const
{
    // Linear transformation is just a straight conversion from
    // Lon/Lat to x/y cartesian coordinates on the screen
    if(m_projectionType == ProjectionType::LinearTransformation)
    {
        return linear(longitude, latitude);
    }

    // Convert longitude and latitude from degrees to radians
    double lonRad = glm::radians(longitude);
    double latRad = glm::radians(latitude);
    const glm::vec2 radians(lonRad, latRad);

    switch (m_projectionType)
    {
    case ProjectionType::Mercator:
    case ProjectionType::Stereographic:
    case ProjectionType::Orthographic:
        return radians;
    }
    return std::nullopt;
}

glm::mat4 Projection::scaleMatrix() const
{
    return m_scaleMatrix;
}
glm::mat4 Projection::projectionMatrix() const
{
    return m_projectionMatrix;
}
glm::vec2 Projection::getCenter()
{
    return glm::vec2(m_centerLongitude, m_centerLatitude);
}

int Projection::width() const
{
    return m_screenWidth;
}
int Projection::height() const
{
    return m_screenHeight;
}