#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<Api.h>
#include<engine/Texture.h>
#include<engine/Shader.h>

class SpriteRenderer
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void DrawSprite(Texture *texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f), bool centered=true);
	
private:

	unsigned int vao;
	
};

