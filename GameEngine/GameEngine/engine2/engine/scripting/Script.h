#pragma once
#include <engine/Namable.h>
#include <mono/metadata/assembly.h>
class Script : public Namable
{
public:
	Script(std::string name);
	~Script();

	MonoMethod *StartMethod;
	MonoMethod *UpdateMethod;
	MonoMethod *OnCollisionEnterMethod;
	
};

