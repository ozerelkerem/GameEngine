#include "Texture.h"

Texture::Texture(std::string path) : Resource(path), Namable(path)
{
	
}

Texture::Texture(std::string n, std::string path) : Namable(n), Resource(path)
{
}


Texture::~Texture()
{}

void Texture::load()
{


	int width, height, nrComponents;
	unsigned char *data = stbi_load(fullpath.c_str(), &width, &height, &nrComponents, 4);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
	
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);


	}
	else
	{
		std::cout << "Texture Could Not Find Path: " << fullpath << std::endl;
	}

	stbi_image_free(data);
}

void Texture::unload()
{
	if(textureID != INVALID_TEXTURE_ID)
		glDeleteTextures(1, &textureID);
}

