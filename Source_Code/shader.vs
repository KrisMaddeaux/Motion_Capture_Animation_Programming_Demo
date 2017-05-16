#version 330

layout (location = 0) in vec3 Position;

//uniform float g_Scale;

uniform mat4 g_World; 
uniform mat4 g_Perspective; 

void main()
{
    gl_Position = g_Perspective * g_World * vec4(Position, 1.0);
}