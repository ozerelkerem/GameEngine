#pragma once

#include <direct.h>

#include <engine/Model.h>
#include <engine/Material.h>
#include <engine/Scene.h>
#include <engine/Prefab.h>
#include <engine/scripting/Script.h>
class Animation;
class Model;
class Prefab;
class Scene;

class ProjectManager
{
public:
	ProjectManager(std::string name, std::string path);
	~ProjectManager();

	std::string name;
	std::string path;

	std::vector<Model *> models;
	std::vector<Scene *> scenes;
	std::vector<Prefab *> prefabs;
	std::vector<Animation *> animations;
	std::vector<Texture *> textures;
	std::vector<Material *> materials;
	std::vector<Script *> scripts;

	std::vector<std::vector<TextureID>> materialTextureMap;

	template <class T>
	bool isExists(T *piece);


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
inline bool ProjectManager::isExists(T * piece)
{
	return true;
}

template <> inline bool ProjectManager::isExists<Script>(Script * piece) { return std::find_if(scripts.begin(), scripts.end(), [piece](Script* script) {return script->name == piece->name; }) != scripts.end(); };

template<class T>
inline void ProjectManager::add(T *piece)
{
};
template<> inline void ProjectManager::add<Scene>(Scene *piece) { scenes.push_back(piece); };
template<> inline void ProjectManager::add<Model>(Model *piece) { models.push_back(piece); };
template<> inline void ProjectManager::add<Material>(Material *piece) { materials.push_back(piece); };
template<> inline void ProjectManager::add<Prefab>(Prefab *piece) { prefabs.push_back(piece); };
template<> inline void ProjectManager::add<Texture>(Texture *piece) { textures.push_back(piece); };
template<> inline void ProjectManager::add<Animation>(Animation *piece) { animations.push_back(piece); };
template<> inline void ProjectManager::add<Script>(Script *piece) { scripts.push_back(piece); };




