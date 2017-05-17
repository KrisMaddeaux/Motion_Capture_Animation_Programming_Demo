#pragma once
#include "include\GL\glew.h"		//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\GL\freeglut.h"	//OpenGL Utility library. Provides simplified API for window management, event handeling, IO control, etc.

#include "ShaderProgram.h"
#include "Camera.h"
#include "MathClass.h"

#include <memory>

//Updates and Draws the scene
class SceneRenderer{
public:
	SceneRenderer(float a_WindowSizeX, float a_WindowSizeY, std::shared_ptr<Camera> a_Camera);
	void RenderScene();

private:

	void UpdateScene();
	void DrawScene();

	//uniform variables for shaders
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_MVPLocation;

	std::unique_ptr<ShaderProgram> m_ShaderProgram;
	std::shared_ptr<Camera> m_Camera;
	
	Mat4f m_PerspectiveMatrix;


};