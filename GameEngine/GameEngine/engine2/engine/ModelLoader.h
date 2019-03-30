#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include<string.h>
#include <type_traits>
#include <engine/Prefab.h>
#include <engine/components/CameraComponent.h>
#include <engine/components/LightComponent.h>
#include <engine/components/ModelComponent.h>
#include <engine/components/SkinnedModelComponent.h>
#include <engine/components/AnimatorComponent.h>
#include <editor/ProjectManager.h>
#include <editor/Serializable.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include<assimp/types.h>
#include <numeric>
#include<Api.h>
#include<engine/SkinnedMesh.h>
#include<engine/Animation.h>

#define aicolortovec3(x) glm::vec3(x.r, x.g, x.b)
#define aivec3tovec3(a) glm::vec3(a.x, a.y, a.z)
#define aiquattoquat(a) glm::quat(a.w, a.x, a.y, a.z)
#define aiamat4tomat4(a) glm::make_mat4(a[0])
namespace ModelLoader
{
	static Prefab *loadPrefab(const char *path, ProjectManager *project);
	static Mesh *generateMesh(const aiMesh *mesh);
	static Prefab *processScene(const aiScene *scene, std::string, ProjectManager *project);
	static Material *generateMaterial(const aiMaterial *material, std::string, ProjectManager *project);
	static Object * generateCamera(const aiCamera *camera);
	static Object * generateLight(const aiLight *light);
	static Animation * generateAnimation(const aiAnimation *anim);


