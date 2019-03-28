#pragma once
#include <engine/Texture.h>
#include <limits>
#include <glm/glm.hpp>

#include <string>

#include <Api.h>
#include <engine/Namable.h>

#include <engine/Shader.h>

using MaterialID = uint16_t;


class Material : public Namable
{
public:
	Material(std::string);
	~Material();

	static const MaterialID INVALID_MATERIAL_ID = std::numeric_limits<MaterialID>::max();



	glm::vec3 ambientColor;
	Texture *ambientTexture;
	bool isTransparent;
	//Texture *normalTexture;

	void active();

};

