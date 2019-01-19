#pragma once
ref class Shader
{
public:
	Shader(const char* , const char*);
	void Use();
private:
	unsigned int programID;
};

