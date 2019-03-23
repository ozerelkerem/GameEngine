#include<editor/ConstantModels.h>

namespace ConstantTextures
{
	Texture * Textures::cameraTexture = new Texture("ConstantIcons\\light.png");
	
	void Textures::initTextures()
	{
		 
		Textures::cameraTexture->loadTexture();
	}
}
