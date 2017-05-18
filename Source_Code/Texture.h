#pragma once
#include "include\GL\glew.h"			//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\Magick\Magick++.h"	//For loading images/textures 

#include <string>
#include <iostream>


class Texture {
public:
	Texture(); 

	bool Load(GLenum TextureTarget, const std::string& FileName); //TextureTarget = GL_TEXTURE_2D
	void Draw(GLenum TextureUnit); //TextureUnit = GL_TEXTURE0, GL_TEXTURE1, etc.

private:

	std::string m_fileName;
	GLenum m_textureTarget;
	GLuint m_textureObj;
	Magick::Image m_image;
	Magick::Blob m_blob;
};
