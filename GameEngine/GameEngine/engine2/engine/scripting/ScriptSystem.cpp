#include "ScriptSystem.h"
#include<engine/ActorManager.h>
#include<Engine.h>
#include<engine/components/ScriptComponent.h>
#include<util/ScriptingHelper.h>
#include<engine/Actor.h>
ScriptSystem::ScriptSystem()
{
	mono_set_dirs("C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc");
	domain = mono_jit_init("GameEngineDomain");
	ScriptHelper::loadMethods();
}


ScriptSystem::~ScriptSystem()
{
}

void ScriptSystem::PreUpdate()
{
}

void ScriptSystem::Update()
{
	for (auto sobject : scriptobjects)
	{
		MonoClass *mclass = mono_object_get_class(sobject);
		MonoMethod *method = mono_class_get_method_from_name(mclass, "Update", 0);
		mono_runtime_invoke(method, sobject, NULL, NULL);	
	}
}

void ScriptSystem::PostUpdate()
{
}

void ScriptSystem::initSystem()
{
 	domain_assembly =  mono_domain_create_appdomain(std::string("gamengine_runtime").data(), nullptr);
	mono_domain_set_config(domain_assembly, ".", "");
	mono_domain_set(domain_assembly, true);

	engineassembly = mono_domain_assembly_open(domain_assembly, (std::string(GE_Engine->mainPath) + "dlls\\engineassembly.dll").c_str());
	scriptassembly = mono_domain_assembly_open(domain_assembly, (std::string(GE_Engine->mainPath) + "dlls\\scriptassembly.dll").c_str());
	engineimage = mono_assembly_get_image(engineassembly);
	scriptimage = mono_assembly_get_image(scriptassembly);

}

void ScriptSystem::freeSystem()
{
	scriptobjects.clear();
	if (mono_domain_get() != domain) mono_domain_set(domain, true);
	MonoObject *exc = NULL;
	mono_gc_collect(mono_gc_max_generation());
	mono_domain_finalize(domain_assembly, 2000);
	mono_gc_collect(mono_gc_max_generation());
	mono_domain_try_unload(domain_assembly, &exc);
	if (exc)
	{
		//handleException(exc);
		return;
	}
	engineassembly = nullptr;
	scriptassembly = nullptr;
	domain_assembly = nullptr;

}



void ScriptSystem::startSytem()
{
		//create scripts objects
	auto end = GE_Engine->componentManager->end<ScriptComponent>();
	for (auto scriptcompit = GE_Engine->componentManager->begin<ScriptComponent>(); scriptcompit.operator!=(end); scriptcompit.operator++())
	{
		MonoObject *actorobject = createSharpActor(GE_Engine->actorManager->GetActor(scriptcompit->owner));
		for (auto script : scriptcompit->scripts)
		{
			MonoObject *obj = createObject(scriptimage,script->name.c_str());
			MonoClass *mclass = mono_class_from_name(engineimage, "GameEngine", "Component");
			MonoMethod *method = mono_class_get_method_from_name(mclass, ".ctor", 1);
			
			
			void *params[1];
			params[0] = actorobject;
			mono_runtime_invoke(method, obj, params, NULL);
			mclass = mono_class_from_name(scriptimage, "GameEngine", script->name.c_str());
		//	obj = mono_object_castclass_mbyref(obj, mclass);
			scriptobjects.push_back(obj);
		}
	}
	return;
		
	
}

MonoObject* ScriptSystem::createSharpActor(Actor * actor)
{
	void *params[2];
	void *test = &actor->transformation;
	params[0] = &actor;
	params[1] = &test;
	
	
	MonoObject *obj = createObject(engineimage,"Actor");
	MonoClass *mclass = mono_class_from_name(engineimage, "GameEngine", "Actor");
	MonoMethod *method = mono_class_get_method_from_name(mclass, ".ctor", 2);
	mono_runtime_invoke(method, obj, params, NULL);
	
	
	return obj;


}

MonoObject * ScriptSystem::createObject(MonoImage *img,const char * mclassname)
{
	MonoClass *mclass = mono_class_from_name(img, "GameEngine", mclassname);
	MonoObject *obj = mono_object_new(mono_domain_get(), mclass);
	return obj;
}
