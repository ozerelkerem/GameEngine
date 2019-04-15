#pragma once


#include <engine/components/Component.h>
#include <engine/Animation.h>

class Animation;

class AnimatorComponent :public Component<AnimatorComponent>
{
public:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	bool state;
	bool isLoop;

	AnimatorComponent();
	~AnimatorComponent();

	void PlayLoop(Animation * anim);
	void PlayOnce(Animation *);
	void Stop();

	Animation *currentAnimation;
	std::unordered_map<std::string, ActorID::value_type> effectlist;

	
	void matchAnimation();
};

