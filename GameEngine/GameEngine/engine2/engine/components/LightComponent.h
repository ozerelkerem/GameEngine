#pragma once

#include <glm/glm.hpp>

#include <engine/components/Component.h>
#include <engine/Transform.h>
#include <engine/Shader.h>

enum LightType { Point, Directional, Spotlight };

static const char * LightTypeNames[] = {"Point", "Directional" , "Spotlight"};

class LightComponent : public Component <LightComponent>
{
private:
	float constant;
	float linear;
	float quadratic;
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

	void calculateAttenuation();
	void passShader(Shader *s, Transform *, int);

	
	inline virtual IComponent * getnew(ActorID own, ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		auto x = ((IComponent*) new LightComponent(*this));
		x->owner = own;
		return x;
	}
};

