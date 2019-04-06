#pragma once


#include <engine/systems/System.h>


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>
#include <mono/utils/mono-logger.h>
#include<mono/metadata/debug-helpers.h>
#include<engine/scripting/Script.h>
#include<Engine.h>
#include<engine/ActorManager.h>
class Actor;

class ScriptSystem : public System<ScriptSystem>
{
public:
	ScriptSystem();
	~ScriptSystem();

	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;

	void initSystem();
	void freeSystem();
	void startSytem();

	std::vector<std::pair<MonoObject*, Script *>> scriptobjects;
	
	inline MonoObject *getActorObject(ActorID id) {
		auto it = actorobjects.find(id);
		if (it != actorobjects.end())
			return it->second;
		else
		{
			return createSharpActor(GE_Engine->actorManager->GetActor(id));
		}
	}

private:
	MonoDomain *domain;
	MonoDomain *domain_assembly;
	MonoAssembly *engineassembly;
	MonoAssembly *scriptassembly;
	MonoImage *engineimage;
	MonoImage *scriptimage;

	MonoMethod *emptymethod;

	MonoObject* createSharpActor(Actor *actor);
	MonoObject* createObject(MonoImage *,const char *mclass);

	void registerScriptFunctions(Script *s);

	std::unordered_map<ActorID::value_type, MonoObject *> actorobjects;
	
};
