#include "CameraComponent.h"

#include<engine/ActorManager.h>


CameraComponent::CameraComponent() : Component()
{
	m_near = 0.01f;
	m_far = 1000.f;
	fov = 60.f;
	size = 5.f;
}


CameraComponent::~CameraComponent()
{
}

void CameraComponent::updateViewMatrix()
{
	Transform *transform = &GE_Engine->actorManager->GetActor(this->owner)->transformation;
	viewMatrix = glm::lookAt(transform->getWorldPosition(), transform->getWorldPosition() + transform->getWorldForwardVector(), transform->getWorldUpVector());
}

void CameraComponent::updateprojectionMatrix()
{
	if (Perspective)
		projectionMatrix = glm::perspective(glm::radians(fov), (float)(GE_Engine->screenSize.x / GE_Engine->screenSize.y), m_near, m_far);
	else
		projectionMatrix = glm::ortho(-size, size,-size, size, m_near, m_far);
		
}
