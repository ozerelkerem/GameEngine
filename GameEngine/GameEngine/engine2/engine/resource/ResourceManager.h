#pragma once
#include <engine/resource/IResource.h>
#include <util/FamilyTypeID.h>
class ResourceManager
{
public:
	ResourceManager() {
		resourcelist.resize(GameEngine::Util::FamilyTypeID<IResource>::Get());
	};
	~ResourceManager();

	using ResourcePair = std::pair<IResource *, int>;

	/*
		Textures:
			123.png, count: 2
			1.png, count: 3
		Models:
			cube.model, count: 1
	*/
	std::vector<std::unordered_map<std::string, ResourcePair>> resourcelist;

	template<class T, class... ARGS>
	T * getResource(std::string path, ARGS&&... args) {
		TypeID CID = T::STATIC_RESOURCE_TYPE_ID;
		
		auto it = resourcelist[CID].find(path);
		
		if (it == resourcelist[CID].end()) // could not find
		{
			T * resource = new T(path, std::forward<ARGS>(args)...);
			ResourcePair a = std::make_pair(resource, 1);
			resourcelist[CID][path] = a;
			resource->load();

			return resource;
		}
		else
		{
			(*it).second.second++;
			return static_cast<T*>((*it).second.first);
		}
	}

	template<class T>
	T * getResource(std::string path, bool x) {
		TypeID CID = T::STATIC_RESOURCE_TYPE_ID;

		auto it = resourcelist[CID].find(path);

		return static_cast<T*>((*it).second.first);
		
	}

	template<class T>
	void changeKey(std::string oldpath, std::string newpath) {
		if (oldpath != newpath)
		{
			TypeID CID = T::STATIC_RESOURCE_TYPE_ID;
			auto it = resourcelist[CID].find(oldpath);

			resourcelist[CID][newpath] = (*it).second;
			resourcelist[CID].erase(oldpath);
		}
	}

	template<class T>
	void removeResource(T * resource) {
		TypeID CID = T::STATIC_RESOURCE_TYPE_ID;

		auto it = resourcelist[CID].find(resource->fullpath);
		
		if (it == resourcelist[CID].end()) // could not find
		{
			assert(false && "Resource not found");
		}
		else
		{
			if ((*it).second.second <= 1)
			{
				//TODO NO MORE RESOURCE YOU SHOUDL DELETE IT
			}
			else
			{
				(*it).second.second--;
			}
		}
	}

};

