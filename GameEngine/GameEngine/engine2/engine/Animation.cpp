#include "Animation.h"


 
AnimationNode::AnimationNode(std::string name, KeyFrameHolder<glm::vec3> poskeys, KeyFrameHolder<glm::vec3> scalekeys, KeyFrameHolder<glm::quat> rotkeys) : 
	Namable(name), positionKeys(poskeys), scaleKeys(scalekeys), rotationKeys(rotkeys) {}
AnimationNode::~AnimationNode()
{
}


Animation::Animation(std::string name, AnimationNodeMap map, AnimationTimeType dura, AnimationTimeType tickpersecond) : 
	Namable(name), animationNodeMap(map), duration(dura), ticksPerSecond(tickpersecond) {}


Animation::~Animation()
{
}
