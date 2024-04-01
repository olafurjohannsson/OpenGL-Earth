#ifndef PROJECTION_H
#define PROJECTION_H

#include <iostream>
#include <string>
#include <proj.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

// Class to handle different types of map projections
class Projection
{
public:
    // Constants for degree calculations
    static constexpr double DEGREES_IN_HALF_CIRCLE = 180.0;
    static constexpr double DEGREES_IN_QUARTER_CIRCLE = 90.0;

    // Enum for different types of projections
    enum class ProjectionType
    {
        LinearTransformation = 0,
        Mercator = 1,
        Stereographic = 2,
        Orthographic = 3
    };
    
    // Constructor that sets the projection type
    Projection(ProjectionType projectionType);

    // Set the screen size for the projection
    void setScreenSize(int width, int height);

    // Getters for the projection, scale, and rotation matrices
    glm::mat4 projectionMatrix() const;
    glm::mat4 scaleMatrix() const;
    glm::mat4 rotationMatrix() const;

    // Get and set the projection type
    ProjectionType projectionType() const;
    void setProjectionType(ProjectionType projectionType);

    // Getters for the screen width and height
    int width() const;
    int height() const;
    
    // Zoom function
    void zoom(float yAngle, const glm::vec2 &screenCoordinates);
    
    // Set the offset and center for the projection
    void setOffset(int x, int y);
    void setCenter(double lon, double lat);

    // Get the center of the projection
    glm::vec2 getCenter();

private:
    // Calculate the rotation to the origin when the center is changed
    void calculateRotationToOrigin();

    // Member variables for the projection type, matrices, screen size, zoom factor, center, and offset
    ProjectionType m_projectionType = ProjectionType::Orthographic;
    glm::mat4 m_scaleMatrix = glm::mat4(1.f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    glm::mat4 m_rotationMatrix = glm::mat4(1.f);
    int m_screenWidth{0};
    int m_screenHeight{0};
    float m_zoomFactor{1.0};
    double m_centerLongitude{0.0};
    double m_centerLatitude{0.0};
    int m_centerX{0};
    int m_centerY{0};
};

#endif