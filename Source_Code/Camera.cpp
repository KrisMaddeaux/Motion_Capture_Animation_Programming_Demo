#include "Camera.h"

Camera::Camera() { 
	m_speed = 0.1f;
	m_ThetaX = 0.0f;
	m_ThetaY = 0.0f; 
}
//--------------------------------------------------------------------------------------------------------------------
//TRANSLATE FUNCTIONS
//only translate the camera target
void Camera::TranslateCameraTarget(float x, float y, float z) {
	m_target += Vec3f(x, y, z);
	UpdateCamera();
}
//only translate camera position
void Camera::TranslateCameraPosition(float x, float y, float z) {
	m_position += Vec3f(x, y, z);
	UpdateCamera();
}
//translate camera target and position
void Camera::TranslateCamera(float x, float y, float z) {
	TranslateCameraTarget(x, y, z);
	TranslateCameraPosition(x, y, z);
	UpdateCamera();
}
//--------------------------------------------------------------------------------------------------------------------
void Camera::RotateCamera(float x, float y, float z) {
	m_ThetaX += y;
	m_ThetaY += x;

	/*fix floating point errors*/
	if (m_ThetaX >= 360.0f){
		m_ThetaX -= 360.0f;
	}
	else if (m_ThetaX <= -360.0f){
		m_ThetaX += 360.0f;
	}

	if (m_ThetaY >= 360.0f) {
		m_ThetaY -= 360.0f;
	}
	else if (m_ThetaY <= -360.0f) {
		m_ThetaY += 360.0f;
	}

	m_ForwardVector = (Rotate(0.0f, m_ThetaY, 0.0f) * Vec4f(m_StartingForwardVector, 1.0f)).returnAsVec3f();
	
	UpdateCamera();
}
//--------------------------------------------------------------------------------------------------------------------
//set functions
void Camera::SetCameraTarget(float x, float y, float z) {
	m_target = Vec3f(x, y, z);

	m_ForwardVector = (m_target - m_position).normalize();
	m_StartingForwardVector = m_ForwardVector;

	UpdateCamera();
}

void Camera::SetCameraPosition(float x, float y, float z) {
	m_position = Vec3f(x, y, z);

	m_ForwardVector = (m_target - m_position).normalize();
	m_StartingForwardVector = m_ForwardVector;

	UpdateCamera();
}
//--------------------------------------------------------------------------------------------------------------------
//get functions
Vec3f Camera::GetCameraTarget() {
	return m_target;
}

Vec3f Camera::GetCameraPosition() {
	return m_position;
}

Mat4f Camera::GetViewMatrix() {
	return m_ViewMatrix;
}
//--------------------------------------------------------------------------------------------------------------------
//input functions
void Camera::KeyboardInput(unsigned char key) {

	if(key == 'a' || key == 'A') { 
		Vec3f l_newPos = m_RightVector * m_speed;
		TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 's' || key == 'S') { 
		Vec3f l_newPos = m_ForwardVector * (-m_speed);
		TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 'd' || key == 'D') { 
		Vec3f l_newPos = m_RightVector * (-m_speed);
		TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 'w' || key == 'W') { 
		Vec3f l_newPos = m_ForwardVector * m_speed;
		TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 27) { //the esc key
		exit(0); 
	}
}

void Camera::MouseInput(int x, int y) {
	Vec3f l_CurrentMousePosition = Vec3f(x, y, 0.0f); 
	Vec3f l_dir = (l_CurrentMousePosition - m_PreviousMousePosition).normalize();	//This tells us the direction the camera needs to rotate
	RotateCamera(l_dir.x, l_dir.y, 0.0f);
	m_PreviousMousePosition = l_CurrentMousePosition; 
}
//--------------------------------------------------------------------------------------------------------------------
void Camera::UpdateCamera() {
	//calculate new frenet frame of camera
	//m_ForwardVector = //(m_target - m_position).normalize();
	m_RightVector = ((m_ForwardVector).cross(Vec3f(0.0f, 1.0f, 0.0f))).normalize();
	m_UpVector = (m_RightVector.cross(m_ForwardVector)).normalize();

	Mat4f l_RotateMatrix = Mat4f(m_RightVector.x, m_RightVector.y, m_RightVector.z, 0.0f,
								 m_UpVector.x, m_UpVector.y, m_UpVector.z, 0.0f,
								-m_ForwardVector.x, -m_ForwardVector.y, -m_ForwardVector.z, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

	//update View matrix
	m_ViewMatrix = l_RotateMatrix * Translate(m_position);
}