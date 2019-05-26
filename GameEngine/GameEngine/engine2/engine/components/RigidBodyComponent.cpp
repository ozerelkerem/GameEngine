#include "RigidBodyComponent.h"
#include<engine/physx/PhysicSystem.h>


RigidBodyComponent::RigidBodyComponent()
{
}


RigidBodyComponent::~RigidBodyComponent()
{
	GE_Engine->physicSystem->removeComponent(this);
}


void RigidBodyComponent::updateFlags()
{
	{
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, LLX);
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, LLY);
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, LLZ);
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, LRX);
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, LRY);
		pxactor->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, LRZ);

		pxactor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, Kinematic);
		pxactor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !Gravity);

	}
}

void RigidBodyComponent::clearForces()
{
	pxactor->clearForce();
	pxactor->clearTorque();
	pxactor->setLinearVelocity(PxVec3(0));
	pxactor->setAngularVelocity(PxVec3(0));
}

void RigidBodyComponent::addForce(PxVec3 forceVec)
{
	pxactor->addForce(forceVec);
}

void RigidBodyComponent::addTorque(PxVec3 torqueVec)
{
	pxactor->addTorque(torqueVec);
}

void RigidBodyComponent::clearForce()
{
	pxactor->clearForce();
}

void RigidBodyComponent::clearTorque()
{
	pxactor->clearTorque();
}

PxReal RigidBodyComponent::getAngularDamping()
{
	return pxactor->getAngularDamping();
}

PxVec3 RigidBodyComponent::getAngularVelocity()
{
	return pxactor->getAngularVelocity();
}


PxReal RigidBodyComponent::getLinearDamping()
{
	return pxactor->getLinearDamping();
}

PxVec3 RigidBodyComponent::getLinearVelocity()
{
	return pxactor->getLinearVelocity();
}

PxReal RigidBodyComponent::getMass()
{
	return pxactor->getMass();
}

void RigidBodyComponent::setAngularDamping(PxReal angDamping)
{
	pxactor->setAngularDamping(angDamping);
}

void RigidBodyComponent::setAngularVelocity(PxVec3 angVelocity)
{
	pxactor->setAngularVelocity(angVelocity);
}

void RigidBodyComponent::setLinearDamping(PxReal linDamping)
{
	pxactor->setLinearDamping(linDamping);
}

void RigidBodyComponent::setLinearVelocity(PxVec3 linVelocity)
{
	pxactor->setLinearVelocity(linVelocity);
}

void RigidBodyComponent::setMass(PxReal mass)
{
	pxactor->setMass(mass);
}

void RigidBodyComponent::setMaxLinearVelocity(PxReal maxLinVelocity)
{
	pxactor->setMaxLinearVelocity(maxLinVelocity);
}

void RigidBodyComponent::setMaxAngularVelocity(PxReal maxAngVelocity)
{
	pxactor->setMaxAngularVelocity(maxAngVelocity);
}

PxReal RigidBodyComponent::getMaxLinearVelocity()
{
	return pxactor->getMaxLinearVelocity();
}

PxReal RigidBodyComponent::getMaxAngularVelocity()
{
	return pxactor->getMaxAngularVelocity();
}