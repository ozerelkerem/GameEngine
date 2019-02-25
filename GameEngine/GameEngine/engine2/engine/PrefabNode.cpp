#include "PrefabNode.h"





PrefabNode::PrefabNode(Object *object, glm::mat4 transformation)
{
	numofChildren = 0;
	children = (PrefabNode**)calloc(numofChildren + 1, sizeof(PrefabNode*));

	this->object = object;
	this->transformation = transformation;
}

PrefabNode::~PrefabNode()
{
}

void PrefabNode::addChild(PrefabNode * node)
{
	children[numofChildren] = node;
	children = (PrefabNode**)realloc(children, (++numofChildren + 1) * sizeof(PrefabNode*));
}
