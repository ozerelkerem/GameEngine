#include "ProjectManager.h"

ProjectManager::ProjectManager(std::string n, std::string p) : name(n), path(p + n)
{
	path += "\\";
	system(("mkdir \"" + p + n + "\\models\"").c_str());

}

ProjectManager::~ProjectManager()
{
}


