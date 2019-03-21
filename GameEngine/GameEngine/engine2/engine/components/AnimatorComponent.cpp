#include "AnimatorComponent.h"
#include <Engine.h>
#include <engine/ActorManager.h>

AnimatorComponent::AnimatorComponent(ActorID own) : Component(own)
{
	state = false;
}


AnimatorComponent::~AnimatorComponent()
{
}

void AnimatorComponent::PlayLoop(Animation * anim)
{
	state = true;
	isLoop = true;
	currentAnimation = anim;
	startTime = GE_Engine->getTime();
	matchAnimation();
}
void AnimatorComponent::PlayOnce(Animation * anim)
{
	state = true;
	isLoop = false;
	currentAnimation = anim;
	startTime = GE_Engine->getTime();
	matchAnimation();
}

void AnimatorComponent::matchAnimation()
{
	if (!currentAnimation)
		return;
	effectlist.clear();
	Actor *a = GE_Engine->actorManager->GetActor(this->owner);

	std::list<Actor *> queue;
	Actor *s;
	queue.push_back(a);
	AnimationNodeMap tempmap(currentAnimation->animationNodeMap);
	
	while (!queue.empty() && tempmap.size() > 0)
	{
		s = queue.front();
		if (tempmap[s->name])
		{
			effectlist[s->name] = s->actorID;
			tempmap[s->name] = nullptr;
		}
			

		queue.pop_front();

		for (int i = 0; i < s->numberOfChildren; i++)
		{
			queue.push_back(GE_Engine->actorManager->GetActor(s->children[i]));
		}
	}

}


