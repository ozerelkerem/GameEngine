#pragma once

#include <Api.h>
#include <engine/resource/Resource.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <stb_image.h>
#include <engine/Namable.h>
#include<engine/resource/ResourceManager.h>


class Texture : public Resource<Texture> , public Namable
{
public:
	Texture(std::string path);
	Texture(std::string n, std::string path);
	~Texture();
	
	static const TextureID INVALID_TEXTURE_ID = 65535;//std::numeric_limits<TextureID>::max();
	virtual void load() override;
	virtual void unload() override;

	inline unsigned int gettextureID();

	void operator delete(void *a) { };

private:
	unsigned int textureID = INVALID_TEXTURE_ID;

};

unsigned int Texture::gettextureID()
{
	if (textureID == INVALID_TEXTURE_ID)
	{ return GE_Engine->resourceManager->getResource<Texture>("ConstantIcons\\notexture.png",true)->textureID; }
	else
	{ return textureID; } 
};

