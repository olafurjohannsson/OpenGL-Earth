

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

    ~Projection();

    void setProjectionType(ProjectionType projectionType);

    glm::vec2 project(double longitude, double latitude) const;

    const glm::mat4 &projectionMatrix() const;

    const float *projectionMatrixPtr() const;

    int width() const;

    int height() const;

    glm::vec2 projectLinearTransformation(double longitude, double latitude) const;

private:
    std::string m_projectionName; // Name of the projection which is applied, it will default to "LinearTransformation"
    ProjectionType m_projectionType = ProjectionType::LinearTransformation;
    int m_width{0};
    int m_height{0};
    glm::mat4 m_projectionMatrix = glm::mat4(1.f);
    int m_centerX{0};
    int m_centerY{0};
    double m_centerLongitude{0.0};
    double m_centerLatitude{0.0};
    PJ *m_sourceProjection;
    PJ *m_mercatorProjection;
    PJ *m_stereographicProjection;
    PJ *m_orthographicProjection;
};

#endif