	static void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene, ProjectManager *pm);
	static void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *prefabNode, aiNode *node, const aiScene *scene, ProjectManager *pm);


	static Prefab *loadPrefab(const char *path, ProjectManager *project)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return NULL;
		}


		std::string name = path;
		name = name.substr(name.find_last_of('\\') + 1, name.find_last_of('.') - name.find_last_of('\\') - 1);
		return processScene(scene, name, project);
	}

	static Prefab *processScene(const aiScene *scene, std::string name, ProjectManager *project)
	{
		Prefab * prefab = new Prefab(name);
		
		for (int i = 0; i < scene->mNumLights; i++)
			prefab->addLight(generateLight(scene->mLights[i]));

		for (int i = 0; i < scene->mNumCameras; i++)
			prefab->addCamera(generateCamera(scene->mCameras[i]));

		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			Animation *a = generateAnimation(scene->mAnimations[i]);
			if (project)
				project->add(a);
		}
		//;prefab->addAnimation(generateAnimation(scene->mAnimations[i]));

		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			Material *m = generateMaterial(scene->mMaterials[i], prefab->name + " Material " + std::to_string(i), project);
			if (project)
			{
				project->add(m);
				if (m->ambientTexture)
					project->add(m->ambientTexture);
			}

			prefab->addMaterials(m);
		}

		Mesh *(*meshes) = (Mesh **)calloc(scene->mNumMeshes,sizeof(Mesh *));
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			meshes[i] = generateMesh(scene->mMeshes[i]);
		//	meshes[i]->material = prefab->materials[scene->mMeshes[i]->mMaterialIndex];
		}

		
		startingNode(meshes, prefab, prefab->rootNode, scene->mRootNode, scene, project);

		for (int i = 0; i < prefab->numberOfModels; i++)
		{
			if (project)
				project->add(prefab->models[i]);
			Serializable::SaveModel(project, prefab->models[i]);
			for (int j = 0; j < prefab->models[i]->numOfMeshes; j++)
			{
				prefab->models[i]->meshes[j]->freeMesh();
			}
			
		}
		return prefab;
	}

	static void startingNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene, ProjectManager *pm)
	{
	
		prefab->rootNode = new PrefabNode(new Object(prefab->name), glm::mat4(aiamat4tomat4(node->mTransformation)));

		for (int i = 0; i < node->mNumChildren; i++)
		{
			processNode(meshes, prefab, prefab->rootNode, node->mChildren[i], scene,pm);
		}
	}
	
	static void processNode(Mesh **meshes, Prefab *prefab, PrefabNode *rootPNode, aiNode *node, const aiScene *scene, ProjectManager *pm)
	{
		Object *o = NULL;
		if (node->mNumMeshes > 0) // it is a model
		{
			o = new Object(node->mName.C_Str());
			Model *m = new Model(pm->path + "\\models\\" + node->mName.C_Str() + ".model",node->mName.C_Str());
			IModelComponent *mc;
			if (dynamic_cast<SkinnedMesh*>(meshes[node->mMeshes[0]]))
			{
				mc = new SkinnedModelComponent(m);
				
				o->componentObject->addComponent<SkinnedModelComponent>((SkinnedModelComponent *)mc);

			}
			else
			{
				mc = new ModelComponent(m);
				o->componentObject->addComponent<ModelComponent>((ModelComponent *)mc);
			}
			mc->numberOfMaterials = node->mNumMeshes;
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				m->addMesh(meshes[node->mMeshes[i]]);
				mc->materials.insert(mc->materials.begin(), 1, prefab->materials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex]);
			}
			prefab->addModel(m);

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
			processNode(meshes,prefab, tmpprefabNode, node->mChildren[i], scene, pm);
		}
	}

	static Material *generateMaterial(const aiMaterial *material, std::string name, ProjectManager *project)
	{
		Material *m = new Material(name);
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString *path = new aiString();
			material->GetTexture(aiTextureType_DIFFUSE, 0, path);
			m->ambientTexture = GE_Engine->resourceManager->getResource<Texture>(path->data);
		}

		aiGetMaterialFloat(material, AI_MATKEY_COLOR_DIFFUSE, &m->ambientColor[0]);
		return m;
	}

	static Mesh * generateMesh(const aiMesh *mesh)
	{
		float minx, miny, minz;
		minx = miny = minz = std::numeric_limits<float>::max();
		float maxx, maxy, maxz;
		maxx = maxy = maxz = std::numeric_limits<float>::min();

		unsigned int *indices = (unsigned int *)calloc(mesh->mNumFaces * mesh->mFaces[0].mNumIndices, sizeof(unsigned int));
		for (int i = 0; i < mesh->mNumFaces; i++)
			for(int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
				indices[i * mesh->mFaces[0].mNumIndices + j] = mesh->mFaces[i].mIndices[j];

		float *vertices = (float *)calloc(mesh->mNumVertices * 3, sizeof(float));
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices[i * 3 + 0] = mesh->mVertices[i][0];
			vertices[i * 3 + 1] = mesh->mVertices[i][1];
			vertices[i * 3 + 2] = mesh->mVertices[i][2];

			minx = min(minx, mesh->mVertices[i][0]);
			miny = min(miny, mesh->mVertices[i][1]);
			minz = min(minz, mesh->mVertices[i][2]);

			maxx = max(maxx, mesh->mVertices[i][0]);
			maxy = max(maxy, mesh->mVertices[i][1]);
			maxz = max(maxz, mesh->mVertices[i][2]);
		}
			
				

		float *normals = (float *)calloc(mesh->mNumVertices * 3, sizeof(float));
		for (int i = 0; i < mesh->mNumVertices; i++)
			for (int j = 0; j < 3; j++)
				normals[i * 3 + j] = mesh->mNormals[i][j];

		float *textureCoords = (float *)calloc(mesh->mNumVertices * 2, sizeof(float));
	
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			if (mesh->mTextureCoords[0] > 0)
			{
				textureCoords[i * 2 + 0] = mesh->mTextureCoords[0][i].x;
				textureCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				textureCoords[i * 2 + 0] = 0;
				textureCoords[i * 2 + 1] = 0;
			}
		
		}

		Mesh *tmpMesh;

		if (mesh->HasBones())
		{
			BonesList bones;
			std::vector<std::vector<std::pair<float, BoneIdType >>> weightsboneids(mesh->mNumVertices);

			for (int i = 0; i < mesh->mNumBones; i++)
			{
				auto bone = mesh->mBones[i];

				for (int j = 0; j < bone->mNumWeights; j++)
				{
					weightsboneids[bone->mWeights[j].mVertexId].push_back(std::make_pair(bone->mWeights[j].mWeight, i));
				
				}
				bones.push_back(std::make_pair(bone->mName.C_Str(),glm::mat4(aiamat4tomat4(bone->mOffsetMatrix))));
			}

			for (int i = 0; i < mesh->mNumVertices; i++)
			{

				std::sort(weightsboneids[i].begin(), weightsboneids[i].end(), [](auto a, auto b)
				{
					return a.first > b.first;
				});
				weightsboneids[i].resize(SKINNED_MESH_MAX_WEIGHT_PER_VERTICES, std::make_pair(0, 0));
				float total = 0.0f;
				for (auto weight : weightsboneids[i]) { total += weight.first; }
				total = 1 / total;
				for (int j = 0; j < SKINNED_MESH_MAX_WEIGHT_PER_VERTICES; j++)
				{ 
					weightsboneids[i][j].first *= total;
				}
	
		

			}

			auto bar = std::accumulate(weightsboneids.begin(), weightsboneids.end(), std::vector<float>{},
				[](auto& dest, auto& src) {
				for (auto a : src)
					dest.push_back(a.first);
				return dest;
			});
			auto bar2 = std::accumulate(weightsboneids.begin(), weightsboneids.end(), std::vector<BoneIdType>{},
				[](auto& dest, auto& src) {
				for (auto a : src)
					dest.push_back(a.second);
				return dest;
			});

			tmpMesh = (Mesh *)new SkinnedMesh(mesh->mNumVertices, mesh->mNumFaces, vertices, normals, indices, textureCoords,bar, bar2, bones);
		}
		else
			tmpMesh = new Mesh(mesh->mNumVertices, mesh->mNumFaces, vertices, normals, indices, textureCoords);
			


		
		
		tmpMesh->bounds = { minx, miny,minz, maxx,maxy,maxz };
		return tmpMesh;
	}
	
	static Object *generateLight(const aiLight *light)
	{
		Object *c = new Object(light->mName.C_Str());
		//	c->componentObject->addComponent(new LightComponent((LightType)light->mType, aicolortovec3(light->mColorDiffuse), aicolortovec3(light->mColorAmbient), aicolortovec3(light->mColorSpecular)));
		c->componentObject->addComponent(new LightComponent());
		return c;
	}

	static Object * generateCamera(const aiCamera *camera)
	{
		Object *c = new Object(camera->mName.C_Str());
		c->componentObject->addComponent(new CameraComponent(camera->mHorizontalFOV, camera->mAspect, camera->mClipPlaneNear, camera->mClipPlaneFar));
		return c;
	}

	static Animation * generateAnimation(const aiAnimation *anim)
	{
		AnimationNodeMap map;
		for (int i = 0; i < anim->mNumChannels; i++)
		{
			auto channel = anim->mChannels[i];
			KeyFrameHolder<glm::vec3> pos, scale;
			KeyFrameHolder<glm::quat> rot;
			
			pos.reserve(channel->mNumPositionKeys);
			scale.reserve(channel->mNumScalingKeys);
			rot.reserve(channel->mNumRotationKeys);

			for (int j = 0; j < channel->mNumPositionKeys; j++)
				pos.push_back(std::make_pair(static_cast<AnimationTimeType>(channel->mPositionKeys[j].mTime), aivec3tovec3(channel->mPositionKeys[j].mValue)));
			for (int j = 0; j < channel->mNumScalingKeys; j++)
				scale.push_back(std::make_pair(static_cast<AnimationTimeType>(channel->mScalingKeys[j].mTime), aivec3tovec3(channel->mScalingKeys[j].mValue)));
			for (int j = 0; j < channel->mNumRotationKeys; j++)
				rot.push_back(std::make_pair(static_cast<AnimationTimeType>(channel->mRotationKeys[j].mTime), aiquattoquat(channel->mRotationKeys[j].mValue)));
			map[channel->mNodeName.C_Str()] = new AnimationNode(channel->mNodeName.C_Str(), pos, scale, rot);
		}
		return new Animation(anim->mName.C_Str(), map, static_cast<AnimationTimeType>(anim->mDuration), static_cast<AnimationTimeType>(anim->mTicksPerSecond));
	}
};

