#pragma once

#include <engine/ModelLoader.h>

namespace ConstantModels
{
	static Model *CreateTranslateTool()
	{
		Prefab *prefab = ModelLoader::loadPrefab("ConstantModels/translate.fbx");
		return prefab->models[0];
	}

	static Model *CreateScaleTool()
	{
		Prefab *prefab = ModelLoader::loadPrefab("ConstantModels/scale.fbx");
		return prefab->models[0];
	}

	static Model *CreateRotateTool()
	{
		Prefab *prefab = ModelLoader::loadPrefab("ConstantModels/rotate.fbx");
		return prefab->models[0];
	}

	
}