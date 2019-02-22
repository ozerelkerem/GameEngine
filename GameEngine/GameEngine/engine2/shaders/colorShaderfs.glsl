#version 330 core

uniform vec3 color;

out vec4 FragColor;

void main()
{
	vec4 test = vec4(color, 1.0);
	FragColor = vec4(color, 1.0);
}