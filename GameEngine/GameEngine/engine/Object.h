#pragma once

#include <string>

#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtc/quaternion.hpp"

#include "Scene.h"

class Scene;

class Transform
{
public:
	glm::vec3 position;
	glm::quat qRotation;
	glm::vec3 eRotation;
	glm::vec3 scale;

	glm::mat4 localMatrix;
};

class Object
{
private:
	bool AddChild(Object *newChild);
	bool RemoveChild(Object *removeChild);

	bool isParent(Object *root);

public:
	std::string name;

	Transform *transform;

	Object *parent;
	int numOfChilds;
	Object *(*childs);

	Scene *scene;

	bool AddParent(Object *newParent);
	bool RemoveParent();
	void RemoveObject();

	Object(std::string name, Scene *scene);
	~Object();
};

