#version 330 core

layout(location = 0) in vec3 position;


out vec3 v_color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	v_color = vec3(1.0,0,0);
}