#pragma once



#include <iostream>
#include <fstream>
#include <direct.h>

#include<engine/components/ModelComponent.h>
#include<engine/components/ScriptComponent.h>

#include <editor/ProjectManager.h>
#include<engine/SkinnedMesh.h>
class ProjectManager;
class Scene;
class Actor;

using namespace std;

class Serializable
{
public:
	static void Save(ProjectManager *);
	static ProjectManager *Load(const char *);

	static void SaveModel(ProjectManager *, Model *);
	static void ReadModel(Model *m);

	static void SaveActorasaPrefab(ProjectManager *, Actor*);
	static void AddPrefab(ProjectManager *pm, std::string name, Actor *targetactor);

	static void SaveScene(ProjectManager *, Scene*);
	static void SaveActor(ProjectManager *, ofstream& file, Actor*);
	static Scene* LoadScene(ProjectManager *, std::string);
	static Actor* LoadActor(ProjectManager *, ifstream& file, Scene*scene, std::unordered_map<ActorID::value_type, ActorID::value_type>&);

	//componets
	template<class T>
	static void SaveComponent(ProjectManager *,ofstream& file, T *component);
	template<class T>
	static void _SaveComponent(ProjectManager *, ofstream& file, T *component);
	template<class T>
	static void LoadComponent(ProjectManager *, ifstream& file, Actor *a);
	template<class T>
	static void _LoadComponent(ProjectManager *, ifstream& file, Actor *a);

	static void SaveIModelComponent(ProjectManager *, ofstream& file, IModelComponent *component);
	static void LoadIModelComponentMaterials(ProjectManager *, ifstream& file, IModelComponent *component);

private:
	static void SaveProjectFile(ProjectManager *);


	static void WriteMaterials(ofstream& file, ProjectManager *pm);
	static void WriteTextures(ofstream& file, ProjectManager *pm);
	static void WriteModels(ofstream& file, ProjectManager *pm);
	static void WriteScenes(ofstream& file, ProjectManager *pm);
	static void WriteScripts(ofstream& file, ProjectManager *pm);
	static void WritePrefabs(ofstream& file, ProjectManager *pm);
	static void WriteAnimations(ofstream& file, ProjectManager *pm);


	static void ReadMaterials(ifstream& file, ProjectManager *pm);
	static void ReadTextures(ifstream& file, ProjectManager *pm);
	static void ReadModels(ifstream& file, ProjectManager *pm);
	static void ReadScenes(ifstream& file, ProjectManager *pm);
	static void ReadScripts(ifstream& file, ProjectManager *pm);
	static void ReadPrefabs(ifstream& file, ProjectManager *pm);
	static void ReadAnimations(ifstream& file, ProjectManager *pm);

public:
	template<typename T>
	inline static void writefile(ofstream& file, T val, size_t size = 1)
	{
		file.write(reinterpret_cast<char *>(&val), sizeof(T) * size);
	}
	template<typename T>
	inline static void writefile(ofstream& file, T *val, size_t size = 1)
	{
		file.write(reinterpret_cast<char *>(val), sizeof(T) * size);
	}
	
	inline static void writefile(ofstream& file, std::string val)
	{
		Serializable::writefile(file, val.size());
		file.write(val.c_str(), val.size());
	}

	template<typename T>
	inline static void readfile(ifstream& file, T *val, size_t size = 1)
	{
		file.read(reinterpret_cast<char *>(val), sizeof(T) * size);
	}

	inline static void readfile(ifstream& file, std::string *val)
	{
		size_t stringsize;
		Serializable::readfile(file,&stringsize);;
		val->resize(stringsize);
		Serializable::readfile(file, val->data(),stringsize);
	}

};


