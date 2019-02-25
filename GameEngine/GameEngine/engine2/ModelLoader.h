#pragma once

#include <Prefab.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <CameraComponent.h>
#include <LightComponent.h>
#include <ModelComponent.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define aicolortovec3(x) glm::vec3(x.r, x.g, x.b)

namespace ModelLoader
{
	static Prefab *loadPrefab(const char *path);
	static Mesh *generateMesh(const aiMesh *mesh);
	static Prefab *processScene(const aiScene *scene, std::string);
	static Material *generateMaterial(const aiMaterial *material);
	static Object * generateCamera(const aiCamera *camera);
	static Object * generateLight(const aiLight *light);

	static void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene);
	static void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *prefabNode, aiNode *node, const aiScene *scene);


	static Prefab *loadPrefab(const char *path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return NULL;
		}
		std::string name = path;
		name = name.substr(name.find_last_of('\\') + 1, name.find_last_of('.') - name.find_last_of('\\') - 1);
		return processScene(scene, name);
	}

	static Prefab *processScene(const aiScene *scene, std::string name)
	{
		Prefab * prefab = new Prefab(name);
		
		for (int i = 0; i < scene->mNumLights; i++)
			prefab->addLight(generateLight(scene->mLights[i]));

		for (int i = 0; i < scene->mNumCameras; i++)
			prefab->addCamera(generateCamera(scene->mCameras[i]));

		
		for (int i = 0; i < scene->mNumMaterials; i++)
			prefab->addMaterials(generateMaterial(scene->mMaterials[i]));

		Mesh *(*meshes) = (Mesh **)calloc(scene->mNumMeshes,sizeof(Mesh *));
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			meshes[i] = generateMesh(scene->mMeshes[i]);
			//meshes[i]->material = prefab->materials[scene->mMeshes[i]->mMaterialIndex];
		}

		
		startingNode(meshes, prefab, prefab->rootNode, scene->mRootNode, scene);

		return prefab;
	}

	static void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene)
	{
	
		prefab->rootNode = new PrefabNode(new Object(prefab->name), glm::make_mat4(node->mTransformation[0]));

		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshes, prefab, prefab->rootNode, node->mChildren[i], scene);
		}
	}
	
	static void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene)
	{
		Object *o = NULL;
		if (node->mNumMeshes > 0) // it is a model
		{
			Model *m = new Model(node->mName.C_Str());
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				m->addMesh(meshes[node->mMeshes[i]]);
			}
			prefab->addModel(m);
			o = new Object(node->mName.C_Str());
			o->componentObject->addComponent(new ModelComponent(m));
		}
		else
		{
			o = prefab->getObject(node->mName.C_Str());

			if (!o)
			{
				o = new Object(node->mName.C_Str());
			}
		}

		PrefabNode *tmpprefabNode = new PrefabNode(o, glm::make_mat4(node->mTransformation[0]));
		rootPNode->addChild(tmpprefabNode);

		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshes,prefab, tmpprefabNode, node->mChildren[i], scene);
		}
	}

	static Material *generateMaterial(const aiMaterial *material)
	{
		Material *m = new Material();

		return m;
	}

	static Mesh * generateMesh(const aiMesh *mesh)
	{
		unsigned int *indices = (unsigned int *)calloc(mesh->mNumFaces * mesh->mFaces[0].mNumIndices, sizeof(unsigned int));
		for (int i = 0; i < mesh->mNumFaces; i++)
			for(int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				indices[i * mesh->mFaces[0].mNumIndices + j] = mesh->mFaces[i].mIndices[j];

		float *vertices = (float *)calloc(mesh->mNumVertices * 3, sizeof(float));
		for (int i = 0; i < mesh->mNumVertices; i++)
			for (int j = 0; j < 3; j++)
				vertices[i * 3 + j] = mesh->mVertices[i][j];

		float *normals = (float *)calloc(mesh->mNumVertices * 3, sizeof(float));
		for (int i = 0; i < mesh->mNumVertices; i++)
			for (int j = 0; j < 3; j++)
				normals[i * 3 + j] = mesh->mNormals[i][j];

		Mesh *tmpMesh = new Mesh(mesh->mNumVertices, mesh->mNumFaces, vertices, normals, indices);

		return tmpMesh;
	}
	
	static Object *generateLight(const aiLight *light)
	{
		Object *c = new Object(light->mName.C_Str());
		c->componentObject->addComponent(new LightComponent((LightType)light->mType, aicolortovec3(light->mColorDiffuse), aicolortovec3(light->mColorAmbient), aicolortovec3(light->mColorSpecular)));
		return c;
	}

	static Object * generateCamera(const aiCamera *camera)
	{
		Object *c = new Object(camera->mName.C_Str());
		c->componentObject->addComponent(new CameraComponent(camera->mHorizontalFOV, camera->mAspect, camera->mClipPlaneNear, camera->mClipPlaneFar));
		return c;
	}
};

