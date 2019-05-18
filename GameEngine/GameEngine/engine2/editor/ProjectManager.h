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

	std::vector<std::string> models;
	std::vector<std::string> scenes;
	std::vector<std::string> actorprefab;
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
		int i = 0;
		for (; i < textures.size(); i++)
		{
			if (textures[i] == t)
				return i;
		}
		return Texture::INVALID_TEXTURE_ID;

	}
	inline int findIndexofScript(Script *t) {
		int i = 0;
		for (; i < scripts.size(); i++)
		{
			if (scripts[i] == t)
				return i;
		}
		return -1;

	}
	inline int findIndexofMaterial(Material *t) {
		int i = 0;
		for (; i < materials.size(); i++)
		{
			if (materials[i] == t)
				return i;
		}
		return -1;
	}

	Animation *getAnimationByName(const char*);


	void loadTexturesForDisplay();
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
template<> inline void ProjectManager::add<Scene>(Scene *piece) { scenes.push_back(piece->name); };
template<> inline void ProjectManager::add<Model>(Model *piece) { models.push_back(piece->name); };
template<> inline void ProjectManager::add<Material>(Material *piece) { materials.push_back(piece); };
template<> inline void ProjectManager::add<Prefab>(Prefab *piece) { prefabs.push_back(piece); };
template<> inline void ProjectManager::add<Texture>(Texture *piece) { textures.push_back(piece); };
template<> inline void ProjectManager::add<Animation>(Animation *piece) { animations.push_back(piece); };
template<> inline void ProjectManager::add<Script>(Script *piece) { scripts.push_back(piece); };




