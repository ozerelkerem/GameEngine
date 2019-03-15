#include "Serializable.h"

void Serializable::Save(ProjectManager *pm, const char *path)
{
	SaveProjectFile(pm, path);
}

void Serializable::SaveProjectFile(ProjectManager *pm, const char *path)
{
	ofstream out;
	out.open(std::string(path) + pm->name + ".project", ios::out | ios::binary);
	if (!out)
		throw std::exception("File error when saving.");
	else
	{
		//project name
		Serializable::writefile(out, pm->name);
		WriteTextures(out, pm);
		WriteMaterials(out, pm);
		WriteModels(out, pm);
	}
	out.close();
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

void Serializable::WriteTextures(ofstream& file, ProjectManager *pm)
{
	Serializable::writefile(file, pm->textures.size());
	for (auto texture : pm->textures)
	{
		Serializable::writefile(file, texture->name);
		Serializable::writefile(file, texture->m_path);
	}
}

void Serializable::WriteModels(ofstream & file, ProjectManager * pm)
{
	Serializable::writefile(file, pm->models.size());
	for (auto model : pm->models)
	{
		Serializable::writefile(file, model->name);
		Serializable::writefile(file, pm->path + "/models/" + model->name + ".model");
	}
}

void Serializable::SaveModel(ProjectManager *pm, Model *m)
{
	ofstream file;
	file.open(pm->path + "models\\" + m->name + ".model", ios::binary | ios::out);
	if (!file)
		throw std::exception("File error when saving a model.");
	Serializable::writefile(file, m->numOfMeshes);
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
	}
	file.close();
}
void Serializable::ReadModel(Model *m)
{
	ifstream file;
	file.open(m->path, ios::binary | ios::in);
	if (!file)
		throw std::exception("File error when saving a model.");
	unsigned int meshcount;

	unsigned int numberOfVertices;
	unsigned int numberOfIndices;
	

	Serializable::readfile(file, &meshcount);
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
	
		Mesh *mesh = new Mesh(numberOfVertices, numberOfIndices, vertices, normals, indices, textureCoords);
		Serializable::readfile(file, &mesh->bounds);
		m->addMesh(mesh);
	}
	
	file.close();
}

