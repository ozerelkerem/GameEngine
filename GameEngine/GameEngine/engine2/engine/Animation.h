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

typedef std::pair<AnimationTimeType, glm::vec3> animPairVec3;
typedef std::pair<AnimationTimeType, glm::quat> animPairQuat;

class AnimationNode : public Namable
{
public:
	AnimationNode(std::string name, KeyFrameHolder<glm::vec3>, KeyFrameHolder<glm::vec3>, KeyFrameHolder<glm::quat>);
	~AnimationNode();

	std::vector<animPairVec3> positionKeys;
	std::vector<animPairVec3> scaleKeys;
	std::vector<animPairQuat> rotationKeys;

	void calcInterpolationPosition(float frame, glm::vec3&);
	void calcInterpolationScale(float frame, glm::vec3&);
	void calcInterpolationRotation(float frame, glm::quat&);

};


class Animation : public Namable
{

public:
	Animation(std::string name, AnimationNodeMap, AnimationTimeType duration, AnimationTimeType tickpersecond);
	~Animation();

	AnimationNodeMap animationNodeMap;
	const AnimationTimeType duration;
	const AnimationTimeType ticksPerSecond;


};



