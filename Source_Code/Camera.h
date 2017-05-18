#pragma once
#include "include\GL\glew.h"		//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\GL\freeglut.h"	//OpenGL Utility library. Provides simplified API for window management, event handeling, IO control, etc.

#include "MathClass.h"

class Camera {
public:
	Camera();

	//TRANSLATE FUNCTIONS
	//only translate the camera target
	void TranslateCameraTarget(float x, float y, float z);
	//only translate camera position
	void TranslateCameraPosition(float x, float y, float z);
	//translate camera target and position
	void TranslateCamera(float x, float y, float z);

	void RotateCamera(float x, float y, float z); 

	//set functions
	void SetCameraTarget(float x, float y, float z);
	void SetCameraPosition(float x, float y, float z);

	//get functions
	Vec3f GetCameraTarget();
	Vec3f GetCameraPosition();
	Mat4f GetViewMatrix(); //used to bring world space coordinates into eye/camera space 

	//Frenet Frame
	Vec3f m_ForwardVector;
	Vec3f m_UpVector;
	Vec3f m_RightVector;

	float m_speed;	//speed the camera can move at

private:
	void UpdateCamera();

	Vec3f m_StartingForwardVector;

	Vec3f m_position; 
	Vec3f m_target; //look at position

	Mat4f m_ViewMatrix; 

	float m_ThetaX, m_ThetaY; 
};