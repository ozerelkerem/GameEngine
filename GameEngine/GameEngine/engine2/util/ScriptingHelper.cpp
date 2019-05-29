#include "ScriptingHelper.h"
#include <Engine.h>
#include <engine/Transform.h>
#include<engine/InputManager.h>
#include<editor/ProjectManager.h>
#include<engine/Actor.h>
#include<Windows.h>
#include<engine/ActorManager.h>

#include<engine/components/RigidBodyComponent.h>

namespace ScriptHelper {

	template<typename T> struct CSharpTypeConvertor
	{
		using Type = T;

		static Type convert(Type val) { return val; }
		static Type convertRet(Type val) { return val; }
	};
	template<> struct CSharpTypeConvertor<const char*>
	{
		using Type = MonoString * ;

		static const char* convert(MonoString* val) { return mono_string_to_utf8(val); }
		static MonoString* convertRet(const char* val) { return mono_string_new(mono_domain_get(), val); }
	};

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
		static typename R call(typename CSharpTypeConvertor<Args>::Type... args ){
			return CSharpTypeConvertor<R>::convert(fnc(CSharpTypeConvertor<Args>::convert(args)...));
		}
	};
	template<typename... Args>
	struct csharp_functionproxy<void(Args...)>
	{
		using F = void(Args...);
		template<F fnc>
		static void call(typename CSharpTypeConvertor<Args>::Type... args) {
			fnc(CSharpTypeConvertor<Args>::convert(args)...);
		}
	};

	template<typename R, typename T,typename... Args>
	struct csharp_methodproxy<R(T::*)(Args...)>
	{
		using F = R(T::*)(Args...);

		template<F fnc>
		static typename R call(T *ins,typename CSharpTypeConvertor<Args>::Type... args) {
			return CSharpTypeConvertor<R>::convert((ins->*fnc)(CSharpTypeConvertor<Args>::convert(args)...));
		}
	};
	template<typename T, typename... Args>
	struct csharp_methodproxy<void(T::*)(Args...)>
	{
		using F = void(T::*)(Args...);

		template<F fnc>
		static void call(T *ins, typename CSharpTypeConvertor<Args>::Type... args) {
			(ins->*fnc)(CSharpTypeConvertor<Args>::convert(args)...);
		}
	};	
	
	void moveMainAssembly()
	{
		std::string from = "",to;

		ifstream file("settings.txt");

		if (!file.is_open() || from.empty())
		{
			printf("Put engine directory into 'settings.txt'.");
			system("PAUSE");
			exit(EXIT_FAILURE);
		}

		getline(file, from);

		file.close();

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
				 "\t\tpublic void Start()\n\t\t{\n\n"
				 "\t\t}\n\n"
				 "\t\tpublic void Update()\n\t\t{\n\n"
				 "\t\t}\n\n"
				"\t\tpublic void onCollisionEnter(Actor other)\n\t\t{\n\n"
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


	void * hasComponent(Actor *actor, const char *name)
	{
		if (strcmp(name,"animatorcomponent")==0)
			return actor->GetComponent<AnimatorComponent>();
		else if (strcmp(name, "rigidbodycomponent") == 0)
			return actor->GetComponent<RigidBodyComponent>();
	}


	void loadMethods()
	{
		{
			auto x = &csharp_methodproxy<decltype(&Transform::applyToRigidBody)>::call<&Transform::applyToRigidBody>;
			mono_add_internal_call("GameEngine.Transform::applyToRigidBody", x);
		}
		{
			auto setter = &csharp_setProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			auto getter = &csharp_getProperty<decltype(&Transform::localPosition), &Transform::localPosition>;
			mono_add_internal_call("GameEngine.Transform::setLocalPosition", setter);
			mono_add_internal_call("GameEngine.Transform::getLocalPosition", getter);
		}
		{
			auto setter = &csharp_setProperty<decltype(&Transform::localeulerRotation), &Transform::localeulerRotation>;
			auto getter = &csharp_getProperty<decltype(&Transform::localeulerRotation), &Transform::localeulerRotation>;
			mono_add_internal_call("GameEngine.Transform::setLocalEulerRotation", setter);
			mono_add_internal_call("GameEngine.Transform::getLocalEulerRotation", getter);
		}
		{
			auto x = &csharp_methodproxy<decltype(&Transform::getWorldForwardVector)>::call<&Transform::getWorldForwardVector>;
			mono_add_internal_call("GameEngine.Transform::getForwardVector", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&Transform::getWorldUpVector)>::call<&Transform::getWorldUpVector>;
			mono_add_internal_call("GameEngine.Transform::getUpVector", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&Transform::getWorldRightVector)>::call<&Transform::getWorldRightVector>;
			mono_add_internal_call("GameEngine.Transform::getRightVector", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&Transform::calcQuatFromEuler)>::call<&Transform::calcQuatFromEuler>;
			mono_add_internal_call("GameEngine.Transform::calcQuatFromEuler", x);
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
		{
			auto x = &csharp_functionproxy<decltype(InputManager::getMouseButtonState)>::call<&InputManager::getMouseButtonState>;
			mono_add_internal_call("GameEngine.InputManager::getMouseButtonState", x);
		}
		{
			auto x = &csharp_functionproxy<decltype(InputManager::getMouseDeltaPosition)>::call<&InputManager::getMouseDeltaPosition>;
			mono_add_internal_call("GameEngine.InputManager::getMouseDeltaPosition", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&ProjectManager::getAnimationByName)>::call<&ProjectManager::getAnimationByName>;
			mono_add_internal_call("GameEngine.Animation::getAnimationByName", x);
		}
		{
			auto x = &csharp_functionproxy<decltype(hasComponent)>::call<&hasComponent>;
			mono_add_internal_call("GameEngine.Component::hasComponent", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&AnimatorComponent::PlayLoop)>::call<&AnimatorComponent::PlayLoop>;
			mono_add_internal_call("GameEngine.AnimatorComponent::playLoop", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&AnimatorComponent::PlayOnce)>::call<&AnimatorComponent::PlayOnce>;
			mono_add_internal_call("GameEngine.AnimatorComponent::playOnce", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&AnimatorComponent::Stop)>::call<&AnimatorComponent::Stop>;
			mono_add_internal_call("GameEngine.AnimatorComponent::stop", x);
		}
		{
			auto getter = &csharp_getProperty<decltype(&AnimatorComponent::state), &AnimatorComponent::state>;
			mono_add_internal_call("GameEngine.AnimatorComponent::getState", getter);
		}
		//Game Managment
		{
			
			auto x = &csharp_methodproxy<decltype(&Actor::RemoveActor)>::call<&Actor::RemoveActor>;
			mono_add_internal_call("GameEngine.GameManager::destroyActor", x);	
		}
		{			
			auto x = &csharp_functionproxy<decltype(Serializable::AddPrefabCS)>::call<&Serializable::AddPrefabCS>;
			mono_add_internal_call("GameEngine.GameManager::spawnActor", x);

		}

		//physx
		{	
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::addForce)>::call<&RigidBodyComponent::addForce>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::addForce", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::addTorque)>::call<&RigidBodyComponent::addTorque>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::addTorque", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::clearForce)>::call<&RigidBodyComponent::clearForce>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::clearForce", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::clearTorque)>::call<&RigidBodyComponent::clearTorque>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::clearTorque", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getAngularDamping)>::call<&RigidBodyComponent::getAngularDamping>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getAngularDamping", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getAngularVelocity)>::call<&RigidBodyComponent::getAngularVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getAngularVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getLinearDamping)>::call<&RigidBodyComponent::getLinearDamping>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getLinearDamping", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getLinearVelocity)>::call<&RigidBodyComponent::getLinearVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getLinearVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getMass)>::call<&RigidBodyComponent::getMass>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getMass", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setAngularDamping)>::call<&RigidBodyComponent::setAngularDamping>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setAngularDamping", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setAngularVelocity)>::call<&RigidBodyComponent::setAngularVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setAngularVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setLinearDamping)>::call<&RigidBodyComponent::setLinearDamping>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setLinearDamping", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setLinearVelocity)>::call<&RigidBodyComponent::setLinearVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setLinearVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setMass)>::call<&RigidBodyComponent::setMass>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setMass", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setMaxLinearVelocity)>::call<&RigidBodyComponent::setMaxLinearVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setMaxLinearVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::setMaxAngularVelocity)>::call<&RigidBodyComponent::setMaxAngularVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::setMaxAngularVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getMaxLinearVelocity)>::call<&RigidBodyComponent::getMaxLinearVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getMaxLinearVelocity", x);
		}
		{
			auto x = &csharp_methodproxy<decltype(&RigidBodyComponent::getMaxAngularVelocity)>::call<&RigidBodyComponent::getMaxAngularVelocity>;
			mono_add_internal_call("GameEngine.RigidBodyComponent::getMaxAngularVelocity", x);
		}




	}

	

	

	

}