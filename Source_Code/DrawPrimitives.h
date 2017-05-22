#pragma once
#include "include\GL\glew.h"		//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\GL\freeglut.h"	//OpenGL Utility library. Provides simplified API for window management, event handeling, IO control, etc.

namespace DrawPrimitive {
	static void DrawCube(float width, float height, float depth) {

		glBegin(GL_QUADS);
		glPushMatrix();

		//front
		glVertex3f(-width / 2, -height / 2, depth / 2);
		glVertex3f(width / 2, -height / 2, depth / 2);
		glVertex3f(width / 2, height / 2, depth / 2);
		glVertex3f(-width / 2, height / 2, depth / 2);

		//back 
		glVertex3f(-width / 2, height / 2, -depth / 2);
		glVertex3f(width / 2, height / 2, -depth / 2);
		glVertex3f(width / 2, -height / 2, -depth / 2);
		glVertex3f(-width / 2, -height / 2, -depth / 2);

		//left side
		glVertex3f(-width / 2, height / 2, depth / 2);
		glVertex3f(-width / 2, height / 2, -depth / 2);
		glVertex3f(-width / 2, -height / 2, -depth / 2);
		glVertex3f(-width / 2, -height / 2, depth / 2);


		//right side
		glVertex3f(width / 2, -height / 2, depth / 2);
		glVertex3f(width / 2, -height / 2, -depth / 2);
		glVertex3f(width / 2, height / 2, -depth / 2);
		glVertex3f(width / 2, height / 2, depth / 2);

		//top side
		glVertex3f(-width / 2, height / 2, depth / 2);
		glVertex3f(width / 2, height / 2, depth / 2);
		glVertex3f(width / 2, height / 2, -depth / 2);
		glVertex3f(-width / 2, height / 2, -depth / 2);


		//bottom side
		glVertex3f(-width / 2, -height / 2, -depth / 2);
		glVertex3f(width / 2, -height / 2, -depth / 2);
		glVertex3f(width / 2, -height / 2, -depth / 2);
		glVertex3f(-width / 2, -height / 2, depth / 2);

		glPopMatrix();
		glEnd();
	}
}
