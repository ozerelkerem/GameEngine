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
	mono_thread_set_main(mono_thread_current());

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
		mono_runtime_invoke(sobject.second->UpdateMethod, sobject.first, NULL, NULL);	
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

	emptymethod =  mono_class_get_method_from_name(mono_class_from_name(engineimage, "GameEngine", "ScriptComponent"),"Start", 0);

	
	MonoClass *cls = mono_class_from_name(engineimage, "GameEngine", "GameManager");
	MonoClassField *field = mono_class_get_field_from_name(cls,"_projectmanagerptr");
	
	
	mono_field_static_set_value(mono_class_vtable(domain_assembly, cls),field, &GE_Engine->projectManager);
	
}

void ScriptSystem::freeSystem()
{
	actorobjects.clear();
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
	scriptstemp.clear();
	
	for (auto chunk : GE_Engine->componentManager->GetComponentContainer<ScriptComponent>()->m_Chunks)
	{
		for (auto scriptcompit : chunk->objects)
	//create scripts objects
		{
			MonoObject *actorobject = createSharpActor(GE_Engine->actorManager->GetActor(scriptcompit->owner));
			int i = 0;
			for (auto script : scriptcompit->scripts)
			{
				if (scriptstemp.find(script) == scriptstemp.end())
				{
					registerScriptFunctions(script);
					scriptstemp.insert(script);
				}

				MonoObject *obj = createObject(scriptimage,script->name.c_str());
				MonoClass *mclass = mono_class_from_name(engineimage, "GameEngine", "Component");
				MonoMethod *method = mono_class_get_method_from_name(mclass, ".ctor", 1);
				MonoClassField *field = mono_class_get_field_from_name(mclass, "_projectmanagerptr");
				mono_field_set_value(obj, field, &GE_Engine->projectManager);
			
			
				void *params[1];
				params[0] = actorobject;
				mono_runtime_invoke(method, obj, params, NULL);
				mclass = mono_class_from_name(scriptimage, "GameEngine", script->name.c_str());
			//	obj = mono_object_castclass_mbyref(obj, mclass);
				scriptobjects.push_back(std::make_pair(obj, script));
				scriptcompit->objects[i++] = obj;
			}
		}

		
	}
	
	for (auto sobject : scriptobjects)
	{
		mono_runtime_invoke(sobject.second->StartMethod, sobject.first, NULL, NULL);
	}
		
	
}

void ScriptSystem::addObjectRuntime(ScriptComponent *scriptcompit)
{
	waitinglist.push_back(scriptcompit);
}

void ScriptSystem::addWaitings()
{
	for (auto scriptcompit : waitinglist)
	{
		MonoObject *actorobject = createSharpActor(GE_Engine->actorManager->GetActor(scriptcompit->owner));
		int i = 0;
		for (auto script : scriptcompit->scripts)
		{
			if (scriptstemp.find(script) == scriptstemp.end())
			{
				registerScriptFunctions(script);
				scriptstemp.insert(script);
			}

			MonoObject *obj = createObject(scriptimage, script->name.c_str());
			MonoClass *mclass = mono_class_from_name(engineimage, "GameEngine", "Component");
			MonoMethod *method = mono_class_get_method_from_name(mclass, ".ctor", 1);
			MonoClassField *field = mono_class_get_field_from_name(mclass, "_projectmanagerptr");
			mono_field_set_value(obj, field, &GE_Engine->projectManager);


			void *params[1];
			params[0] = actorobject;
			mono_runtime_invoke(method, obj, params, NULL);
			mclass = mono_class_from_name(scriptimage, "GameEngine", script->name.c_str());
			//	obj = mono_object_castclass_mbyref(obj, mclass);
			scriptobjects.push_back(std::make_pair(obj, script));
			scriptcompit->objects[i++] = obj;

			mono_runtime_invoke(scriptobjects.back().second->StartMethod, scriptobjects.back().first, NULL, NULL);
		}
	}
	waitinglist.clear();
		

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
	
	actorobjects[actor->actorID] = obj;
	
	return obj;


}

MonoObject * ScriptSystem::createObject(MonoImage *img,const char * mclassname)
{
	MonoClass *mclass = mono_class_from_name(img, "GameEngine", mclassname);
	MonoObject *obj = mono_object_new(mono_domain_get(), mclass);
	return obj;
}

void ScriptSystem::registerScriptFunctions(Script * s)
{
	{//Start
		MonoClass *mclass = mono_class_from_name(scriptimage, "GameEngine", s->name.c_str());
		MonoMethod *method = mono_class_get_method_from_name(mclass, "Start", 0);
		s->StartMethod = method ? method : emptymethod;
	}

	{//Update
		MonoClass *mclass = mono_class_from_name(scriptimage, "GameEngine", s->name.c_str());
		MonoMethod *method = mono_class_get_method_from_name(mclass, "Update", 0);
		s->UpdateMethod = method ? method : emptymethod;
	}
	{//OnCollisionEnter
		MonoClass *mclass = mono_class_from_name(scriptimage, "GameEngine", s->name.c_str());
		MonoMethod *method = mono_class_get_method_from_name(mclass, "onCollisionEnter", 1);
		s->OnCollisionEnterMethod = method ? method : emptymethod;
	}
}
