#include "LightComponent.h"



LightComponent::LightComponent(LightType type, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular) : Component()
{
	this->type = type;
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->specular = specular;
}


LightComponent::~LightComponent()
{
}
