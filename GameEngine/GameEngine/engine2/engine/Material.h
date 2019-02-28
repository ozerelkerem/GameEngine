#pragma once

#include <string>

class Material
{
public:
	Material(std::string);
	~Material();

	std::string name;

	glm::vec3 ambientColor;

	
};

