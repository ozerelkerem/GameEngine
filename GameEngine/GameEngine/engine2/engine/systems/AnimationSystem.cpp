#include "System.h"
#include "AnimationSystem.h"
#include <engine/ActorManager.h>
#include <iostream>
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
		if (animator.second->currentAnimation && animator.second->state)
		{
			Animation *a = animator.second->currentAnimation;
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(GE_Engine->getTime() - animator.second->startTime).count();
			if (!animator.second->isLoop && elapsed >( a->duration * (1000.f / (float)a->ticksPerSecond)))
			{
				animator.second->state = 0;
				continue;
			}
			
			auto  frame = fmod((elapsed / (1000.f / (float)a->ticksPerSecond)), a->duration);

			for (auto effect : animator.second->effectlist)
			{
				Actor *actor = GE_Engine->actorManager->GetActor(effect.second);
				a->animationNodeMap[effect.first]->calcInterpolationPosition(frame, actor->transformation->localPosition);
				a->animationNodeMap[effect.first]->calcInterpolationRotation(frame, actor->transformation->localquatRotation);
				actor->transformation->calcEulerFromQuat();
				a->animationNodeMap[effect.first]->calcInterpolationScale(frame, actor->transformation->localScale);
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
