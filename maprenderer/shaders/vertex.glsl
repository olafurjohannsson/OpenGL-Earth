#version 430 core
layout (location = 0) in vec2 vertex; // Pass longitude and latitude as radians in a vec2 structure

#define PI 3.1415926535897932384626433832795


/**
    
    Vertex Shader

    This shaderfile computes map projections
    All vertex input is in radians, and the origin/center is also in radians
    The reason that the computation is done in the shader is because these mathematical projections
    are not linear, therefore when the origin changes, all vertex coordinates change relatie
    to the origin, so if we would do this in C++ we would have to re-compute everything when we pan/zoom,
    but doing it in the shader is much more computationally efficient.


*/


//
/// Rotation matrix handles creating an axis and angle around an origin point, used for Orthographic projection
uniform mat4 rotationMatrix;

//
/// Projection matrix converts radians to clip-space, so it handles converting [-2PI,2PI] to [-1,1]
uniform mat4 projectionMatrix;

//
/// Scale matrix handles zooming by applying a scaling matrix on our projected points
uniform mat4 scaleMatrix;

//
/// Center, or the origin point is the point which all projected points are relative to and rendered
uniform vec2 center;

//
/// Which projection to render, so we can change at runtime
uniform int projectionType;

/**
    Apply Stereographic map projection on position relative to origin
*/
vec3 stereographic(vec2 position, vec2 center)
{
    // Convert the center longitude and latitude to radians 
    float centerLonRad = center.x;
    float centerLatRad = center.y;

    float lon = position.x;
    float lat = position.y;

    // Calculate x, y, and z coordinates for stereographic projection
    float k = 2.0 / (1.0 + sin(centerLatRad) * sin(lat) + cos(centerLatRad) * cos(lat) * cos(lon - centerLonRad));
    float x = k * cos(lat) * sin(lon - centerLonRad);
    float y = k * (cos(centerLatRad) * sin(lat) - sin(centerLatRad) * cos(lat) * cos(lon - centerLonRad));
    float z = 0.0;
    return vec3(x, y, z);
}

/**
    Apply Mercator map projection on position relative to origin
*/
vec3 mercator(vec2 position, vec2 center)
{
    // Subtract the center from the longitude and latitude
    float lon = position.x - center.x;
    float lat = clamp(position.y - center.y, -PI/2.0, PI/2.0);

    // Calculate x and y coordinates
    float x = lon;
    float y = log(tan(0.25 * PI + 0.5 * lat));
    return vec3(x, y, 0.0);
}

/**
    Apply Orthographic map projection on position relative to origin, with a rotation matrix
*/
vec3 orthographic(vec2 position, vec2 center)
{
    // Convert the position to Cartesian coordinates
    vec3 point = vec3(
        cos(position.y) * cos(position.x),
        cos(position.y) * sin(position.x),
        sin(position.y)
    );

    // We need to handle the orthographic matrix also with a rotation matrix, we need to apply a spherical rotation before
    // Applying the projection. The rotation brings the center of the projection to the origin of the coordinate system.
    point = vec3(rotationMatrix * vec4(point, 1.0));

    // Apply the orthographic projection
    float x = point.x;
    float y = point.y;
    return vec3(x, y, 0.0);
}

void main()
{
    // Projection matrix handles converting the vertex to clip-space
    // Scale matrix handles zooming
    if (projectionType == 1) 
    {
        gl_Position = projectionMatrix * scaleMatrix * vec4(mercator(vertex.xy, center), 1.0);
    } 
    else if (projectionType == 2) 
    {
        gl_Position = projectionMatrix * scaleMatrix * vec4(stereographic(vertex.xy, center), 1.0);
    } 
    else if (projectionType == 3) 
    {
        gl_Position = projectionMatrix * scaleMatrix * vec4(orthographic(vertex.xy, center), 1.0);   
    } 
    else 
    {
        // Linear Transformation
        gl_Position = projectionMatrix * scaleMatrix * vec4(vertex.xy, 1.0, 1.0);
    }
}