#version 330 core

in vec3 position;
in vec3 color;

out vec3 v_color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	v_color = color;
}