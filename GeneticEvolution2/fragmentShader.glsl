#version 410 core
in vec3 vertexPos;
out vec4 color;
void main()
{
    vec2 disp = vertexPos.xy - gl_FragCoord.xy;
    color = vec4(1,0,0,1);//(dot(disp, disp) < 100 )?  : vec4(0,0,0,1);
}