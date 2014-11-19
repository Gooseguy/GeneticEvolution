#version 410 core
layout(location=0) in vec3 vertex;
layout(location=1) in vec3 vertexColor;
uniform mat4 transformMatrix;
out vec3 vertexPos;
out vec3 fragColor;
void main()
{
    gl_Position=transformMatrix * vec4(vertex,1);
    vertexPos = gl_Position.xyz;
    fragColor=vertexColor;
}