#version 430 core
layout (location = 0) in vec2 vertex; // Pass longitude and latitude as a vec2

#define PI 3.1415926535897932384626433832795

uniform mat4 rotationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 scaleMatrix;
uniform vec2 center;
uniform int projectionType;

vec3 rotate(vec3 point, vec2 center) {
    // Convert the center to Cartesian coordinates
    vec3 centerPoint = vec3(
        cos(center.y) * cos(center.x),
        cos(center.y) * sin(center.x),
        sin(center.y)
    );

    // Calculate the rotation axis and angle
    vec3 axis = cross(centerPoint, vec3(0.0, 0.0, 1.0));
    float angle = acos(dot(centerPoint, vec3(0.0, 0.0, 1.0)));

    // Calculate the rotation matrix
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0 - c;
    mat3 rotation = mat3(
        t * axis.x * axis.x + c,          t * axis.x * axis.y - s * axis.z,  t * axis.x * axis.z + s * axis.y,
        t * axis.x * axis.y + s * axis.z,  t * axis.y * axis.y + c,           t * axis.y * axis.z - s * axis.x,
        t * axis.x * axis.z - s * axis.y,  t * axis.y * axis.z + s * axis.x,  t * axis.z * axis.z + c
    );

    // Rotate the point
    return rotation * point;
}

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
vec3 azimuthalEqualArea(vec2 position, vec2 center)
{
    // Subtract the center from the longitude and latitude
    float lon = position.x - center.x;
    float lat = position.y - center.y;

    // Calculate intermediate values
    float sinLat = sin(lat);
    float cosLat = cos(lat);
    float sinLon = sin(lon);
    float cosLon = cos(lon);
    float sinCenterLat = sin(center.y);
    float cosCenterLat = cos(center.y);

    // Calculate the scale factor
    float q = (1 + sinCenterLat * sinLat + cosCenterLat * cosLat * cosLon) / 2;

    // Calculate x and y coordinates
    float x = sqrt(2 / q) * cosLat * sinLon;
    float y = sqrt(2 / q) * (cosCenterLat * sinLat - sinCenterLat * cosLat * cosLon);

    return vec3(x, y, 0.0);
}
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
vec3 orthographic(vec2 position, vec2 center)
{
    // Convert the position to Cartesian coordinates
    vec3 point = vec3(
        cos(position.y) * cos(position.x),
        cos(position.y) * sin(position.x),
        sin(position.y)
    );

    // Rotate the point
    point = vec3(rotationMatrix * vec4(point, 1.0));

    // Apply the orthographic projection
    float x = point.x;
    float y = point.y;
    return vec3(x, y, 0.0);
}

void main()
{
    // Linear Transformation
    if (projectionType == 0) 
    {
        gl_Position = projectionMatrix * vec4(vertex.xy, 1.0, 1.0);
    } 
    else if (projectionType == 1) 
    {
        gl_Position = projectionMatrix * vec4(mercator(vertex.xy, center), 1.0);
    } 
    else if (projectionType == 2) 
    {
        gl_Position = projectionMatrix * vec4(stereographic(vertex.xy, center), 1.0);
    } 
    else if (projectionType == 3) 
    {
        // We need to handle the orthographic matrix also with a rotation matrix, we need to apply a spherical rotation before
        // Applying the projection. The rotation brings the center of the projection to the origin of the coordinate system.
        gl_Position = projectionMatrix * vec4(orthographic(vertex.xy, center), 1.0);   
    } 
    else if (projectionType == 4) 
    {
        gl_Position = projectionMatrix * vec4(azimuthalEqualArea(vertex.xy, center), 1.0);
    } 
    else 
    {
        gl_Position = projectionMatrix * vec4(vertex.xy, 1.0, 1.0);
    }
}