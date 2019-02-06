#pragma once

#include <string>

#include "Scene.h"
#include "Shader.h"
#include "Transform.h"

class Scene;

class Object
{
private:
	bool AddChild(Object *newChild);
	bool RemoveChild(Object *removeChild);

	bool isParent(Object *root);

public:
	std::string name;
	int id;

	Transform *transform;

	Object *parent;
	int numOfChilds;
	Object *(*childs);

	Scene *scene;

	bool AddParent(Object *newParent);
	bool RemoveParent();
	void RemoveObject();

	void Render(Shader *) { return; };

	Object(std::string name, Scene *scene);
	~Object();
};

