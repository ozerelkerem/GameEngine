#include "ProjectManager.h"
#include <util/ScriptingHelper.h>
ProjectManager::ProjectManager(std::string n, std::string p) : name(n), path(p + n)
{
	path += "\\";
	system(("rmdir /Q /S \"" + p + n + "\"").c_str());
	system(("mkdir \"" + p + n + "\\models\"").c_str());
	system(("mkdir \"" + p + n + "\\scripts\"").c_str());
	system(("mkdir \"" + p + n + "\\dlls\"").c_str());
	
}

ProjectManager::~ProjectManager()
{
}


