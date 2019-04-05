#pragma once
#include<Api.h>
#include<engine/scripting/Script.h>
#include<fstream>
#include<filesystem>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>
#include <mono/utils/mono-logger.h>
#include<mono/metadata/debug-helpers.h>
namespace ScriptHelper{
	
		 void moveMainAssembly();
		 void compile();
		 void createScript(Script *s);
	

}