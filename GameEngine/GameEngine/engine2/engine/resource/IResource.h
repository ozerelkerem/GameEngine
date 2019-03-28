#pragma once
#include <Api.h>

using ResourceTypeID = TypeID;

class IResource
{
public:
	IResource(std::string fp) : fullpath(fp) {};
	~IResource();

	std::string fullpath;
	
	virtual void load() = 0;
	virtual void unload() = 0;
};

