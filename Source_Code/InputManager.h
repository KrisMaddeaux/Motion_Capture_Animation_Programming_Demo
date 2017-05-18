#pragma once
#include "Camera.h"

#include <memory>

class InputManager {
public:
	InputManager(std::shared_ptr<Camera> a_Camera);

	void KeyboardInput(unsigned char key);
	void MouseMotionInput(int x, int y);

private:
	std::shared_ptr<Camera> m_Camera; 
	Vec3f m_PreviousMousePosition;

};