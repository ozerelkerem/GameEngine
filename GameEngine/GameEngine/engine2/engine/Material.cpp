#include "Material.h"



Material::Material(std::string name) : ambientColor(1, 0, 0), Namable(name), isTransparent(false)
{

}


Material::~Material()
{
}

void Material::active()
{
	normalShader->Use();
	if (ambientTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ambientTexture->gettextureID());
		
		glUniform1i(glGetUniformLocation(normalShader->programID, "material.ambientTexture"), 0);
	}
	normalShader->setVec3("material.ambientColor", ambientColor);
	normalShader->setInt("material.hasTexture", ambientTexture ? 1 : 0);
	
}
void Material::active(Shader *sha)
{
	sha->Use();
	if (ambientTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ambientTexture->gettextureID());

		glUniform1i(glGetUniformLocation(normalShader->programID, "material.ambientTexture"), 0);
	}
	sha->setVec3("material.ambientColor", ambientColor);
	sha->setInt("material.hasTexture", ambientTexture ? 1 : 0);

}
