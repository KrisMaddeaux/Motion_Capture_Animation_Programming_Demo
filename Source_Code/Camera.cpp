#include "Camera.h"

Camera::Camera() { }

void Camera::UpdateCamera() {
	m_ForwardVector = (m_target - Vec3f(m_position.x, -m_position.y, m_position.z)).normalize();
	m_RightVector = ((m_ForwardVector).cross(Vec3f(0.0f, 1.0f, 0.0f))).normalize();
	m_UpVector = (m_RightVector.cross(m_ForwardVector)).normalize();

	m_ForwardVector = m_ForwardVector.negate();
	m_RightVector = m_RightVector.negate();
	m_UpVector = m_UpVector.negate();
}

//TRANSLATE FUNCTIONS
//only translate the camera target
void Camera::TranslateCameraTarget(float x, float y, float z) {
	m_target += Vec3f(x, y, z);
}
//only translate camera position
void Camera::TranslateCameraPosition(float x, float y, float z) {
	m_position += Vec3f(x, y, z);
}
//translate camera target and position
void Camera::TranslateCamera(float x, float y, float z) {
	TranslateCameraTarget(x, y, z);
	TranslateCameraPosition(x, y, z);
}

//set functions
void Camera::SetCameraTarget(float x, float y, float z) {
	m_target = Vec3f(x, y, z);
}

void Camera::SetCameraPosition(float x, float y, float z) {
	m_position = Vec3f(x, y, z);
}

//get functions
Vec3f Camera::GetCameraTarget() {
	return m_target;
}

Vec3f Camera::GetCameraPosition() {
	return m_position;
}