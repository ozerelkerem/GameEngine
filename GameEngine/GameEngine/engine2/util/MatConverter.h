#pragma once
#include <physx/PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
using namespace physx;

PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4);
void PhysXMat4ToglmMat4(const PxMat44& mat4, glm::mat4& newMat);
PxVec3 glmVec3ToPhysXVec3(const glm::vec3& vec3);
PxQuat glmQuatToPhysXQuat(const glm::quat& quat);



