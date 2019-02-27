#include "LightComponent.h"



LightComponent::LightComponent() : Component()
{
	this->lightType = LightType::Point;
	this->color = { 1,1,1 };
	this->angle = 10;

	this->constant = 1;
	this->linear = 0.7;
	this->quadratic = 1.8;
	this->distance = 7;
	this->intensity = 1;
}


LightComponent::~LightComponent()
{
}
