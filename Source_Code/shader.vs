#version 330

layout (location = 0) in vec3 Position;

//uniform float g_Scale;

uniform mat4 g_MVP; 

out vec4 g_Color; 

void main(){
    gl_Position = g_MVP * vec4(Position, 1.0);
	g_Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
}