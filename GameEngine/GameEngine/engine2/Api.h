#pragma once
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <memory>

#include <string>
#include <math.h>

#include <chrono>
#include <util/Handle.h>

using TypeID = uint16_t;

using ActorID = GameEngine::Util::Handle64;

#define SKINNED_MESH_MAX_WEIGHT_PER_VERTICES 4

class Shader;
extern Shader *normalShader;
extern Shader *colorShader;




class Engine;

extern Engine*		GE_Engine;

void	Initialize();
void	Terminate();