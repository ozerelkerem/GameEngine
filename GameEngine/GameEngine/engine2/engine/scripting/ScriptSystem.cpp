#include "ScriptSystem.h"

#include<Engine.h>

ScriptSystem::ScriptSystem()
{
}


ScriptSystem::~ScriptSystem()
{
}

void ScriptSystem::PreUpdate()
{
}

void ScriptSystem::Update()
{
}

void ScriptSystem::PostUpdate()
{
}

void ScriptSystem::initSystem()
{
	mono_set_dirs("C:\\Program Files (x86)\\Mono\\lib", "C:\\Program Files (x86)\\Mono\\etc");
	domain = mono_jit_init("GameEngineDomain");
	engineassembly = mono_domain_assembly_open(domain, (std::string(GE_Engine->mainPath) + "dlls\\engineassembly.dll").c_str());
	scriptassembly = mono_domain_assembly_open(domain, (std::string(GE_Engine->mainPath) + "dlls\\scriptassembly.dll").c_str());
	engineimage = mono_assembly_get_image(engineassembly);
	scriptimage = mono_assembly_get_image(scriptassembly);

}

void ScriptSystem::freeSystem()
{
	if (mono_domain_get() != domain) mono_domain_set(domain, true);
	MonoObject *exc = NULL;
	mono_gc_collect(mono_gc_max_generation());
	mono_domain_finalize(domain, 2000);
	mono_gc_collect(mono_gc_max_generation());
	mono_domain_try_unload(domain, &exc);
	
	engineassembly = nullptr;
	scriptassembly = nullptr;
	engineimage = nullptr;
	scriptimage = nullptr;
	domain = nullptr;
	mono_jit_cleanup(domain);

}
