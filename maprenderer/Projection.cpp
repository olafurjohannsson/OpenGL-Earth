#include "Projection.h"
#include <glm/gtc/matrix_transform.hpp>

Projection::Projection(ProjectionType projectionType) : m_projectionType(projectionType)
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

void Projection::setCenter(double lon, double lat)
{
    m_centerLongitude = lon;
    m_centerLatitude = lat;

    calculateRotationToOrigin();
}
void Projection::calculateRotationToOrigin()
{
    // float lon = m_centerLongitude;
    // float lat = m_centerLatitude;

    // // Convert the center to Cartesian coordinates
    // glm::vec3 centerPoint = glm::vec3(
    //     cos(lat) * cos(lon),
    //     cos(lat) * sin(lon),
    //     sin(lat)
    // );

    // // Calculate the rotation axis and angle
    // glm::vec3 axis = glm::cross(centerPoint, glm::vec3(0.0f, 0.0f, 1.0f));
    // float angle = glm::acos(glm::dot(centerPoint, glm::vec3(0.0f, 0.0f, 1.0f)));

    // // Create the rotation matrix
    // m_rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);

    float lon = m_centerLongitude;
    float lat = m_centerLatitude;

    // Calculate the rotation around the y-axis
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(lon), glm::vec3(0.0f, 1.0f, 0.0f));

    // Calculate the rotation around the x-axis
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(-lat), glm::vec3(1.0f, 0.0f, 0.0f));

    // Combine the rotations
    m_rotationMatrix = rotationY * rotationX;
}
void Projection::setOffset(int dx, int dy)
{
    float scaleFactor = 1.f;
    // Convert delta from screen coordinates to degrees
    if (dx != 0)
    {
        double dLon = -dx * DEGREES_IN_HALF_CIRCLE / m_screenWidth;
        dLon *= scaleFactor;
        m_centerLongitude += dLon;
    }

    if (dy != 0)
    {
        double dLat = dy * DEGREES_IN_QUARTER_CIRCLE / m_screenHeight;
        dLat *= scaleFactor;
        m_centerLatitude -= dLat;
    }

    calculateRotationToOrigin();
}
Projection::ProjectionType Projection::projectionType() const
{
    return m_projectionType;
}
void Projection::setProjectionType(ProjectionType projectionType)
{
    m_projectionType = projectionType;
}
glm::mat4 Projection::rotationMatrix() const
{
    return m_rotationMatrix;
}
float Projection::zoomFactor() const
{
    return m_zoomFactor;
}
void Projection::zoom(float yAngle, const glm::vec2 &screenCoordinates)
{
    // Calculate the zoom factor based on the yAngle
    if (yAngle > 0)
    { // zooming in
        m_zoomFactor *= 1.1f;
    }
    else if (yAngle < 0)
    { // zooming out
        m_zoomFactor /= 1.1f;
    }

    // Clamp the zoom factor to a reasonable range
    m_zoomFactor = std::max(0.1f, std::min(m_zoomFactor, 10.0f));

    // Scale the matrix by the zoom factor
    m_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
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