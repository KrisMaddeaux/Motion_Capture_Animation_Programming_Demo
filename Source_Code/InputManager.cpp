#include "InputManager.h"

InputManager::InputManager(std::shared_ptr<Camera> a_Camera) {
	m_Camera = a_Camera;
}

void InputManager::KeyboardInput(unsigned char key) {
	if (key == 'a' || key == 'A') {
		Vec3f l_newPos = m_Camera->m_RightVector * m_Camera->m_speed;
		m_Camera->TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 's' || key == 'S') {
		Vec3f l_newPos = m_Camera->m_ForwardVector * (-m_Camera->m_speed);
		m_Camera->TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 'd' || key == 'D') {
		Vec3f l_newPos = m_Camera->m_RightVector * (-m_Camera->m_speed);
		m_Camera->TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 'w' || key == 'W') {
		Vec3f l_newPos = m_Camera->m_ForwardVector * m_Camera->m_speed;
		m_Camera->TranslateCamera(l_newPos.x, l_newPos.y, l_newPos.z);
	}
	else if (key == 27) { //the esc key
		exit(0);
	}
}

void InputManager::MouseMotionInput(int x, int y) {
	Vec3f l_CurrentMousePosition = Vec3f(x, y, 0.0f);
	Vec3f l_dir = (l_CurrentMousePosition - m_PreviousMousePosition).normalize();	//This tells us the direction the camera needs to rotate
	m_Camera->RotateCamera(l_dir.x, l_dir.y, 0.0f);
	m_PreviousMousePosition = l_CurrentMousePosition;
}