#pragma once

#include <engine/Object.h>

#include <string>
#include <glm/glm.hpp>

class PrefabNode
{
public:
	PrefabNode(Object *, glm::mat4);
	~PrefabNode();

	

	Object * object;
	glm::mat4 transformation;
	unsigned int numofChildren;
	PrefabNode **children;

	void addChild(PrefabNode *node);

};

