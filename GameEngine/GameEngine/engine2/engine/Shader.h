#pragma once

#include "..//ogl/glm/glm.hpp"

#include <string>

class Shader
{

public:
	Shader(const char* , const char*);
	void Use();
	unsigned int programID;
	void setFloat(const std::string &, const float &);
	void setVec3(const std::string &, const glm::vec3 &);
	void setVec3(const std::string & name, const float v1, const float v2, const float v3);
	void setMat4(const std::string &, const glm::mat4 &);

	unsigned int getProgramID();
};







