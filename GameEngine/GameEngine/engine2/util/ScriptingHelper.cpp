#include "ScriptingHelper.h"
#include <Engine.h>
#include <engine/Transform.h>
#include<engine/Actor.h>
namespace ScriptHelper {



	template <typename T> struct ClassOf;
	template <typename R, typename C, typename... Args> struct ClassOf<R(C::*)(Args...)> { using Type = C; };
	template <typename R, typename C, typename... Args> struct ClassOf<R(C::*)(Args...)const > { using Type = C; };
	template <typename R, typename C> struct ClassOf<R(C::*)> { using Type = C; };

	template <typename T> struct TypeOf;
	template <typename R, typename C, typename... Args> struct TypeOf<R(C::*)(Args...)> { using Type = R; };
	template <typename R, typename C, typename... Args> struct TypeOf<R(C::*)(Args...)const > { using Type = R; };
	template <typename R, typename C> struct TypeOf<R(C::*)> { using Type = R; };

	template<typename Setter, Setter setter>
	void csharp_setProperty(typename ClassOf<Setter>::Type* cls, typename TypeOf<Setter>::Type val)
	{
		cls->*setter = val;
	}

	template<typename Getter, Getter getter>
	typename TypeOf<Getter>::Type csharp_getProperty(typename ClassOf<Getter>::Type* cls)
	{
		return cls->*getter;
	}

	
	
	void moveMainAssembly()
	{
		std::string from,to;
		from = PROJECTDIRECTORY;
		from += "\\engineassembly.dll";
		to = GE_Engine->mainPath;
		to += "dlls\\engineassembly.dll";
		std::filesystem::copy_file(from,to);

		decltype(&Actor::transformation) a;
	}

	

	void compile()
	{
		std::string buildstring;
		buildstring = "C:\\\"Program Files (x86)\"\\Mono\\bin\\mcs.bat -reference:\"";
		buildstring += GE_Engine->mainPath;
		buildstring += "dlls\\engineassembly.dll\" -out:\"";
		buildstring += GE_Engine->mainPath;
		buildstring += "dlls\\scriptassembly.dll\" -target:library -debug -unsafe -recurse:\"";
		buildstring += GE_Engine->mainPath;
		buildstring += "\\scripts\\*.cs\"";


		system(buildstring.c_str());
	}
	void createScript(Script * s)
	{
		std::ofstream file;
		file.open(std::string(GE_Engine->mainPath) + "scripts\\" + s->name + ".cs", std::ios::out);
		if (!file)
			throw std::exception("File error when scripting.");
		file << "namespace GameEngine{\n\n";
		file << "\tpublic class " << s->name << "{\n\n";
		file << "\t\tpublic void Start(){\n";
		file << "\t\t}\n";
		file << "\t\tpublic void Update(){\n";
		file << "\t\t}\n";
		file << "\n\t}\n\n}";

		
	}

	void loadMethods()
	{
		{
			auto setter = &csharp_setProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			auto getter = &csharp_getProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			mono_add_internal_call("GameEngine.Transform::setLocalPosition", setter);
			mono_add_internal_call("GameEngine.Transform::getLocalPosition", getter);
		}
	}

	

	

}