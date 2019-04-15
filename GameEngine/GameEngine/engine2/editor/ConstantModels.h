#pragma once
#include<physx/PxPhysicsAPI.h>
#include <engine/ModelLoader.h>
class Texture;
namespace ConstantModels
{
	static Model *CreateTranslateTool()
	{

		/*
		ModelLoader::loadPrefab("ConstantModels/translate.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		ModelLoader::loadPrefab("ConstantModels/scale.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		ModelLoader::loadPrefab("ConstantModels/rotate.fbx", new ProjectManager("Kero Game 1", "C:\\GameEngine\\"));
		*/
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/translate.model", "TranslateModel");

		/*Model *model = new Model("translate");
		model->path = "ConstantModels/translate.model";
		model->loadModelToGPU();
		return model;*/
	}

	static Model *CreateScaleTool()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/scale.model", "ScaleModel");
	/*	Model *model = new Model("scale");
		model->path = "ConstantModels/scale.model";
		model->loadModelToGPU();
		return model;*/
	}

	static Model *CreateRotateTool()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/rotate.model", "RotateModel");
	/*	Model *model = new Model("rotate");
		model->path = "ConstantModels/rotate.model";
		model->loadModelToGPU();
		return model;*/
	}

	static Model *getCubeModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Cube.model", "Cube Model");
	}
	static Model *getConeModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Cone.model", "Cone Model");
	}
	static Model *getSphereModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Sphere.model", "Sphere Model");
	}
	static Model *getCylinderModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Cylinder.model", "Cylinder Model");
	}
	static Model *getCapsuleModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Capsule.model", "Capsule Model");
	}
	static Model *getPlaneModel()
	{
		return GE_Engine->resourceManager->getResource<Model>("ConstantModels/Plane.model", "Plane Model");
	}

	
}

namespace ConstantMaterials {
	class Materials
		{
		public:
			static Material * defaultMaterial;

			static void initMaterial();
		};
}

namespace ConstantTextures
{
	class Textures
	{
	public:
		static Texture * sceneTexture;
		static Texture * cameraTexture;
		static Texture * noTexture;
		static Texture * lightTexture;
		static Texture * scriptTexture;
		static Texture * animationTexture;
		static Texture * modelTexture;
		static void initTextures();
	};
		
}

	
namespace ConstantPhysicMaterials
{
	class PhysicMaterials
	{
	public:
		static physx::PxMaterial* defaultMaterial;
		static void initMaterials();
	};
}