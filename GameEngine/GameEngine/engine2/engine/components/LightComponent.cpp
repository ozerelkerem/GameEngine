#include "LightComponent.h"

#include <math.h>

LightComponent::LightComponent(ActorID own) : Component(own)
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

void LightComponent::calculateAttenuation()
{
	constant = 1 / (distance / 2);
	linear = 1 / (distance / 4);
	quadratic = 1 / (distance / 8);
}

void LightComponent::passShader(Shader * s, Transform *t, int lindex)
{	
	s->setInt("lights[" + std::to_string(lindex) + "].type", lightType);
	s->setVec3("lights[" + std::to_string(lindex) + "].position", t->getWorldPosition());
	const glm::mat4 inverted = glm::inverse(t->worldMatrix);
	const glm::vec3 forward = normalize(glm::vec3(inverted[2]));
	s->setVec3("lights[" + std::to_string(lindex) + "].direction", forward);


	s->setVec3("lights[" + std::to_string(lindex) + "].color", color);


	s->setFloat("lights[" + std::to_string(lindex) + "].angle", glm::radians(angle));
	s->setFloat("lights[" + std::to_string(lindex) + "].constant", constant);
	s->setFloat("lights[" + std::to_string(lindex) + "].linear", linear);
	s->setFloat("lights[" + std::to_string(lindex) + "].quadratic", quadratic);
	s->setFloat("lights[" + std::to_string(lindex) + "].distance", distance);
	s->setFloat("lights[" + std::to_string(lindex) + "].intensity", intensity);

}
