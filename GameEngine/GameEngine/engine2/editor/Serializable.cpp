#include "Serializable.h"
#include<engine/Scene.h>
#include<engine/ActorManager.h>



void Serializable::Save(ProjectManager *pm)
{
	SaveProjectFile(pm);
}

ProjectManager * Serializable::Load(const char *path)
{
	ifstream file;

	file.open(path, ios::in | ios::binary);
	if (!file)
		throw std::exception("File error when reading.");
	
	std::string name,pathh;

	readfile(file, &name);
	readfile(file, &pathh);
	
	ProjectManager *pm = new ProjectManager(name, pathh);
	pm->path = pathh;
	ReadTextures(file, pm);
	ReadMaterials(file, pm);
	ReadScenes(file, pm);
	ReadModels(file, pm);

	return pm;

}



void Serializable::SaveProjectFile(ProjectManager *pm)
{
	ofstream out;
	
	out.open(pm->path + pm->name + ".project", ios::out | ios::binary);
	if (!out)
		throw std::exception("File error when saving.");
	else
	{
		//project name
		Serializable::writefile(out, pm->name);
		Serializable::writefile(out, pm->path);
		WriteTextures(out, pm);
		WriteMaterials(out, pm);
		WriteScenes(out, pm);
		WriteModels(out, pm);
		
	}
	out.close();
}

void Serializable::WriteTextures(ofstream& file, ProjectManager *pm)
{
	Serializable::writefile(file, pm->textures.size());
	for (auto texture : pm->textures)
	{
		Serializable::writefile(file, texture->name);
		Serializable::writefile(file, texture->fullpath);
	}
}
void Serializable::ReadTextures(ifstream & file, ProjectManager * pm)
{
	size_t size;
	readfile(file, &size);
	std::string name;
	std::string fullpath;
	pm->materials.resize(size);
	for (int i = 0; i < size; i++)
	{
		readfile(file, &name);
		readfile(file, &fullpath);
		Texture *texture = new Texture(name, fullpath);
		pm->textures[i] = texture;
	}
}
void Serializable::WriteScenes(ofstream& file, ProjectManager *pm)
{
	Serializable::writefile(file, pm->scenes.size());
	for (auto scene : pm->scenes)
	{
		Serializable::writefile(file, scene);
	}
}
void Serializable::ReadScenes(ifstream & file, ProjectManager * pm)
{
	size_t size;
	readfile(file, &size);
	std::string name;
	for (int i = 0; i < size; i++)
	{
		readfile(file, &name);
		pm->scenes.push_back(name);
	}
}
void Serializable::WriteMaterials(ofstream & file, ProjectManager * pm)
{
	Serializable::writefile(file, pm->materials.size());
	for (auto material : pm->materials)
	{
		Serializable::writefile(file, material->name);
		Serializable::writefile(file, material->ambientColor);
		Serializable::writefile(file, pm->findIndexofTexture(material->ambientTexture));
	}
}
void Serializable::ReadMaterials(ifstream & file, ProjectManager * pm)
{
	size_t size;
	readfile(file, &size);
	std::string name;
	int textureid;
	pm->materials.resize(size);
	for (int i = 0; i < size; i++)
	{
		readfile(file, &name);
		Material *m = new Material(name);
		readfile(file, &m->ambientColor);
		readfile(file, &textureid);
		m->ambientTexture = pm->textures[textureid];
		pm->materials[i] = m;
	}
}
void Serializable::ReadModels(ifstream & file, ProjectManager * pm)
{
	size_t size;
	readfile(file, &size);
	std::string name;
	for (int i = 0; i < size; i++)
	{
		readfile(file, &name);
		pm->models.push_back(name);
	}
}




void Serializable::SaveScene(ProjectManager *pm, std::string path,Scene *scene)
{
	ofstream file;
	file.open(path + "scenes\\" + scene->name + ".scene", ios::out | ios::binary);
	if(!file)
		throw std::exception("File error when saving scene.");
	writefile(file, scene->name);

	std::vector<Actor *> list;
	Actor *actor;
	list.push_back(GE_Engine->actorManager->GetActor(scene->rootActor));
	while (!list.empty())
	{

		actor = list.back();
		list.pop_back();

		for (int i = 0; i < actor->numberOfChildren; i++)
		{
			list.push_back(GE_Engine->actorManager->GetActor(actor->children[i]));
		}

		SaveActor(pm,file,actor);
		
	}
	
	file.close();
}

