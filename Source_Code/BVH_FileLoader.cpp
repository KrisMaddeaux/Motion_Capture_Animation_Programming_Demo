#include "BVH_FileLoader.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//Class Joint
Joint::Joint()
{
	m_Parent = NULL;
	m_TLocal = m_ObjectSpace = Mat4f();
	m_ObjectSpacePos = Vec4f();
}

Joint::~Joint()
{
}

void Joint::LoadWeightMap(std::string a_fileDirectory)
{
	std::string l_temp = a_fileDirectory;
	l_temp.append(m_name);
	l_temp.append(".jpg");

	m_weightMap.Load(GL_TEXTURE_2D, l_temp); 

	//load all of the childern's weight maps too
	for (unsigned int i = 0; i < this->m_Children.size(); ++i)
	{
		m_Children[i]->LoadWeightMap(a_fileDirectory);
	}
}

void Joint::UpdateTransform(std::shared_ptr<Animation> a_animation, int a_frameCounter, int &a_channelCounter)
{
	if (m_numChannels == 6)
	{
		Mat4f l_temp = Translate(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter], 
							   a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 1], 
							   a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 2]);
		m_TLocal = l_temp;

		////so animations stay in one spot
		//Mat4f temp = Translate(0.0f, animation->Frames[frameCounter].Tranformations[channelCounter + 1], 0.0f);
		//TLocal = temp; 	

		Mat4f rZ = RotateZ(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 3]);	//z
		Mat4f rX = RotateX(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 4]);	//y
		Mat4f rY = RotateY(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 5]);	//x
		m_RLocal = rZ * rX * rY; //rZ * rY * rX

		a_channelCounter += 6;
	}
	else if (m_numChannels == 3)
	{
		Mat4f rZ = RotateZ(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter]);
		Mat4f rX = RotateX(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 1]);
		Mat4f rY = RotateY(a_animation->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 2]);
		m_RLocal = rZ * rX * rY;

		a_channelCounter += 3;
	}

	//update all of the childern
	for (unsigned int i = 0; i < this->m_Children.size(); ++i)
	{
		m_Children[i]->UpdateTransform(a_animation, a_frameCounter, a_channelCounter);
	}
}

void Joint::UpdateAdditiveTransform(std::shared_ptr<Animation> a_animation1, std::shared_ptr<Animation> a_animation2, int a_frameCounter, int &a_channelCounter)
{
	if (m_numChannels == 6)
	{
		Mat4f temp = Translate(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter], 
							   a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 1], 
							   a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 2]);
		m_TLocal = temp;

		Mat4f rZ = RotateZ(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 3] + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 3]);
		Mat4f rY = RotateY(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 4] + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 4]);
		Mat4f rX = RotateX(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 5] + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 5]);
		m_RLocal = rZ * rY * rX;

		a_channelCounter += 6;
	}
	else if (m_numChannels == 3)
	{
		Mat4f rZ = RotateZ(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter]	 + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter]);
		Mat4f rY = RotateY(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 1] + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 1]);
		Mat4f rX = RotateX(a_animation1->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 2] + a_animation2->m_Frames[a_frameCounter].m_Tranformations[a_channelCounter + 2]);
		m_RLocal = rZ * rY * rX;

		a_channelCounter += 3;
	}

	//update all of the childern
	for (unsigned int i = 0; i < this->m_Children.size(); i++)
	{
		m_Children[i]->UpdateAdditiveTransform(a_animation1, a_animation2, a_frameCounter, a_channelCounter);
	}
}

void Joint::UpdateObjectSpaceTransform(Mat4f a_WParent)
{
	// FORWARD KINEMATICS!
	m_ObjectSpace = a_WParent * m_TLocal * m_RLocal;

	// CONCATENATE THE MATRICES IN THE SKINNING EQUATION TO SEND TO SHADER
	m_SkinningMult = m_ObjectSpace * m_InverseObjectSpaceAtBind;
	*(m_skinningOutput) = m_SkinningMult;

	m_ObjectSpacePos = m_ObjectSpace * Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

	for (unsigned int i = 0; i < m_Children.size(); i++)
	{
		m_Children[i]->UpdateObjectSpaceTransform(m_ObjectSpace);
	}
}

void Joint::DrawJoint()
{
	/*glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(ObjectSpacePos[0], ObjectSpacePos[1], ObjectSpacePos[2]);
	drawBox(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	for (unsigned int i = 0; i < Children.size(); i++)
	{
		Children[i]->drawJoint();
	}*/
}

