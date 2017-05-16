#pragma once
#include "MathClass.h"

class Camera {
public:
	Camera();

	void UpdateCamera();

	//TRANSLATE FUNCTIONS
	//only translate the camera target
	void TranslateCameraTarget(float x, float y, float z);
	//only translate camera position
	void TranslateCameraPosition(float x, float y, float z);
	//translate camera target and position
	void TranslateCamera(float x, float y, float z);

	//set functions
	void SetCameraTarget(float x, float y, float z);
	void SetCameraPosition(float x, float y, float z);

	//get functions
	Vec3f GetCameraTarget();
	Vec3f GetCameraPosition();

private:
	//Frenet Frame
	Vec3f m_ForwardVector;
	Vec3f m_UpVector;
	Vec3f m_RightVector; 

	Vec3f m_position; 
	Vec3f m_target; //look at position
};