#pragma once

#include <glm/glm.hpp>

#include <engine/components/Component.h>
#include <engine/Transform.h>
#include <engine/Shader.h>

enum LightType { Point, Directional, Spotlight };

static const char * LightTypeNames[] = {"Point", "Directional" , "Spotlight"};

class LightComponent : public Component <LightComponent>
{

public:

	LightComponent();
	LightComponent(LightType a);
	~LightComponent();

	LightType lightType;

	glm::vec3 color;
	float angle;
	
	//attenuation
	float distance;
	float intensity;
	float constant;
	float linear;
	float quadratic;

	void calculateAttenuation();
	void passShader(Shader *s, Transform *, int);

	
	
};

