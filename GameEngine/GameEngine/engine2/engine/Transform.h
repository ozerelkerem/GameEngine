#pragma once
#include<Api.h>
#include "../ogl/glm/glm.hpp"
#include "../ogl/glm/gtx/euler_angles.hpp"
#include "../ogl/glm/gtx/quaternion.hpp"
#include "../ogl/glm/gtx/transform.hpp"
#include <physx/PxPhysicsAPI.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include<util/MatConverter.h>



class Transform
{
public:
	Transform();
	~Transform();
	void decomposeLocalMatrix();

	glm::vec3 localPosition;
	glm::quat localquatRotation;
	glm::vec3 localeulerRotation;
	glm::vec3 localScale;
	

	void calcEulerFromQuat();
	void calcQuatFromEuler();

	glm::vec3 getWorldPosition();
	glm::vec3 getWorldScale();
	glm::quat getWorldRotation();

	glm::vec3 getWorldUpVector();
	glm::vec3 getWorldRightVector();
	glm::vec3 getWorldForwardVector();

	//only pos and rot
	glm::mat4 getWorldPose();


	void applyToRigidBody();
	void calcLocalMatrix();

	Transform *parent;
	physx::PxRigidActor *physicactor;

	inline void applyPhysic() {
		if (physicactor)
			physicactor->setGlobalPose(PxTransform(glmVec3ToPhysXVec3(getWorldPosition()), glmQuatToPhysXQuat(getWorldRotation())));
	}

	inline void setLocalMatrix(glm::mat4 &mat) { 
		localMatrix = mat;
		decomposeLocalMatrix();
		worldMatrix = parent->worldMatrix * localMatrix;
		applyPhysic();
	};
	inline void setWorldMatrix(glm::mat4 &mat) {
		// Parent * Local = World => Local = Parent(^-1) * World
		glm::mat4 parentt = glm::inverse(parent->worldMatrix);
		worldMatrix = mat;
		localMatrix = parentt * worldMatrix;
		decomposeLocalMatrix();
		applyPhysic();
	};
	inline void setLocalMatrix(glm::mat4 mat) {
		localMatrix = mat;
		decomposeLocalMatrix();
		worldMatrix = parent->worldMatrix * localMatrix;
		applyPhysic();
	};
	inline void setWorldMatrix(glm::mat4 mat) {
		// Parent * Local = World => Local = Parent(^-1) * World
		glm::mat4 parentt = glm::inverse(parent->worldMatrix);
		worldMatrix = mat;
		localMatrix = parentt * worldMatrix;
		decomposeLocalMatrix();
		applyPhysic();
	};
	inline glm::mat4& getLocalMatrix() { return localMatrix; };
	inline glm::mat4& getWorldMatrix() { return worldMatrix; };
private:
	glm::mat4 localMatrix;
	glm::mat4 worldMatrix;
};

