#pragma once
#include "JointTransformClass.h"
#include <vector>
#include <map>
class KeyFrame
{
private:

	float timeStamp;
	std::map<std::string,JointTransform> pose;
public:
	KeyFrame(float timeStamp, std::map<std::string, JointTransform> pose)
	{
		this->timeStamp = timeStamp;
		this->pose = pose;
	}
	float get_timeStamp()
	{
		return timeStamp;
	}
	std::map<std::string, JointTransform> get_pose()
	{
		return pose;
	}
};