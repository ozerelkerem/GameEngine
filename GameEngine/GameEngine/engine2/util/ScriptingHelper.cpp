#include "ScriptingHelper.h"
#include <Engine.h>
#include <engine/Transform.h>
#include<engine/InputManager.h>
#include<engine/Actor.h>
#include<Windows.h>
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
		if constexpr (std::is_same<ClassOf<Setter>::Type, Transform>::value)
		{
			cls->applyPhysic();
		}
	}

	template<typename Getter, Getter getter>
	typename TypeOf<Getter>::Type csharp_getProperty(typename ClassOf<Getter>::Type* cls)
	{
		return cls->*getter;
	}

	template <typename F> struct csharp_methodproxy;
	template <typename F> struct csharp_functionproxy;

	template<typename R, typename... Args>
	struct csharp_functionproxy<R(Args...)>
	{
		using F = R (Args...);

		template<F fnc>
		static typename R call(Args ...args ){
			return fnc(args...);
		}
	};
	
	

	


	
	
	void moveMainAssembly()
	{
		std::string from,to;
		from = PROJECTDIRECTORY;
		from += "\\engineassembly.dll";
		to = GE_Engine->mainPath;
		to += "dlls\\engineassembly.dll";
		std::filesystem::copy_file(from,to,std::filesystem::copy_options::overwrite_existing);

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

		file << "namespace GameEngine{\n\n"
				 "\tpublic class " + s->name + " : ScriptComponent {\n\n"
				 "\t\tpublic void Start(){\n"
				 "\t\t}\n"
				 "\t\tpublic void Update(){\n"
				 "\t\t}\n"
				 "\n\t}\n\n}";

		file.close();
	}

	void openCSProject()
	{
		ShellExecute(NULL,"open",("\""+ std::string(GE_Engine->mainPath) + "scripts\\project.csproj\"").c_str(),NULL,NULL,SW_NORMAL);

	}

	void createCSProject(ProjectManager *m)
	{
		std::ofstream file;
		file.open(std::string(GE_Engine->mainPath) + "scripts\\project.csproj", std::ios::out);
		if (!file)
			throw std::exception("File error when cs project.");
		file <<
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n<Project ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
			"<Import Project=\"$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props\" Condition=\"Exists('$(MSBuildExtensionsPath)\\$(MSBuildToolsVersion)\\Microsoft.Common.props')\" />\r\n  \r\n  "
			"<ItemGroup>\r\n   "
			" <Reference Include=\"GameEngineAssembly\">\r\n      <HintPath>" + std::string(GE_Engine->mainPath) + "dlls\\engineassembly.dll" + "</HintPath>\r\n    </Reference>\r\n  "
			"</ItemGroup>\r\n  <ItemGroup>\r\n   "
			+ getfilelist(m) +
			" </ItemGroup>\r\n\r\n  <Import Project=\"$(MSBuildToolsPath)\\Microsoft.CSharp.targets\" />\r\n</Project>";
	
		file.close();


	}

	std::string getfilelist(ProjectManager * m)
	{
		std::string returnstr;
		for (auto script : m->scripts)
		{
			returnstr += "  <Compile Include=\"" + script->name + ".cs\" />";
		}
		
		return returnstr;
	}

	void loadMethods()
	{
		{
			auto setter = &csharp_setProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			auto getter = &csharp_getProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			mono_add_internal_call("GameEngine.Transform::setLocalPosition", setter);
			mono_add_internal_call("GameEngine.Transform::getLocalPosition", getter);
		}
		{
			auto setter = &csharp_setProperty<decltype(&Transform::localScale), &Transform::localScale>;
			auto getter = &csharp_getProperty<decltype(&Transform::localScale), &Transform::localScale>;
			mono_add_internal_call("GameEngine.Transform::setLocalScale", setter);
			mono_add_internal_call("GameEngine.Transform::getLocalScale", getter);
		}
		{
			
			
			auto x = &csharp_functionproxy<decltype(InputManager::getKeyState)>::call<&InputManager::getKeyState>;
			mono_add_internal_call("GameEngine.InputManager::getKeyState", x);
		}

	}

	

	

}