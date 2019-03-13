#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<string>
#include <stb_image.h>

#include<limits>
#include <engine/Namable.h>
using TextureID = uint16_t;

class Texture : public Namable
{
public:
	Texture(std::string path);
	Texture(std::string n, std::string path);
	~Texture();

	static const TextureID INVALID_TEXTURE_ID = std::numeric_limits<TextureID>::max();


	unsigned int textureID;
	std::string m_path;


	void loadTexture();
	
};

