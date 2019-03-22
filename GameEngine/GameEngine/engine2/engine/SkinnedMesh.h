#pragma once
#include <engine/Mesh.h>

#include <engine/Shader.h>


using BoneWeightVector = std::vector<std::vector<float>>;
using BonesList = std::vector<std::pair<std::string, glm::mat4>>;
using BoneIdType = float;
class SkinnedMesh : public Mesh
{
public:


	std::vector<float> weights;
	std::vector<BoneIdType> boneids;
	BonesList bones;

	SkinnedMesh(unsigned int, unsigned int, float *, float *, unsigned int *, float *, std::vector<float>, std::vector<BoneIdType> boneids, BonesList);
	~SkinnedMesh();

	virtual void loadMesh();



	virtual inline void bind() {
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		//normalShader->setInt("isSkinned",1);
	};
	virtual inline void unbind() {
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glBindVertexArray(0);
	};

	virtual inline  void freeMesh() {
		free(vertices);
		free(normals);
		free(indices);
		free(textureCoords);
		std::vector<float>().swap(weights);
			
		
	};
};

