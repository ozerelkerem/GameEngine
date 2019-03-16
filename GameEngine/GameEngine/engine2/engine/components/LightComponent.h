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

	
	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new LightComponent(*this));
	}
};

