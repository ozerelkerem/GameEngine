#include "System.h"
#include "AnimationSystem.h"
#include <engine/ActorManager.h>
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
			for (auto effect : animator.second->effectlist)
			{
				/*TODO CALC interpolated frame*/
				GE_Engine->actorManager->GetActor((ActorID)effect.second)->transformation->position = animator.second->currentAnimation->animationNodeMap[effect.first]->positionKeys.front().second;
			}
			
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
