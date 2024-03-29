

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
        LinearTransformation,
        Mercator,
        Stereographic,
        Orthographic,
    };
    Projection(int width, int height);

    void setProjectionType(ProjectionType projectionType);

    /**
     * Project the given longitude and latitude to screen coordinates
    */
    std::optional<glm::vec2> project(double longitude, double latitude) const;

    /**
     * Unproject the given screen coordinates to longitude and latitude
    */
    std::optional<glm::vec2> unproject(int x, int y) const;

    glm::mat4 matrix() const;

    const glm::mat4 &projectionMatrix() const;

    const float *projectionMatrixPtr() const;

    void setCenter(int x, int y);

    void setCenterLongitudeLatitude(double lon, double lat);

    int width() const;

    int height() const;
    void onZoom(float zoomChange);
    glm::vec2 linear(double longitude, double latitude) const;

    float calculateZoomFactor();

private:
    std::string m_projectionName; // Name of the projection which is applied, it will default to "LinearTransformation"
    ProjectionType m_projectionType = ProjectionType::LinearTransformation;
    
    int m_screenWidth{0};
    int m_screenHeight{0};

    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    int m_screenCenterX{0};
    int m_screenCenterY{0};


    double m_centerLongitude{0.0};
    double m_centerLatitude{0.0};

    float m_zoomFactor{1.0};
    int m_zoomFactorX{1};
    int m_zoomFactorY{1};
    
};

#endif