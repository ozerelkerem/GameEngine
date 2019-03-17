#include "System.h"
#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(GameBase *gb) : gamebase(gb) {}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::PreUpdate()
{

}

void AnimationSystem::Update()
{
	auto animators = gamebase->currentScene->componentSystem->GetComponents<AnimatorComponent>();
	for (auto animator : animators)
	{
		loopHierarchy(animator.first);
	}
}

void AnimationSystem::PostUpdate()
{

}

void AnimationSystem::loopHierarchy(Actor *a)
{
	a->transformation->position.x += 1;
	a->RecalculateRealMatrix();
	for (int i = 0; i < a->numberOfChildren; i++)
	{
		loopHierarchy(a->children[i]);
	}
}
