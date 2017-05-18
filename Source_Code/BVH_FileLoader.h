#pragma once
//stores skeleton animations from BVH files

#include "Texture.h"
#include "MathClass.h"
#include <vector>
#include <stdio.h>
#include <string>
#include <memory>
//#include "drawPrimitives.h"

//a frame is a list of tranformations for the skeleton
struct Frame{
	std::vector<float> m_Tranformations;
};

//a animation is a list of frames
class Animation{
public:
	Animation();
	~Animation();

	void LoadAnimation(FILE *a_BVH_File, int a_TotalNumChannels);

	int m_numFrames;
	float m_FrameTime;
	float m_FramesPerSec;
	std::vector<Frame> m_Frames;

private:
	char m_line[255];
};

//stores the data of each joint of the skeleton
class Joint : public std::enable_shared_from_this<Joint>{
public:
	Joint();
	~Joint();

	void LoadWeightMap(std::string a_fileDirectory);
	void UpdateTransform(std::shared_ptr<Animation> a_animation, int a_frameCounter, int &a_channelCounter);
	void UpdateAdditiveTransform(std::shared_ptr<Animation> a_animation1, std::shared_ptr<Animation> a_animation2, int fa_rameCounter, int &a_channelCounter);	//both animation must have the same number of frames
	void UpdateObjectSpaceTransform(Mat4f a_WParent);
	void AttachChild(std::shared_ptr<Joint> n);

	void DrawJoint();

	std::string m_name;	//the name of the joint
	std::shared_ptr<Joint> m_Parent;
	Mat4f m_TLocal, m_RLocal, m_ObjectSpace, m_SkinningMult;
	Mat4f m_ObjectSpaceAtBind;	//first frame
	Mat4f m_InverseObjectSpaceAtBind;
	Vec4f m_ObjectSpacePos;
	std::vector<std::shared_ptr<Joint>> m_Children;
	std::vector<float> m_Channels;
	int m_numChannels; //can only have 3 or 6 numChannels
	int m_level;	//the level of the joint in the heiarachy

	//Mat4f *skinningOutput;
	Mat4f *m_skinningOutput;

	Texture m_weightMap; //the weight map of the joint for the model's vertices 
};

//can only support one skeleton at a time
class Skeleton
{
public:
	Skeleton();
	~Skeleton();

	//load the entire BVH file, this includes the skeleton and animation
	void LoadBVHFile(char *filename);

	//to load joint weight maps, just give the location of the file that they are in, and program will automatically load all of the
	//proper weight maps and store it to the proper joint
	void LoadJointWeightMaps(std::string a_fileDirectory);
	void UpdateSkeleton(Vec3f a_pos, float a_theta, float a_dt);	//updates the skeleton with the current animation
	void UpdateAdditiveSkeleton(std::shared_ptr<Skeleton> a_otherSkeleton, Vec3f a_pos, float a_theta, float a_dt);	//updates the skeleton with the current additive animation
	void SetOutputJoints(); //intializes the data for the joints that are sent to the shader for mesh skinning
	void DrawSkeleton();

	std::shared_ptr<Joint> m_Root;
	int m_totalNumChannels;	//the total number of numChannels for all of the joints in the skeleton
	float m_time;
	bool m_update;
	int m_channelCounter;
	int m_frameCounter;

	Mat4f m_skiningOutputList[100]; //needed for skinning

	std::vector<std::shared_ptr<Joint>> m_allJoints; //all of the joints of the skeleton in a nice list

	//is used to access animation data
	std::shared_ptr<Animation> m_animation;

private:
	FILE *m_BVH_File;
	char m_line[255];
	bool m_EndSite;

	//recurrsivly parse the skeleton heiarchy by using a depth first traversal
	void ParseFile(int &a_numOfBrakets, std::shared_ptr<Joint> a_current);
	//load the skeleton
	void LoadSkeleton();
};