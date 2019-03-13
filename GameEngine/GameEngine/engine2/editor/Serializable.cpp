#include "Serializable.h"

void Serializable::Save(ProjectManager *pm, const char *path)
{
	SaveProjectFile(pm, path);
}

void Serializable::SaveProjectFile(ProjectManager *pm, const char *path)
{
	ofstream out;
	out.open(std::string(path) + pm->name, ios::out | ios::binary);
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
	_mkdir((pm->path + "/models/" + m->name + ".model").c_str());
	file.open(pm->path + "/models/" + m->name + ".model", ios::binary | ios::out);
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
	}
}

