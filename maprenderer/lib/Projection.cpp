#include "Projection.h"
#include "Transformation.h"

Projection::Projection(int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
    std::cout << "Projection created (" << screenWidth << ", " << screenHeight << ")\n";
    m_projectionMatrix = glm::ortho(
        0.f, 
        static_cast<float>(screenWidth), 
        static_cast<float>(screenHeight), 
        0.f, 
        -1.f, 
        1.f
    );
    // setCenter(screenWidth / 2, screenHeight / 2);
}

void Projection::setProjectionType(ProjectionType projectionType)
{
    m_projectionType = projectionType;
}

void Projection::setCenter(int deltaX, int deltaY)
{
    // Update the screen center coordinates based on the pan delta
    m_screenCenterX += deltaX;
    m_screenCenterY += deltaY;

    // Make sure the screen center coordinates stay within the screen bounds
    m_screenCenterX = std::clamp(m_screenCenterX, 0, m_screenWidth);
    m_screenCenterY = std::clamp(m_screenCenterY, 0, m_screenHeight);
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
void Projection::onZoom(float zoomChange)
{
    const float zoomFactorStep = 1.1f; // Adjust this value as needed

    // Update m_zoomFactor based on the zoomChange
    if (zoomChange > 0) {
        // Zooming in
        m_zoomFactor *= zoomFactorStep;
    } else if (zoomChange < 0) {
        // Zooming out
        m_zoomFactor /= zoomFactorStep;
    }
}
float Projection::calculateZoomFactor()
{
    // Convert the screen center coordinates to geographic coordinates
    double centerX = (static_cast<double>(m_screenCenterX) / m_screenWidth) * 2.0 * glm::pi<double>() - glm::pi<double>();
    double centerY = (static_cast<double>(m_screenCenterY) / m_screenHeight) * glm::pi<double>() - glm::pi<double>() / 2.0;

    // Project the center point
    glm::vec2 center = Transformation::stereographic(glm::vec2(centerX, centerY));

    // Calculate the size of the viewport in projected coordinates
    double viewportWidth = m_screenWidth * m_zoomFactor;
    double viewportHeight = m_screenHeight * m_zoomFactor;

    // TODO: handle other projections

    // Project the points at the edges of the viewport
    glm::vec2 left = Transformation::stereographic(glm::vec2(centerX - m_screenWidth / 2.0, centerY));
    glm::vec2 right = Transformation::stereographic(glm::vec2(centerX + m_screenWidth / 2.0, centerY));

    // Calculate the range of the projected x coordinates
    double xRange = right.x - left.x;
    // Calculate the zoom factor as the ratio of the viewport width to the x range
    m_zoomFactor = m_screenWidth / xRange;

    {
        // range
        std::cout << "range: " << ( 1.0 / m_zoomFactor ) << "\n";

        


    }

    return m_zoomFactor;
}
void Projection::setCenterLongitudeLatitude(double lon, double lat)
{
    m_centerLongitude = lon;
    m_centerLatitude = lat;
    auto centerXY = project(lon, lat);
    if(centerXY)
    {
        m_screenCenterX = centerXY->x;
        m_screenCenterY = centerXY->y;
        std::cout << "center: " << m_screenCenterX << ":" << m_screenCenterY << std::endl;
    }
}
std::optional<glm::vec2> Projection::unproject(int screenX, int screenY) const
{
    if(m_projectionType == ProjectionType::LinearTransformation)
    {
        throw std::runtime_error("Not implemented");
    }
    switch(m_projectionType)
    {
        case ProjectionType::Mercator:
        {
            const auto &unmapped = Transformation::unmap(glm::vec2(screenX, screenY), m_screenWidth, m_screenHeight);
            double mercatorX = unmapped.x * glm::pi<double>();
            double mercatorY = unmapped.y * glm::pi<double>() / 2.0;  // The factor of 2.0 is because the Mercator projection's y-coordinate ranges from -pi to pi
            const auto &inverseMercator = Transformation::inverseMercator(glm::vec2(mercatorX, mercatorY));
            return glm::degrees(inverseMercator);
        }
        case ProjectionType::Stereographic:
        {
            // Normalize screen coordinates to [-1, 1]
            double x = 2.0 * screenX / m_screenWidth - 1.0;
            double y = 1.0 - 2.0 * screenY / m_screenHeight;

            // Scale by zoom factor
            x /= m_zoomFactor;
            y /= m_zoomFactor;

            // Unproject
            double rho = sqrt(x * x + y * y);
            double c = 2 * atan(rho);
            double lat = asin(cos(c) * sin(m_screenCenterY) + y * sin(c) * cos(m_screenCenterY) / rho);
            double lon = m_screenCenterX + atan2(x * sin(c), rho * cos(m_screenCenterY) * cos(c) - y * sin(m_screenCenterY) * sin(c));

            return glm::vec2(lon, lat);
        }
        case ProjectionType::Orthographic:
        {
            throw std::runtime_error("Not implemented");
            // const auto &unmapped = Transformation::unmap(glm::vec2(x, y), m_screenWidth, m_screenHeight);
            // const auto &inverseOrthographic = Transformation::inverseOrthographic(unmapped);
            // return glm::degrees(inverseOrthographic);
        }
    }
    return std::nullopt;
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
    {
        const glm::vec2& mercator = Transformation::mercator(radians);  
        // Normalize x and y to the range [-1, 1]
        double normalizedX = mercator.x / glm::pi<double>();
        double normalizedY = 2.0 * mercator.y / glm::pi<double>();  // The factor of 2.0 is because the Mercator projection's y-coordinate ranges from -pi to pi
        return Transformation::map(glm::vec2(normalizedX, normalizedY), m_screenWidth, m_screenHeight);
    }
    case ProjectionType::Stereographic:
    {
        const glm::vec2& stereographic = Transformation::stereographic(radians);  
        const glm::vec2& normalized = Transformation::normalize(stereographic);
        return Transformation::map(normalized, m_screenWidth, m_screenHeight);
    }
    case ProjectionType::Orthographic:
    {
        const glm::vec2& orthographic = Transformation::orthographic(radians);  
        const glm::vec2& normalized = Transformation::normalize(orthographic);
        return Transformation::map(normalized, m_screenWidth, m_screenHeight);
    }
    }
    return std::nullopt;
}

glm::mat4 Projection::matrix() const
{
    // // Create a scale matrix for zooming
    // glm::mat4 projection = glm::ortho(0.f, static_cast<float>(m_screenWidth), 0.f, static_cast<float>(m_screenHeight));
    // glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));

    // // Adjust the translation to account for the new zoom factor
    // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_screenCenterX / m_zoomFactor, -m_screenCenterY / m_zoomFactor, 0.f));

    // return projection * scaleMatrix * translationMatrix;
    
    glm::mat4 projection = glm::ortho(0.f, static_cast<float>(m_screenWidth), 0.f, static_cast<float>(m_screenHeight));
    
    // glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-590.f, -550.f, 0.f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-m_screenCenterX, -m_screenCenterY, 0.f));
    float scaleValue = std::log(std::abs(m_zoomFactor));
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoomFactor, m_zoomFactor, 1.0f));
    
    std::cout << "return projection center: " << m_screenCenterX << ":" << m_screenCenterY << std::endl;
    std::cout << "zoomFactor: " << m_zoomFactor << std::endl;
    
    std::cout << "scaleValue: " << scaleValue << std::endl;
    return projection * scaleMatrix * translationMatrix;
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
    return m_screenWidth;
}
int Projection::height() const
{
    return m_screenHeight;
}