#version 330 core
#define MaxBone 150

layout(location = 0) in vec3 position;
layout(location = 3) in vec4 weights;
layout(location = 4) in vec4 boneids;


uniform int hasBones;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

uniform mat4 bones[MaxBone];



void main()
{
	vec4 pos = vec4(position, 1.0);
	if (hasBones > 0.5f)
	{
		mat4 boneMatrix = bones[int(boneids[0])] * weights[0];
		boneMatrix += (bones[int(boneids[1])] * weights[1]);
		boneMatrix += (bones[int(boneids[2])] * weights[2]);
		boneMatrix += (bones[int(boneids[3])] * weights[3]);
		pos = boneMatrix * pos;
		//tmp = mat3(boneMatrix) * a_Normal;
		pos = vec4(pos.xyz, 1.0);
	}
	else
		pos = modelMatrix * pos;

	gl_Position = projectionMatrix * viewMatrix * pos;


}