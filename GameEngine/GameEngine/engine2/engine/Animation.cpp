#include "Animation.h"

#include<iostream>
 
AnimationNode::AnimationNode(std::string name, KeyFrameHolder<glm::vec3> poskeys, KeyFrameHolder<glm::vec3> scalekeys, KeyFrameHolder<glm::quat> rotkeys) : 
	Namable(name), positionKeys(poskeys), scaleKeys(scalekeys), rotationKeys(rotkeys) {}
AnimationNode::~AnimationNode()
{
}

void AnimationNode::calcInterpolationPosition(float frame, glm::vec3 &dest)
{
	auto asd = std::lower_bound(positionKeys.begin(), positionKeys.end(), frame, [](const animPairVec3 &left, const float &right) -> bool {
		return left.first <= right;
	});

	dest = glm::mix((*asd).second, (*(asd - 1)).second, (int)frame - frame);
	
}

void AnimationNode::calcInterpolationScale(float frame, glm::vec3 &dest)
{
	auto asd = std::lower_bound(scaleKeys.begin(), scaleKeys.end(), frame, [](const animPairVec3 &left, const float &right) -> bool {
		return left.first <= right;
	});

	dest = glm::mix((*asd).second, (*(asd - 1)).second, (int)frame - frame);
}

void AnimationNode::calcInterpolationRotation(float frame, glm::quat &dest)
{
	auto asd = std::lower_bound(rotationKeys.begin(), rotationKeys.end(), frame, [](const animPairQuat &left, const float &right) -> bool {
		return left.first <= right;
	});

	dest = glm::slerp((*asd).second, (*(asd - 1)).second, (int)frame - frame);
}


Animation::Animation(std::string name, AnimationNodeMap map, AnimationTimeType dura, AnimationTimeType tickpersecond) : 
	Namable(name), animationNodeMap(map), duration(dura), ticksPerSecond(tickpersecond) {}


Animation::~Animation()
{
}

