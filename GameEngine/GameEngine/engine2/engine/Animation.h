#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include<engine/Namable.h>
#include <vector>
#include <unordered_map>
#include<glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using AnimationTimeType = uint16_t;

template<typename T>
using KeyFrameHolder = std::vector<std::pair<AnimationTimeType, T>>;

class AnimationNode;
using AnimationNodeMap = std::unordered_map<std::string, AnimationNode *>; // maybe we can change unordered to normal map


class AnimationNode : public Namable
{
public:
	AnimationNode(std::string name, KeyFrameHolder<glm::vec3>, KeyFrameHolder<glm::vec3>, KeyFrameHolder<glm::quat>);
	~AnimationNode();

	KeyFrameHolder<glm::vec3> positionKeys;
	KeyFrameHolder<glm::vec3> scaleKeys;
	KeyFrameHolder<glm::quat> rotationKeys;


};


class Animation : public Namable
{

public:
	Animation(std::string name, AnimationNodeMap, AnimationTimeType duration, AnimationTimeType tickpersecond);
	~Animation();

	AnimationNodeMap animationNodeMap;
	AnimationTimeType duration;
	AnimationTimeType ticksPerSecond;


	
};

