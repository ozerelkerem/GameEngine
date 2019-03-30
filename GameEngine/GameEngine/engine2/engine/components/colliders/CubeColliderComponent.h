#pragma once
#include<engine/components/colliders/ICollider.h>
#include<engine/components/Component.h>
class CubeColliderComponent : public ICollider, public Component<CubeColliderComponent>
{
public:
	CubeColliderComponent();
	~CubeColliderComponent();

	float x, y, z;
};