void Serializable::SaveActor(ProjectManager *pm, ofstream& file,Actor *actor)
{
	writefile(file, actor->name);
	writefile(file, actor->actorID);
	writefile(file, actor->transformation);
	writefile(file, actor->parent);
	writefile(file, actor->numberOfChildren);
	for (int i = 0; i < actor->numberOfChildren; i++)
	{
		writefile(file, actor->children[i]);
	}

	//components
	SaveComponent(pm, file,actor->GetComponent<ModelComponent>());
	SaveComponent(pm, file, actor->GetComponent<SkinnedModelComponent>());
	SaveComponent(pm, file,actor->GetComponent<LightComponent>());
	SaveComponent(pm, file,actor->GetComponent<AnimatorComponent>());
	SaveComponent(pm, file,actor->GetComponent<RigidBodyComponent>());
	SaveComponent(pm, file,actor->GetComponent<CameraComponent>());
	SaveComponent(pm, file,actor->GetComponent<CapsuleColliderComponent>());
	SaveComponent(pm, file,actor->GetComponent<CubeColliderComponent>());
	SaveComponent(pm, file,actor->GetComponent<SphereColliderComponent>());

}

Scene * Serializable::LoadScene(ProjectManager *pm, std::string name)
{
	
	auto endd= GE_Engine->actorManager->actorContainer->end();
	for (auto it = GE_Engine->actorManager->actorContainer->begin(); it != endd; it.operator++())
	{
		GE_Engine->actorManager->DestroyActor(it->actorID);
	}
	

	std::vector<SkinnedModelComponent*> smclist;
	std::unordered_map<ActorID::value_type, ActorID::value_type> oldnewids;
	ifstream file;
	file.open(pm->path + "scenes\\" + name + ".scene", ios::in | ios::binary);
	if (!file)
		throw std::exception("File error when reading scene.");
	std::vector<Actor*> list;
	Scene *scene = (Scene*)calloc(sizeof(Scene),1);
	readfile(file, &scene->name);

	while (file.peek() != EOF)
		list.push_back(LoadActor(pm,file,scene,oldnewids));

	scene->rootActor = list[0]->actorID;
	
	file.close();
	int i = 0;
	for (auto actor : list)
	{
		actor->parent = actor->parent != ActorID::INVALID_HANDLE ? oldnewids[actor->parent] : ActorID::INVALID_HANDLE;
		for (int i = 0; i < actor->numberOfChildren; i++)
		{
			actor->children[i] = oldnewids[actor->children[i]];
		}
		if(i++>0)
			actor->transformation.parent = &GE_Engine->actorManager->GetActor(actor->parent)->transformation;
		if (auto x = actor->GetComponent<SkinnedModelComponent>())
		{
			x->rootBone = x->rootBone != ActorID::INVALID_HANDLE ? oldnewids[x->rootBone] : ActorID::INVALID_HANDLE;
			smclist.push_back(x);
		}
	}

	for (auto cmp : smclist)
		if(cmp->rootBone != ActorID::INVALID_HANDLE ) cmp->matchBones();

	return scene;
}

Actor * Serializable::LoadActor(ProjectManager *pm, ifstream & file,Scene *scene, std::unordered_map<ActorID::value_type, ActorID::value_type>& oldnewids)
{

	ActorID aid = GE_Engine->actorManager->CreateActor();
	Actor *actor = GE_Engine->actorManager->GetActor(aid);
	actor->scene = scene;
	readfile(file, &actor->name);
	readfile(file, &aid);
	readfile(file, &actor->transformation);
	readfile(file, &actor->parent);
	readfile(file, &actor->numberOfChildren);
	actor->children = (ActorID*)malloc(sizeof(ActorID)*(actor->numberOfChildren +1));
	for (int i = 0; i < actor->numberOfChildren; i++)
	{
		readfile(file, &actor->children[i]);
	}
	actor->componentManagerInstance = GE_Engine->componentManager;
	oldnewids[aid] = actor->actorID;


	LoadComponent<ModelComponent>(pm, file, actor);
	LoadComponent<SkinnedModelComponent>(pm, file, actor);
	LoadComponent<LightComponent>(pm, file, actor);
	LoadComponent<AnimatorComponent>(pm, file, actor);
	LoadComponent<RigidBodyComponent>(pm, file, actor);
	LoadComponent<CameraComponent>(pm, file, actor);
	LoadComponent<CapsuleColliderComponent>(pm, file, actor);
	LoadComponent<CubeColliderComponent>(pm, file, actor);
	LoadComponent<SphereColliderComponent>(pm, file, actor);

	return actor;
}
	





