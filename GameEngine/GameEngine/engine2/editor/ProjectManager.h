#pragma once

#include <engine/Model.h>
#include <engine/Material.h>
#include <engine/Scene.h>
#include <engine/Prefab.h>

class ProjectManager
{
public:
	ProjectManager();
	~ProjectManager();

	std::list<Model *> models;
	
	std::list<Scene *> scenes;
	std::list<Prefab *> prefabs;

	std::vector<Texture *> textures;
	std::vector<Material *> materials;

	std::vector<std::vector<TextureID>> materialTextureMap;

	template <class T>
	void add(T *piece);

	
	

};

template<class T>
inline void ProjectManager::add(T *piece)
{
	
	if (typeid(T) == typeid(Scene))
		scenes.push_back((Scene *)piece);
	else if (typeid(T) == typeid(Model))
		models.push_back((Model *) piece);
	else if (typeid(T) == typeid(Material))
		materials.push_back((Material *)piece);
	else if (typeid(T) == typeid(Prefab))
		prefabs.push_back((Prefab *)piece);
	else if (typeid(T) == typeid(Texture))
		textures.push_back((Texture *)piece);
};


