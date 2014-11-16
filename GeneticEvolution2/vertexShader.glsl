#version 410 core
in vec3 vertex;
uniform mat4 transformMatrix;
out vec3 vertexPos;
void main()
{
    gl_Position=transformMatrix * vec4(vertex,1);
    vertexPos = gl_Position.xyz;
}