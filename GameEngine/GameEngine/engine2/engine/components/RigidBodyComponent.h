#pragma once
#include <engine/components/Component.h>
class RigidBodyComponent : public Component<RigidBodyComponent>
{
public:
	RigidBodyComponent();
	~RigidBodyComponent();
};

