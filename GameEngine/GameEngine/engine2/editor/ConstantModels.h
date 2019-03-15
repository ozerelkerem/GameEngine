#pragma once

#include <engine/ModelLoader.h>

namespace ConstantModels
{
	static Model *CreateTranslateTool()
	{
		/*
		ModelLoader::loadPrefab("ConstantModels/translate.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		ModelLoader::loadPrefab("ConstantModels/scale.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		ModelLoader::loadPrefab("ConstantModels/rotate.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		*/
		Model *model = new Model("translate");
		model->path = "ConstantModels/translate.model";
		model->loadModelToGPU();
		return model;
	}

	static Model *CreateScaleTool()
	{
		Model *model = new Model("scale");
		model->path = "ConstantModels/scale.model";
		model->loadModelToGPU();
		return model;
	}

	static Model *CreateRotateTool()
	{
		Model *model = new Model("rotate");
		model->path = "ConstantModels/rotate.model";
		model->loadModelToGPU();
		return model;
	}

	
}