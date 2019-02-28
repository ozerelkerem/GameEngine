#pragma once

#include <glm/glm.hpp>

#include <engine/Component.h>
#include <engine/Transform.h>
#include <engine/Shader.h>

enum LightType { Point, Directional, Spotlight };

static const char * LightTypeNames[] = {"Point", "Directional" , "Spotlight"};

class LightComponent :
	public Component
{
public:

	LightComponent();
	~LightComponent();

	LightType lightType;

	glm::vec3 color;
	float angle;
	
	//attenuation
	float constant;
	float linear;
	float quadratic;
	float distance;
	float intensity;

	void passShader(Shader *s, Transform *, int);

	ComponentType getType() { return ComponentType::Light; }
	Component * copy() { return new LightComponent(*this); }
};

