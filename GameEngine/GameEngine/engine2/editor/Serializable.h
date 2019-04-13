#pragma once

#include <iostream>
#include <fstream>
#include <direct.h>

#include <editor/ProjectManager.h>
#include<engine/SkinnedMesh.h>
class ProjectManager;
class Scene;
class Actor;

using namespace std;

class Serializable
{
public:
	static void Save(ProjectManager *, const char *);
	static ProjectManager *Load(const char *);

	static void SaveModel(ProjectManager *, Model *);
	static void ReadModel(Model *m);

	static void SaveScene(std::string, Scene*);
	static void SaveActor(ofstream& file, Actor*);
	static Scene* LoadScene(std::string);
	static Actor* LoadActor(ifstream& file, Scene*scene, std::unordered_map<ActorID::value_type, ActorID::value_type>&);


private:
	static void SaveProjectFile(ProjectManager *, const char *);


	static void WriteMaterials(ofstream& file, ProjectManager *pm);
	static void WriteTextures(ofstream& file, ProjectManager *pm);
	static void WriteModels(ofstream& file, ProjectManager *pm);
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