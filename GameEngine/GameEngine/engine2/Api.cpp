#include "Api.h"
#include <Engine.h>
#include <engine/Shader.h>
#include <editor/ConstantModels.h>

Engine *GE_Engine = nullptr;
Shader *normalShader = nullptr;
Shader *colorShader = nullptr;
Shader *spriteShader = nullptr;
#include <memory/MemoryManager.h>
MemoryManager*	ECSMemoryManager = new MemoryManager();
GlobalMemoryUser::GlobalMemoryUser() : ECS_MEMORY_MANAGER(ECSMemoryManager)
{}

GlobalMemoryUser::~GlobalMemoryUser()
{}

 const void* GlobalMemoryUser::Allocate(size_t memSize)
{
	return ECS_MEMORY_MANAGER->Allocate(memSize);
}

 void GlobalMemoryUser::Free(void* pMem)
{
	ECS_MEMORY_MANAGER->Free(pMem);
}


void Initialize()
{
	if (GE_Engine == nullptr)
		GE_Engine = new Engine();
	if (normalShader == nullptr)
		normalShader = new Shader("engine2/shaders/mainShadervs.glsl", "engine2/shaders/mainShaderfs.glsl");
	if (colorShader == nullptr)
		colorShader = new Shader("engine2/shaders/colorShadervs.glsl", "engine2/shaders/colorShaderfs.glsl");
	if (spriteShader == nullptr)
		spriteShader = new Shader("engine2/shaders/spriteShadervs.glsl", "engine2/shaders/spriteShaderfs.glsl");

	ConstantTextures::Textures::initTextures();
	ConstantMaterials::Materials::initMaterial();
	ConstantPhysicMaterials::PhysicMaterials::initMaterials();
}

void Terminate()
{
	if (GE_Engine != nullptr)
	{
		delete GE_Engine;
		GE_Engine = nullptr;
	}

	// check for memory leaks
	//Memory::Internal::ECSMemoryManager->CheckMemoryLeaks();
}


