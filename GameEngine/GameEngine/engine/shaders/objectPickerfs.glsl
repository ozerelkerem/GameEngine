#version 330 core


uniform float color;
out vec4 FragColor;

void main()
{
	FragColor = vec4(color/255, 0, 1.0, 1.0);
}