#include "ShaderProgram.h"

//--------------------------------------------------------------------------------------------------------------------
ShaderProgram::ShaderProgram() {
	m_ShaderProgramObject = glCreateProgram();	//all shaders in this program are linked together into this object

												//Check for errors
	if (m_ShaderProgramObject == 0) {
		fprintf(stderr, "Error creating shader program\n");
	}
}
//--------------------------------------------------------------------------------------------------------------------
void ShaderProgram::AddShader(const char *a_ShaderFileName, GLenum a_ShaderType) {
	CompileShader(a_ShaderFileName, a_ShaderType);	//must compile shader file first before being able to run it

	//Link Shader
	LinkShader();
}
//--------------------------------------------------------------------------------------------------------------------
void ShaderProgram::Enable() {
	glUseProgram(m_ShaderProgramObject);
}
//--------------------------------------------------------------------------------------------------------------------
void ShaderProgram::Disable() {
	glUseProgram(0);
}
//--------------------------------------------------------------------------------------------------------------------
GLuint ShaderProgram::GetUniformLocation(const char *a_VariableName) {
	GLuint l_v = glGetUniformLocation(m_ShaderProgramObject, a_VariableName);
	if (l_v == -1) {	//if there was an error, could be caused by misspelling name or variable not used in shader
		std::cout << "Error trying to get uniform location for: " << a_VariableName << std::endl;	
	}
	return l_v; 
}
//--------------------------------------------------------------------------------------------------------------------
void ShaderProgram::CompileShader(const char *a_ShaderFileName, GLenum a_ShaderType) {
	//Load Shader File so it can be compiled
	std::ifstream l_file;
	l_file.open(a_ShaderFileName);

	std::string l_shadertext = "";
	if (l_file.is_open()) {
		char l_c;
		while (l_file.get(l_c)) {
			l_shadertext += l_c;
		}
		l_file.close();
	}
	else {
		std::cout << "Unable to open file: " << a_ShaderFileName << std::endl;
		return;
	}

	const char *l_buffer = l_shadertext.c_str();

	GLuint l_ShaderObject = glCreateShader(a_ShaderType);	//create a shader object
	//Check for errors
	if (l_ShaderObject == 0) {
		fprintf(stderr, "Error creating shader type %d\n", a_ShaderType);
		return;
	}

	//Specify shader source code
	const GLchar *l_p[1];
	l_p[0] = l_buffer;
	GLint Lengths[1];
	Lengths[0] = strlen(l_buffer);
	glShaderSource(l_ShaderObject, 1, l_p, Lengths);

	//Compile Shader source code
	glCompileShader(l_ShaderObject);
	GLint l_success;
	glGetShaderiv(l_ShaderObject, GL_COMPILE_STATUS, &l_success);
	if (!l_success) {
		GLchar l_InfoLog[1024];
		glGetShaderInfoLog(l_ShaderObject, 1024, NULL, l_InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", a_ShaderType, l_InfoLog);
		return;
	}

	glAttachShader(m_ShaderProgramObject, l_ShaderObject); //attach compiled shader object to program object
}
//--------------------------------------------------------------------------------------------------------------------
void ShaderProgram::LinkShader() {
	GLint l_Success = 0;
	GLchar l_ErrorLog[1024] = { 0 };

	//Link program to check for errors on the combination of shaders
	glLinkProgram(m_ShaderProgramObject);
	glGetProgramiv(m_ShaderProgramObject, GL_LINK_STATUS, &l_Success);
	if (l_Success == 0) {
		glGetProgramInfoLog(m_ShaderProgramObject, sizeof(l_ErrorLog), NULL, l_ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", l_ErrorLog);
		return;
	}

	//Validate program to check whether the program can execute given the current pipeline state
	glValidateProgram(m_ShaderProgramObject);
	glGetProgramiv(m_ShaderProgramObject, GL_VALIDATE_STATUS, &l_Success);
	if (!l_Success) {
		glGetProgramInfoLog(m_ShaderProgramObject, sizeof(l_ErrorLog), NULL, l_ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", l_ErrorLog);
		return;
	}
}
//--------------------------------------------------------------------------------------------------------------------