void Serializable::WriteModels(ofstream & file, ProjectManager * pm)
{
	Serializable::writefile(file, pm->models.size());
	for (auto model : pm->models)
	{
		Serializable::writefile(file, model);
		Serializable::writefile(file, pm->path + "/models/" + model + ".model");
	}
}
void Serializable::SaveModel(ProjectManager *pm, Model *m)
{
	ofstream file;
	file.open(m->fullpath, ios::binary | ios::out);
	if (!file)
		throw std::exception("File error when saving a model.");
	Serializable::writefile(file, m->numOfMeshes);
	Serializable::writefile(file, m->getType());
	for (int i = 0; i < m->numOfMeshes; i++)
	{
		Mesh *mesh = m->meshes[i];
		writefile(file, mesh->numberOfVertices);
		writefile(file, mesh->vertices, mesh->numberOfVertices * 3);
		writefile(file, mesh->normals, mesh->numberOfVertices * 3);
		writefile(file, mesh->textureCoords, mesh->numberOfVertices * 2);
		writefile(file, mesh->numberOfIndices);
		writefile(file, mesh->indices, mesh->numberOfIndices * 3);
		writefile(file, mesh->bounds);
		if (m->getType() == ModelType::Skinned)
		{
			SkinnedMesh * mesh2 = (SkinnedMesh *)mesh;
			writefile(file, &mesh2->weights.data()[0],SKINNED_MESH_MAX_WEIGHT_PER_VERTICES* mesh->numberOfVertices);
			writefile(file, &mesh2->boneids.data()[0], SKINNED_MESH_MAX_WEIGHT_PER_VERTICES* mesh->numberOfVertices);
			writefile(file, mesh2->bones.size());
			for (auto bone : mesh2->bones)
			{
				writefile(file, bone.first);
				writefile(file, bone.second);
			}
		}

	}
	file.close();
}
void Serializable::ReadModel(Model *m)
{
	ifstream file;
	file.open(m->fullpath, ios::binary | ios::in);
	if (!file)
		throw std::exception("File error when saving a model.");
	unsigned int meshcount;

	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	
	ModelType t;

	Serializable::readfile(file, &meshcount);
	Serializable::readfile(file, &t);
	m->numOfMeshes = 0;
	for (int i = 0; i < meshcount; i++)
	{
		
		Serializable::readfile(file, &numberOfVertices);
		float *vertices = (float *)malloc(numberOfVertices * 3 * sizeof(float));
		float *normals = (float *)malloc(numberOfVertices * 3 * sizeof(float));
		float *textureCoords = (float *)malloc(numberOfVertices * 2 * sizeof(float));
		Serializable::readfile(file, vertices, numberOfVertices * 3);
		Serializable::readfile(file, normals, numberOfVertices * 3);
		Serializable::readfile(file, textureCoords, numberOfVertices * 2);
		Serializable::readfile(file, &numberOfIndices);
		unsigned int *indices = (unsigned int *)malloc(numberOfIndices * 3 * sizeof(unsigned int));
		Serializable::readfile(file, indices,numberOfIndices * 3);
		Mesh *mesh;
		
		Mesh::boundstype b;
		Serializable::readfile(file, &b);
		
	
	
		
		if (t == ModelType::Skinned)
		{
			std::vector<float> weights(numberOfVertices * SKINNED_MESH_MAX_WEIGHT_PER_VERTICES);
			decltype(SkinnedMesh::boneids) boneids(numberOfVertices * SKINNED_MESH_MAX_WEIGHT_PER_VERTICES);
			Serializable::readfile(file, &weights.data()[0], SKINNED_MESH_MAX_WEIGHT_PER_VERTICES * numberOfVertices);
			Serializable::readfile(file, &boneids.data()[0], SKINNED_MESH_MAX_WEIGHT_PER_VERTICES * numberOfVertices);
			size_t bonesize;
			Serializable::readfile(file, &bonesize);
			BonesList bones(bonesize);
			for (int j = 0; j < bonesize; j++)
			{
				std::string bonename;
				glm::mat4 offset;
				Serializable::readfile(file, &bonename);
				Serializable::readfile(file, &offset);
				bones[j] = std::make_pair(bonename, offset);
			}
			mesh = (Mesh *)new SkinnedMesh(numberOfVertices, numberOfIndices, vertices, normals, indices, textureCoords,weights, boneids,bones);
		}
			
		else
			mesh = new Mesh(numberOfVertices, numberOfIndices, vertices, normals, indices, textureCoords);
		m->addMesh(mesh);
		mesh->bounds = b;
	}
	
	file.close();
}



template<class T>
inline void Serializable::LoadComponent(ProjectManager *pm, ifstream & file, Actor *a)
{
	int t;
	readfile(file, &t);
	if (t != 0)
		return _LoadComponent<T>(pm,file, a);


}

template<class T>
inline void Serializable::_LoadComponent(ProjectManager *, ifstream & file, Actor *a){}


