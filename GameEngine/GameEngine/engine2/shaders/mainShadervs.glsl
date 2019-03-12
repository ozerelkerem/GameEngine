#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec3 v_normal;
out vec3 v_position;
out vec2 v_textureCoords;

void main()
{
	v_textureCoords = textureCoords;
	vec4 tmp = modelMatrix * vec4(position, 1);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);

	v_normal = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
	v_position = vec3(tmp);
	
}