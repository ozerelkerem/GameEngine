#pragma once


#include <engine/components/Component.h>

class AnimatorComponent :public Component<AnimatorComponent>
{
public:
	AnimatorComponent();
	~AnimatorComponent();

	inline void PlayLoop() {};
	inline void PlayOnce() {};


	inline virtual IComponent * getnew(ComponentTypeID *id) override {
		*id = STATIC_COMPONENT_TYPE_ID;
		return ((IComponent*) new AnimatorComponent(*this));
	}
};

