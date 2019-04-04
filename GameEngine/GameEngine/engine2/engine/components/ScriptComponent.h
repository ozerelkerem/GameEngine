#pragma once

#include <engine/components/Component.h>
#include <engine/scripting/Script.h>

class ScriptComponent : public Component <ScriptComponent>
{
public:
	ScriptComponent();
	~ScriptComponent();

	std::vector<Script *> scripts;
	void add(Script *s) { scripts.push_back(s); };
	void remove(Script *s) {scripts.erase(std::remove_if(scripts.begin(), scripts.end(), [s](auto script) {return s== script; })); };

};

