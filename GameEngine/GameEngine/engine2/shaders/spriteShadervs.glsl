#version 330 core
layout(location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{

	TexCoords = vertex.zw;
	gl_Position = projectionMatrix * modelMatrix * vec4(vertex.xy, 0.0, 1.0);
}