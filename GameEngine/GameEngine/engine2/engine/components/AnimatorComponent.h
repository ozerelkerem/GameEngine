#pragma once


#include <engine/components/Component.h>
#include <engine/Animation.h>



class Animation;

class AnimatorComponent :public Component<AnimatorComponent>
{
private:
	time_t startTime;

public:
	AnimatorComponent(ActorID);
	~AnimatorComponent();

	void PlayLoop(Animation * anim);
	void PlayOnce(Animation *) {};

	Animation *currentAnimation;
	std::unordered_map<std::string, ActorID::value_type> effectlist;

	inline virtual IComponent * getnew(ActorID own, ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		auto x = ((IComponent*) new AnimatorComponent(*this));
		x->owner = own;
		return x;
	}

	void matchAnimation();
};

