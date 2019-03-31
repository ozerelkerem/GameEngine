#include "System.h"
#include "AnimationSystem.h"
#include <engine/ActorManager.h>
#include <engine/components/AnimatorComponent.h>
AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::PreUpdate()
{

}

void AnimationSystem::Update()
{
	auto end = GE_Engine->componentManager->end<AnimatorComponent>();
	
	for (auto it = GE_Engine->componentManager->begin<AnimatorComponent>(); it.operator!=(end); it.operator++())
	{
		if (it->currentAnimation && it->state)
		{
			Animation *a = it->currentAnimation;
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(GE_Engine->getTime() - it->startTime).count();
			if (!it->isLoop && elapsed >( a->duration * (1000.f / (float)a->ticksPerSecond)))
			{
				it->state = 0;
				continue;
			}
			
			auto  frame = fmod((elapsed / (1000.f / (float)a->ticksPerSecond)), a->duration);

			for (auto effect : it->effectlist)
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
