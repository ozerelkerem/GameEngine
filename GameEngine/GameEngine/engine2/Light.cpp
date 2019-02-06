#include "Light.h"


Light::Light(LightType type, glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular, std::string name) : Object(name)
{
	this->type = type;
	this->diffuse = diffuse;
	this->ambient = ambient;
	this->specular = specular;
}

Light::~Light()
{
}
