

// Load the data
// Project to mercator

#ifndef PROJECTION_H
#define PROJECTION_H

#include <iostream>
#include <string>
#include <proj.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <optional>

class Projection
{
public:
    static constexpr double DEGREES_IN_HALF_CIRCLE = 180.0;
    static constexpr double DEGREES_IN_QUARTER_CIRCLE = 90.0;

    enum class ProjectionType
    {
        LinearTransformation = 0,
        Mercator = 1,
        Stereographic = 2,
        Orthographic = 3,
        AzimuthalEqualArea = 4
    };
    Projection(ProjectionType projectionType);

    void setScreenSize(int width, int height);


    /**
     * Project the given longitude and latitude to screen coordinates
    */
    std::optional<glm::vec2> project(double longitude, double latitude) const;

    /**
     * Unproject the given screen coordinates(which are possibly projected) to longitude and latitude
    */
    std::optional<glm::vec2> unproject(double x, double y) const;

    glm::mat4 projectionMatrix() const;

    glm::mat4 scaleMatrix() const;

    glm::mat4 rotationMatrix() const;

    ProjectionType projectionType() const;


    int width() const;

    int height() const;
    
    void zoom(float yAngle, const glm::vec2 &screenCoordinates);

    glm::vec2 linear(double longitude, double latitude) const;
    
    void updateTransformation(float xAngle, float yAngle);


    void setOffset(int x, int y);
    void setCenter(double lon, double lat);
    glm::vec2 getCenter();

private:

    /**
     * When the center is changed, the rotation matrix should be recomputed
     * We calculate the rotation axis and angle needed to rotate a point on a sphere to the origin
    */
    void calculateRotationToOrigin();

    ProjectionType m_projectionType = ProjectionType::LinearTransformation;
    
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