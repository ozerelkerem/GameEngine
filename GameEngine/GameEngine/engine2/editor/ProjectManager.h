#pragma once

#include <direct.h>

#include <engine/Model.h>
#include <engine/Material.h>
#include <engine/Scene.h>
#include <engine/Prefab.h>

class Model;
class Prefab;
class Scene;

class ProjectManager
{
public:
	ProjectManager(std::string, std::string);
	~ProjectManager();

	std::string name;
	std::string path;

	std::vector<Model *> models;
	
	std::vector<Scene *> scenes;
	std::vector<Prefab *> prefabs;

	std::vector<Texture *> textures;
	std::vector<Material *> materials;

	std::vector<std::vector<TextureID>> materialTextureMap;

	template <class T>
	void add(T *piece);

	inline int findIndexofTexture(Texture *t) {
		if (!t)
			return Texture::INVALID_TEXTURE_ID;
		int i = 0;
		while (i < textures.size() && textures[i++] == t);
		if (i > textures.size())
			return Texture::INVALID_TEXTURE_ID;
		return --i;
	}
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


