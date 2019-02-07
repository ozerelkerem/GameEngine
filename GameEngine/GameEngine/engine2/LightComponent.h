#pragma once
#include <glm/glm.hpp>
#include <Component.h>


enum LightType { Undefined, Directional, Point, Spotlight };

class LightComponent :
	public Component
{
public:

	LightComponent(LightType, glm::vec3, glm::vec3, glm::vec3);
	~LightComponent();

	LightType type;

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;

	ComponentType getType() { return ComponentType::Light; }
	Component * copy() { return new LightComponent(*this); }
};

