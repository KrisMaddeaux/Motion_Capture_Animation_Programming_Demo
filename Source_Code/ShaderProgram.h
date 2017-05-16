#pragma once
#include "include\GL\glew.h"		//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\GL\freeglut.h"	//OpenGL Utility library. Provides simplified API for window management, event handeling, IO control, etc.

#include <iostream>
#include <fstream>
#include <string>

//Helper class to allow easy creation and use of OpenGL shaders
class ShaderProgram{
public:
	ShaderProgram(); //Constructor
	void AddShader(const char *a_ShaderFileName, GLenum a_ShaderType); //Adds shader to Shader Program, returns -1 if there is problem and 1 if successful
	void Enable();
	void Disable();
	GLuint GetUniformLocation(const char *a_VariableName);

private:
	void CompileShader(const char *a_ShaderFileName, GLenum a_ShaderType);
	void LinkShader();

	GLuint m_ShaderProgramObject; //all shaders in this program are linked together into this object

};