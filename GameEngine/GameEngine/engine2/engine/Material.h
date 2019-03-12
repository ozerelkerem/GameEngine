#pragma once
#include <engine/Texture.h>

#include <glm/glm.hpp>

#include <string>

#include <Api.h>

class Material
{
public:
	Material(std::string);
	~Material();

	std::string name;

	glm::vec3 ambientColor;
	Texture *ambientTexture;
	//Texture *normalTexture;

	void active();

	inline static void noMaterial() { 
		normalShader->setInt("material.hasTexture", 0);
		normalShader->setVec3("material.ambientColor", glm::vec3(1,0,1));
	}
};

