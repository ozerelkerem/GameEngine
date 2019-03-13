#pragma once

#include <iostream>
#include <fstream>
#include <direct.h>

#include <editor/ProjectManager.h>

class ProjectManager;
class Model;

using namespace std;

class Serializable
{
public:
	static void Save(ProjectManager *, const char *);
	static ProjectManager *Load(const char *);

	static void SaveModel(ProjectManager *, Model *);

private:
	static void SaveProjectFile(ProjectManager *, const char *);
	static void SaveComponents();

	static void WriteMaterials(ofstream& file, ProjectManager *pm);
	static void WriteTextures(ofstream& file, ProjectManager *pm);
	static void WriteModels(ofstream& file, ProjectManager *pm);
public:
	template<typename T>
	inline static void writefile(ofstream& file, T val, size_t size = 1)
	{
		file.write(reinterpret_cast<char *>(&val), sizeof(T) * size);
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
		Serializable::readfile(file, val,stringsize);
	}

};