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
		if (animator.second->currentAnimation)
		{
			//loopHierarchy(animator.first);
		}
		
	}
}

void AnimationSystem::PostUpdate()
{

}

void AnimationSystem::loopHierarchy(ActorID aid)
{
	/*Actor *a = GE_Engine->actorManager.
	a->transformation->position.x += 1;
	a->RecalculateRealMatrix();
	for (int i = 0; i < a->numberOfChildren; i++)
	{
		loopHierarchy(a->children[i]);
	}*/
}
