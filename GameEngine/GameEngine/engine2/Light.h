#pragma once

#include <glm/glm.hpp>

#include <Object.h>

enum LightType { Undefined, Directional, Point, Spotlight };

class Light : public Object
{
public:
	Light(LightType, glm::vec3, glm::vec3, glm::vec3, std::string);
	~Light();

	LightType type;

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
};

