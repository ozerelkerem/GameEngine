#pragma once


#include <engine/systems/System.h>
#include <engine/GameBase.h>
#include <engine/Actor.h>

class AnimationSystem : public System <AnimationSystem>
{
public:

	AnimationSystem(GameBase *);
	~AnimationSystem();

	virtual void PreUpdate() override;
	virtual void Update() override;
	virtual void PostUpdate() override;

	void loopHierarchy(ActorID);

	GameBase *gamebase;

};