template<> inline void Serializable::_LoadComponent<ModelComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	int t;
	readfile(file, &t);
	if (t != 0)
	{
		string path,name;
		readfile(file, &name);
		readfile(file, &path);
		auto x = a->AddComponent<ModelComponent>(GE_Engine->resourceManager->getResource<Model>(path,name));
		LoadIModelComponentMaterials(pm, file, x);

	}
}
template<> inline void Serializable::_LoadComponent<SkinnedModelComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	int t;
	readfile(file, &t);
	if (t != 0)
	{
		string path, name;
		readfile(file, &name);
		readfile(file, &path);
		auto x = a->AddComponent<SkinnedModelComponent>(GE_Engine->resourceManager->getResource<Model>(path, name));
		LoadIModelComponentMaterials(pm, file, x);
		readfile(file, &x->rootBone);
	}
}
template<> inline void Serializable::_LoadComponent<LightComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<LightComponent>();
	readfile(file, &x->lightType);
	readfile(file, &x->color);
	readfile(file, &x->angle);
	readfile(file, &x->distance);
	readfile(file, &x->intensity);
	readfile(file, &x->constant);
	readfile(file, &x->linear);
	readfile(file, &x->quadratic);
}
template<> inline void Serializable::_LoadComponent<AnimatorComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<AnimatorComponent>();
	
}
template<> inline void Serializable::_LoadComponent<SphereColliderComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<SphereColliderComponent>();
	readfile(file, &x->geometry);
}
template<> inline void Serializable::_LoadComponent<CubeColliderComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<CubeColliderComponent>();
	readfile(file, &x->geometry);
}
template<> inline void Serializable::_LoadComponent<CapsuleColliderComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<CapsuleColliderComponent>();
	readfile(file, &x->up);
	readfile(file, &x->upp);
	readfile(file, &x->geometry);
}
template<> inline void Serializable::_LoadComponent<RigidBodyComponent>(ProjectManager *pm, ifstream & file, Actor *a)
{
	auto x = a->AddComponent<RigidBodyComponent>();

}
template<class T>
inline void Serializable::SaveComponent(ProjectManager *pm, ofstream & file, T * component)
{
	if (component)
	{
		writefile(file, 1);
		_SaveComponent(pm,file, component);
	}
	else
		writefile(file, 0);
}

template<class T>
inline void Serializable::_SaveComponent(ProjectManager *, ofstream & file, T * component) {}



template<> inline void Serializable::_SaveComponent<ModelComponent>(ProjectManager *pm, ofstream & file, ModelComponent * component)
{
	if (auto model = component->getModel())
	{
		writefile(file, 1);
		SaveIModelComponent(pm,file, component);
	}
	else
		writefile(file, 0);
}
template<> inline void Serializable::_SaveComponent<SkinnedModelComponent>(ProjectManager *pm, ofstream & file, SkinnedModelComponent * component)
{
	if (auto model = component->getModel())
	{
		writefile(file, 1);
		SaveIModelComponent(pm, file, component);
		writefile(file, component->rootBone);
	}
	else
		writefile(file, 0);
}
template<> inline void Serializable::_SaveComponent<LightComponent>(ProjectManager *pm, ofstream & file, LightComponent * component)
{
	writefile(file,component->lightType);
	writefile(file,component->color	);
	writefile(file,component->angle	);
	writefile(file,component->distance	);
	writefile(file,component->intensity);
	writefile(file,component->constant);
	writefile(file,component->linear);
	writefile(file,component->quadratic);
}
template<> inline void Serializable::_SaveComponent<AnimatorComponent>(ProjectManager *pm, ofstream & file, AnimatorComponent * component)
{
}
template<> inline void Serializable::_SaveComponent<SphereColliderComponent>(ProjectManager *pm, ofstream & file, SphereColliderComponent * component)
{
	writefile(file, component->geometry);
}
template<> inline void Serializable::_SaveComponent<CubeColliderComponent>(ProjectManager *pm, ofstream & file, CubeColliderComponent * component)
{
	writefile(file, component->geometry);
}
template<> inline void Serializable::_SaveComponent<CapsuleColliderComponent>(ProjectManager *pm, ofstream & file, CapsuleColliderComponent * component)
{
	writefile(file, component->up);
	writefile(file, component->upp);
	writefile(file, component->geometry);
}
template<> inline void Serializable::_SaveComponent<RigidBodyComponent>(ProjectManager *pm, ofstream & file, RigidBodyComponent * component)
{
}

void Serializable::SaveIModelComponent(ProjectManager *pm, ofstream & file, IModelComponent * component)
{
	auto model = component->getModel();
	writefile(file, model->name);
	writefile(file, model->fullpath);
	writefile(file, component->numberOfMaterials);
	for (int i = 0; i < component->numberOfMaterials; i++)
	{
		writefile(file, pm->findIndexofMaterial(component->materials[i]));
	}
}
void Serializable::LoadIModelComponentMaterials(ProjectManager *pm, ifstream & file, IModelComponent * component)
{
	readfile(file, &component->numberOfMaterials);
	for (int i = 0; i < component->numberOfMaterials; i++)
	{
		int index;
		readfile(file, &index);
		component->materials[i] = (index!= -1)  ? pm->materials[index] : ConstantMaterials::Materials::defaultMaterial;
	}
}

