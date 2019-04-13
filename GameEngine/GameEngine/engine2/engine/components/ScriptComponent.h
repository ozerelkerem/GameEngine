#pragma once

#include <engine/components/Component.h>
#include <engine/scripting/Script.h>
#include <mono/metadata/object.h>

class ScriptComponent : public Component <ScriptComponent>
{
public:
	ScriptComponent();
	~ScriptComponent();

	std::vector<Script *> scripts;
	std::vector<MonoObject *> objects;
	void add(Script *s) { scripts.push_back(s); objects.resize(scripts.size()); };
	void remove(Script *s) {scripts.erase(std::remove_if(scripts.begin(), scripts.end(), [s](auto script) {return s== script; })); };

};

