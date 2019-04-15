#include "ProjectManager.h"
#include <util/ScriptingHelper.h>
#include<editor/ConstantModels.h>
ProjectManager::ProjectManager(std::string n, std::string p) : name(n), path(p)
{
	path += "\\";
	//system(("rmdir /Q /S \"" + p + n + "\"").c_str());
	system(("mkdir \"" + p + "\\prefabs\"").c_str());
	system(("mkdir \"" + p  + "\\models\"").c_str());
	system(("mkdir \"" + p + "\\scripts\"").c_str());
	system(("mkdir \"" + p  + "\\dlls\"").c_str());
	system(("mkdir \"" + p  + "\\scenes\"").c_str());

	
	
}

ProjectManager::~ProjectManager()
{
}

Animation * ProjectManager::getAnimationByName(const char *name)
{
	for (auto animation : animations)
		if (name == animation->name)
			return animation;
	return nullptr;
}


