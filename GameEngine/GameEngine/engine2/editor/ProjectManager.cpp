#include "ProjectManager.h"
#include <util/ScriptingHelper.h>
#include<editor/ConstantModels.h>
ProjectManager::ProjectManager(std::string n, std::string p) : name(n), path(p)
{
	path += "\\";
	//system(("rmdir /Q /S \"" + p + n + "\"").c_str());
	system(("mkdir \"" + p  + "\\models\"").c_str());
	system(("mkdir \"" + p + "\\scripts\"").c_str());
	system(("mkdir \"" + p  + "\\dlls\"").c_str());
	system(("mkdir \"" + p  + "\\scenes\"").c_str());

	
	
}

ProjectManager::~ProjectManager()
{
}


