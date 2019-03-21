#version 330 core
#define MaxBone 120

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec4 weights;
layout(location = 4) in uvec4 boneids;


uniform int hasBones;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

uniform mat4 bones[MaxBone];

out vec3 v_normal;
out vec3 v_position;
out vec2 v_textureCoords;

void main()
{
	vec4 pos = vec4(position, 1.0);
	if (hasBones > 0.5f)
	{
		mat4 boneMatrix = bones[boneids[0]] * weights[0];
		boneMatrix += (bones[boneids[1]] * weights[1]);
		boneMatrix += (bones[boneids[2]] * weights[2]);
		boneMatrix += (bones[boneids[3]] * weights[3]);
		pos = boneMatrix * pos;
		//tmp = mat3(boneMatrix) * a_Normal;
	}

	v_textureCoords = textureCoords;
	vec4 tmp = modelMatrix * vec4(position, 1);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;

	v_normal = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
	v_position = vec3(tmp);
	
}