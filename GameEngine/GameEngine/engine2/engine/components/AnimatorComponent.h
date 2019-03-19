#pragma once


#include <engine/components/Component.h>
#include <engine/Animation.h>

class AnimatorComponent :public Component<AnimatorComponent>
{
private:
	time_t startTime;

public:
	AnimatorComponent();
	~AnimatorComponent();

	inline void PlayLoop() {};
	inline void PlayOnce() {};

	Animation *currentAnimation;
//	std::unordered_map<std::string, Actor **> effectlist;

	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new AnimatorComponent(*this));
	}
};

