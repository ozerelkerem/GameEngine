#pragma once
#include <physx/PxPhysicsAPI.h>
#include <glm/glm.hpp>
using namespace physx;

PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4);


