#pragma once

#include <engine/components/ModelComponent.h>
#include <engine/components/IModelComponent.h>
class Actor;

class SkinnedModelComponent : public IModelComponent, public Component<SkinnedModelComponent>
{
public:
	SkinnedModelComponent(Model *);
	~SkinnedModelComponent();

	Actor *rootBone;
};