void Joint::AttachChild(std::shared_ptr<Joint> n)
{
	n->m_Parent = shared_from_this(); 
	m_Children.push_back(n);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class Animation
Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::LoadAnimation(FILE *a_BVH_File, int a_totalNumChannels)
{
	Frame l_tempFrame;

	while (1)	//while true
	{
		int l_res = fscanf(a_BVH_File, "%s", m_line);
		//checks for end of file (EOF)
		if (l_res == EOF)
		{
			break;
		}
		else if (strcmp(m_line, "Frames:") == 0)
		{
			int l_frames;
			fscanf(a_BVH_File, "%i", &l_frames);
			m_numFrames = l_frames;
		}
		else if (strcmp(m_line, "Time:") == 0)
		{
			float l_frameTime;
			fscanf(a_BVH_File, "%f", &l_frameTime);
			m_FrameTime = l_frameTime;
			m_FramesPerSec = 1.0f / m_FrameTime;

			//get the animation data
			int l_counter = 0;
			while (1)
			{
				float l_num;
				l_res = fscanf(a_BVH_File, "%f", &l_num);

				if (l_counter == a_totalNumChannels)
				{
					m_Frames.push_back(l_tempFrame);
					l_tempFrame.m_Tranformations.clear();
					l_counter = 0;
				}
				else if (l_res == EOF)
				{
					fclose(a_BVH_File);
					return;
				}

				l_tempFrame.m_Tranformations.push_back(l_num);
				l_counter++;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Class Skeleton

Skeleton::Skeleton()
{
	m_Root = NULL;
	m_EndSite = false;
	m_totalNumChannels = 0;
	m_update = true;
	m_channelCounter = 0;
	m_frameCounter = 0;
	m_time = 0.0f;
	m_animation = std::make_unique<Animation>();
}

Skeleton::~Skeleton()
{
}

//load the entire BVH file
void Skeleton::LoadBVHFile(char *a_filename)
{
	m_BVH_File = fopen(a_filename, "r");
	//When the file cannot be opened (e.g., we don't have permission or it doesn't exist when opening for reading), fopen() will return NULL.
	if (m_BVH_File == NULL)
	{
		std::cout << "ERROR, could not open BVH file" << std::endl;
		return;
	}

	//load the skeleton
	LoadSkeleton();
	//load the animation
	m_animation->LoadAnimation(m_BVH_File, m_totalNumChannels);
	//initliaze joints for mesh skinning
	SetOutputJoints();
}

void Skeleton::LoadSkeleton()
{
	//the current joint being processed
	std::shared_ptr<Joint> l_current;
	l_current = std::make_shared<Joint>();

	//lets use know which level the joint is on
	int l_numOfBrakets = 0;

	while (1)	//while true
	{
		fscanf(m_BVH_File, "%s", m_line);

		if (strcmp(m_line, "ROOT") == 0)
		{
			m_Root = std::make_unique<Joint>();
			char l_jointName[50];
			fscanf(m_BVH_File, "%s", l_jointName);
			m_Root->m_name = l_jointName;
			m_Root->m_level = l_numOfBrakets;
			m_Root->m_numChannels = 6;
			l_current = m_Root;
			m_totalNumChannels += 6;
			m_allJoints.push_back(m_Root);
		}
		else if (strcmp(m_line, "OFFSET") == 0)
		{
			Vec3f l_offset;
			fscanf(m_BVH_File, "%f %f %f\n", &l_offset.x, &l_offset.y, &l_offset.z);
			//current->TLocal = Translate(offset);
			l_current->m_TLocal = Identity(); //so animations stay in one spot
			if (l_current->m_Parent != NULL)
			{
				l_current->m_ObjectSpaceAtBind = l_current->m_Parent->m_ObjectSpaceAtBind * l_current->m_TLocal;
				l_current->m_InverseObjectSpaceAtBind = transformInverseNoScale(l_current->m_ObjectSpaceAtBind);
				//skiningOutputList[allJoints.size()-1] = current->ObjectSpaceAtBind * current->InverseObjectSpaceAtBind; 
			}
			else
			{
				l_current->m_ObjectSpaceAtBind = Identity(); /* current->TLocal;*/
				l_current->m_InverseObjectSpaceAtBind = transformInverseNoScale(l_current->m_ObjectSpaceAtBind);
				//skiningOutputList[allJoints.size()-1] = current->ObjectSpaceAtBind * current->InverseObjectSpaceAtBind; 
			}
		}
		else if (strcmp(m_line, "{") == 0)
		{
			l_numOfBrakets++;
		}
		else if (strcmp(m_line, "}") == 0)
		{
			l_numOfBrakets--;
		}
		else if (strcmp(m_line, "JOINT") == 0)
		{
			std::shared_ptr<Joint> l_temp;
			l_temp = std::make_shared<Joint>();
			char l_jointName[50];
			fscanf(m_BVH_File, "%s", l_jointName);
			l_temp->m_name = l_jointName;
			l_temp->m_level = l_numOfBrakets;
			l_temp->m_numChannels = 3;
			m_totalNumChannels += 3;

			//find the right parent
			if (l_current->m_level >= l_temp->m_level)
			{
				while (l_current->m_level >= l_temp->m_level)
				{
					l_current = l_current->m_Parent;
				}
			}

			l_current->AttachChild(l_temp);
			l_current = l_temp;
			m_allJoints.push_back(l_temp);
			ParseFile(l_numOfBrakets, l_current);
		}

		//this indicates that it is time to move onto to the animation portion of the BVH file
		if (strcmp(m_line, "MOTION") == 0)
		{
			return;
		}
	}
}

void Skeleton::ParseFile(int &a_numOfBrakets, std::shared_ptr<Joint> a_current)
{
	while (1)
	{
		fscanf(m_BVH_File, "%s", m_line);

		//general cases
		if (strcmp(m_line, "}") == 0)
		{
			a_numOfBrakets--;
		}
		else if (strcmp(m_line, "End") == 0)
		{
			m_EndSite = true;
		}
		else if (m_EndSite && strcmp(m_line, "OFFSET") == 0)
		{
			m_EndSite = false;
		}
		else if (strcmp(m_line, "{") == 0)
		{
			a_numOfBrakets++;
		}
		else if (strcmp(m_line, "OFFSET") == 0)
		{
			Vec3f l_offset;
			fscanf(m_BVH_File, "%f %f %f\n", &l_offset.x, &l_offset.y, &l_offset.z);
			a_current->m_TLocal = Translate(l_offset);
			if (a_current->m_Parent != NULL)
			{
				a_current->m_ObjectSpaceAtBind = a_current->m_Parent->m_ObjectSpaceAtBind * a_current->m_TLocal;
				a_current->m_InverseObjectSpaceAtBind = transformInverseNoScale(a_current->m_ObjectSpaceAtBind);
				//skiningOutputList[allJoints.size()-1] = current->ObjectSpaceAtBind * current->InverseObjectSpaceAtBind; 
			}
			else
			{
				a_current->m_ObjectSpaceAtBind = Identity(); /* current->TLocal;*/
				a_current->m_InverseObjectSpaceAtBind = transformInverseNoScale(a_current->m_ObjectSpaceAtBind);
				//skiningOutputList[allJoints.size()-1] = current->ObjectSpaceAtBind * current->InverseObjectSpaceAtBind; 
			}
		}
		else if (strcmp(m_line, "JOINT") == 0)
		{
			std::shared_ptr<Joint> l_temp;
			l_temp = std::make_shared<Joint>();
			char l_jointName[50];
			fscanf(m_BVH_File, "%s", l_jointName);
			l_temp->m_name = l_jointName;
			l_temp->m_level = a_numOfBrakets;
			l_temp->m_numChannels = 3;
			m_totalNumChannels += 3;

			//find the right parent
			if (a_current->m_level >= l_temp->m_level)
			{
				while (a_current->m_level >= l_temp->m_level)
				{
					a_current = a_current->m_Parent;
				}
			}

			a_current->AttachChild(l_temp);
			a_current = l_temp;
			m_allJoints.push_back(l_temp);
			ParseFile(a_numOfBrakets, a_current);
		}

		//base cases
		if (strcmp(m_line, "MOTION") == 0)
		{
			return;
		}
	}
}

void Skeleton::UpdateSkeleton(Vec3f a_pos, float a_theta, float a_dt)
{
	m_time += a_dt /* (animation->FrameTime)*/;

	//update the animation
	if (m_time >= m_animation->m_FrameTime)
	{
		m_update = true;
		m_time = 0.0f;
		m_channelCounter = 0;
		m_frameCounter++;

		//repeat the animation
		if (m_frameCounter >= m_animation->m_numFrames)
		{
			m_frameCounter = 0;
		}
	}

	//if it is time to update the frame of the animation
	if (m_update)
	{
		m_update = false;
		m_Root->UpdateTransform(m_animation, m_frameCounter, m_channelCounter);
	}

	Mat4f l_temp = Translate(0.0f, 0.0f, 0.0f);//Translate(pos) * RotateY(theta) * Scale(0.65f, 0.65f, 0.65f);
	m_Root->UpdateObjectSpaceTransform(l_temp);
}

void Skeleton::UpdateAdditiveSkeleton(std::shared_ptr<Skeleton> a_otherSkeleton, Vec3f a_pos, float a_theta, float a_dt)
{
	m_time += a_dt /* (animation->FrameTime)*/;

	//update the animation
	if (m_time >= m_animation->m_FrameTime)
	{
		m_update = true;
		m_time = 0.0f;
		m_channelCounter = 0;
		m_frameCounter++;

		//repeat the animation
		if (m_frameCounter >= m_animation->m_numFrames)
		{
			m_frameCounter = 0;
		}
	}

	//if it is time to update the frame of the animation
	if (m_update)
	{
		m_update = false;
		m_Root->UpdateAdditiveTransform(m_animation, a_otherSkeleton->m_animation, m_frameCounter, m_channelCounter);
	}

	Mat4f l_temp = Translate(a_pos) * RotateY(a_theta);
	m_Root->UpdateObjectSpaceTransform(l_temp);
}

void Skeleton::DrawSkeleton()
{
	/*glDisable(GL_LIGHTING);
	Root->drawJoint();
	//Root->Children[0]->Children[0]->drawJoint();
	glEnable(GL_LIGHTING);*/
}

void Skeleton::LoadJointWeightMaps(std::string a_fileDirectory)
{
	m_Root->LoadWeightMap(a_fileDirectory);
}

void Skeleton::SetOutputJoints()
{
	for (unsigned int i = 0; i < m_allJoints.size(); i++)
	{
		m_allJoints[i]->m_skinningOutput = m_skiningOutputList + i;
	}
}

