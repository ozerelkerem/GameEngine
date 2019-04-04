#include "ScriptingHelper.h"
#include <Engine.h>
#include <engine/Transform.h>
namespace ScriptHelper {




	template <typename T> struct ClassOf;
	template <typename R, typename C, typename... Args> struct ClassOf<R(C::*)(Args...)> { using Type = C; };
	template <typename R, typename C, typename... Args> struct ClassOf<R(C::*)(Args...)const > { using Type = C; };
	template <typename R, typename C> struct ClassOf<R(C::*)> { using Type = C; };

	template <typename Setter, Setter setter>
	void csharp_setProperty(typename ClassOf<Setter>::Type* scene, int cmp)
	{
		//(scene->*setter)({ cmp }, fromCSharpValue(value));
	}


	void moveMainAssembly()
	{
		std::string from,to;
		from = PROJECTDIRECTORY;
		from += "\\engineassembly.dll";
		to = GE_Engine->mainPath;
		to += "dlls\\engineassembly.dll";
		std::filesystem::copy_file(from,to);
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

		auto x = &csharp_setProperty<decltype(&Transform::setLocalMatrix), &Transform::setLocalMatrix>;

		
	}

	

}