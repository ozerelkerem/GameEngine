#pragma once

#include <Prefab.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define aicolortovec3(x) glm::vec3(x.r, x.g, x.b)

namespace ModelLoader
{
	Prefab *loadPrefab(const char *path);
	Mesh *generateMesh(const aiMesh *mesh);
	Prefab *processScene(const aiScene *scene);
	Material *generateMaterial(const aiMaterial *material);
	Camera * generateCamera(const aiCamera *camera);

	void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene);
	void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *prefabNode, aiNode *node, const aiScene *scene);


	Prefab *loadPrefab(const char *path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return NULL;
		}

		return processScene(scene);
	}

	Prefab *processScene(const aiScene *scene)
	{
		Prefab * prefab = new Prefab();

		
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
			meshes[i]->material = prefab->materials[scene->mMeshes[i]->mMaterialIndex];
		}

		startingNode(meshes, prefab, prefab->rootNode, scene->mRootNode, scene);

		return prefab;
	}

	void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene)
	{
	
		prefab->rootNode = new PrefabNode(NULL, glm::make_mat4(node->mTransformation[0]));

		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshes, prefab, prefab->rootNode, node->mChildren[i], scene);
		}
	}
	
	void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene)
	{
		Object *o=NULL;
		if (node->mNumMeshes > 0) // it is a model
		{
			Model *m = new Model(node->mName.C_Str());
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				m->addMesh(meshes[node->mMeshes[i]]);
			}
			prefab->addModel(m);
			o = m;
	
		}
		else; //kamera veya ýþýk veya empty

		PrefabNode *tmpprefabNode = new PrefabNode(o, glm::make_mat4(node->mTransformation[0]));
		rootPNode->addChild(tmpprefabNode);
		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshes,prefab, tmpprefabNode, node->mChildren[i], scene);
		}
	}

	Material *generateMaterial(const aiMaterial *material)
	{
		Material *m = new Material();

		return m;
	}

	Mesh * generateMesh(const aiMesh *mesh)
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

	Camera * generateCamera(const aiCamera *camera)
	{
		Camera *c = new Camera(camera->mHorizontalFOV, camera->mAspect, camera->mClipPlaneNear, camera->mClipPlaneFar, camera->mName.C_Str());
		return c;
	}

	Light *generateLight(const aiLight *light)
	{
		Light *l = new Light((LightType)light->mType, aicolortovec3(light->mColorDiffuse), aicolortovec3(light->mColorAmbient), aicolortovec3(light->mColorSpecular), light->mName.C_Str());
		return l;
	}

	//void processNode(Model *model, aiNode *node, const aiScene *scene);
	//Model * loadModelFromFile(const char *path);
	//Mesh *createMesh(aiMesh *mesh);

	//Model * loadModelFromFile(const char *path)
	//{
	//	Assimp::Importer importer;

	//	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	//	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	//	{
	//		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
	//		return NULL;
	//	}



	//	std::string name = path;
	//	name = name.substr(name.find_last_of('\\') + 1, name.find_last_of('.') - name.find_last_of('\\') - 1);

	//	Model *m = new Model(name);
	//	processNode(m, scene->mRootNode, scene);

	//	return NULL;
	//}

	//void processNode(Model *model, aiNode *node, const aiScene *scene)
	//{
	//	for (int i = 0; i < node->mNumMeshes; i++)
	//	{
	//		Mesh *m = createMesh(scene->mMeshes[node->mMeshes[i]]);


	//	}

	//	for (int i = 0; i < node->mNumChildren; i++)
	//	{
	//		processNode(model, node->mChildren[i], scene);
	//	}
	//}

	//Mesh *createMesh(aiMesh *mesh)
	//{
	//	Mesh * m = new Mesh();
	//	return m;
	//}


};
