#version 330 core
in vec3 vertexPos;
in vec3 fragColor;
out vec4 color;
void main()
{
    vec2 disp = vertexPos.xy - gl_FragCoord.xy;
    color = vec4(fragColor,1);//(dot(disp, disp) < 100 )?  : vec4(0,0,0,1);
}