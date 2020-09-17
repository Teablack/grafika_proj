#version 330 core

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec4 lp; //pozycja dla swiatla punktowego
uniform vec3 vp;

out vec2 i_tc;
out vec3 n;
out vec4 FragPos;
out vec4 lightPos;
out vec3 viewPos;

void main()
{
    viewPos = vp;
    lightPos = lp;
    n = normal;
    i_tc= texCoord;
    FragPos = M* vertex;
    gl_Position = P*V*M *vertex;
} 
