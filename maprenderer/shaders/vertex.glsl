#version 430 core
layout (location = 0) in vec2 aPos; // Pass longitude and latitude as a vec2

uniform mat4 matrix;
uniform mat4 projectionMatrix;

void main()
{
    // Pass the vertex coordinates directly to

    gl_Position = matrix * vec4(aPos, 0.0, 1.0);
    //gl_Position = vec4(aPos, 0.0, 1.0);
}