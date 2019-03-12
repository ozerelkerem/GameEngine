#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<string>
#include <stb_image.h>
class Texture
{
public:
	Texture(std::string path);
	~Texture();


	unsigned int textureID;
	std::string m_path;


	void loadTexture();
	
};

