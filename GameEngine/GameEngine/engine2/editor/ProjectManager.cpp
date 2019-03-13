#include "ProjectManager.h"

ProjectManager::ProjectManager(std::string n, std::string p) : name(n), path(p + n)
{
	_mkdir((p + n).c_str());
	_mkdir((p + n + "/models").c_str());
}


ProjectManager::~ProjectManager()
{
}


