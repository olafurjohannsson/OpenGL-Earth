#include "Projection.h"

Projection::Projection(int width, int height)
    : m_width(width), m_height(height)
{
    std::cout << "Projection created (" << width << ", " << height << ")\n";
    m_projectionMatrix = glm::ortho(0.f, static_cast<float>(width), static_cast<float>(height), 0.f, -1.f, 1.f);

    m_sourceProjection = proj_create(PJ_DEFAULT_CTX, "+proj=longlat +datum=WGS84");
    m_mercatorProjection = proj_create(PJ_DEFAULT_CTX, "+proj=merc +datum=WGS84");
    m_stereographicProjection = proj_create(PJ_DEFAULT_CTX, "+proj=stere +datum=WGS84");
    m_orthographicProjection = proj_create(PJ_DEFAULT_CTX, "+proj=ortho +datum=WGS84");
}

Projection::~Projection()
{
    proj_destroy(m_sourceProjection);
    proj_destroy(m_mercatorProjection);
    proj_destroy(m_stereographicProjection);
    proj_destroy(m_orthographicProjection);
}
void Projection::setProjectionType(ProjectionType projectionType)
{
    m_projectionType = projectionType;
}
glm::vec2 Projection::projectLinearTransformation(double longitude, double latitude) const
{
    // Normalize longitude and latitude to the range [-1, 1]
    double normalizedLongitude = longitude / DEGREES_IN_HALF_CIRCLE;
    double normalizedLatitude = latitude / DEGREES_IN_QUARTER_CIRCLE;

    // Map normalized coordinates to the range [0, width] for x and [0, height] for y
    double x = (normalizedLongitude + 1.0) * 0.5 * m_width;
    double y = (normalizedLatitude + 1.0) * 0.5 * m_height;
    return glm::vec2(x, y);
}

glm::vec2 Projection::project(double longitude, double latitude) const
{
    PJ *targetProjection;

    switch (m_projectionType)
    {
    case ProjectionType::Mercator:
    {
        if (latitude < -85.0511 || latitude > 85.0511)
        {
            std::cerr << "Invalid latitude for Mercator projection: " << latitude << "\n";
            std::exit(0);
            return glm::vec2(0, 0);  // or some other appropriate default value
        }
        targetProjection = m_mercatorProjection;
        break;
    }
    case ProjectionType::Stereographic:
        targetProjection = m_stereographicProjection;
        break;
    case ProjectionType::Orthographic:
        targetProjection = m_orthographicProjection;
        break;
    case ProjectionType::LinearTransformation:
    default:
        return projectLinearTransformation(longitude, latitude);
    }

    // Set the source coordinates
    PJ_COORD sourceCoord = proj_coord(longitude, latitude, 0, 0);
    if (proj_errno(m_sourceProjection)) {
        std::cerr << "Error in inverse transformation: " << proj_errno_string(proj_errno(m_sourceProjection)) << "\n";
        std::cout << "Lon: " << longitude << " Lat: " << latitude << "\n";
        std::exit(0);
        return glm::vec2(0, 0);  // or some other appropriate default value
    }

    // Perform the transformation
    PJ_COORD targetCoord = proj_trans(m_sourceProjection, PJ_INV, sourceCoord);
    if (proj_errno(targetProjection)) {
        std::cerr << "Error in forward transformation: " << proj_errno_string(proj_errno(targetProjection)) << "\n";
        std::exit(0);
        return glm::vec2(0, 0);  // or some other appropriate default value
    }
    targetCoord = proj_trans(targetProjection, PJ_FWD, targetCoord);

    // Normalize and map to screen coordinates
    double x = (targetCoord.xy.x + 1.0) * 0.5 * m_width;
    double y = (targetCoord.xy.y + 1.0) * 0.5 * m_height;

    return glm::vec2(x, y);
}

const glm::mat4 &Projection::projectionMatrix() const
{
    return m_projectionMatrix;
}

const float *Projection::projectionMatrixPtr() const
{
    return glm::value_ptr(m_projectionMatrix);
}

int Projection::width() const
{
    return m_width;
}
int Projection::height() const
{
    return m_height;
}