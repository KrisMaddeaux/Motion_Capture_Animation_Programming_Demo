#include "ShaderProgram.h"
#include "MathClass.h"

#include <memory>

GLuint g_VBO;
//GLuint g_ScaleLocation; 
GLuint g_WorldLocation; 
GLuint g_PerspectiveLocation; 

Mat4f g_PerspectiveMatrix; 

const int g_WindowSizeX = 1024, g_WindowSizeY = 768;

std::unique_ptr<ShaderProgram> g_ShaderProgram;

void fn_SceneRenderer() {
	glClear(GL_COLOR_BUFFER_BIT);

	//---------------------
	g_ShaderProgram->Enable();

	static float l_Scale = 0.0f;
	l_Scale += 0.001f;

	Mat4f l_world = Translate(sinf(l_Scale), 0.0f, 0.0f); 
	glUniformMatrix4fv(g_WorldLocation, 1, GL_TRUE, &l_world.m[0]);

	glUniformMatrix4fv(g_PerspectiveLocation, 1, GL_TRUE, &g_PerspectiveMatrix.m[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO); //bind buffer again in preperation for draw call
	glVertexAttribPointer(	//tells the pipeline how to interpret the data inside the buffer
		0,					//Index of the attribute or shader being used. 0 for deafault/fixed pipeline 
		3,					//number of components in the attribute (such as 3 for X,Y,Z positions)
		GL_FLOAT,			//Data type of each component
		GL_FALSE,			// indicates whether we want our attribute to be normalized before it is used in the pipeline
		0,					//called the 'stride', is the number of bytes between two instances of that attribute in the buffer
		0);					//offset inside the structure where the pipeline will find our attribute

	glDrawArrays(		//call to draw the geometry
		GL_TRIANGLES,	//Specifies what kind of primitives to render
		0,				//Specifies the starting index in the enabled arrays
		3);				//Specifies the number of indices to be rendered
	glDisableVertexAttribArray(0);
	g_ShaderProgram->Disable();
	//---------------------

	glutSwapBuffers();	//Swap front and back framebuffers
}

void fn_InitializeGlutCallbacks() {
	glutDisplayFunc(fn_SceneRenderer);	//Set up rendering/drawing callback. Called once a frame
	glutIdleFunc(fn_SceneRenderer);		//allows renderer to be continuously called when events are not being received
}

int main(int argc, char *argv[]) {
	//set up glut
	glutInit(&argc, argv);	//initializes GLUT library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed)
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Advance Computer Animation Demo");

	fn_InitializeGlutCallbacks();

	//set up glew, must be done after glut is initialized
	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return -1;
	}
	
	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//Clear framebuffer using specified colour

	//Create vertex buffer
	Vec3f l_vertices[3];
	l_vertices[0] = Vec3f(-1.0f, -1.0f, 0.0f);
	l_vertices[1] = Vec3f(1.0f, -1.0f, 0.0f);
	l_vertices[2] = Vec3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &g_VBO);	//generates buffer object (in this case g_VBO). First parameter specifies the number of objects you want to create	
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);	//Bind handle to a target name. he target GL_ARRAY_BUFFER means that the buffer will contain an array of vertices  
	glBufferData(			//Fill buffer object with data. Call takes:
		GL_ARRAY_BUFFER,	// target name (same as what we used for binding)
		sizeof(l_vertices),	// size of the data in bytes
		l_vertices,			//address of the array of vertices
		GL_STATIC_DRAW);	//flag that indicates the usage pattern for this data. Since we are not going to change the buffer contents we specify GL_STATIC_DRAW
	
	//Create shader program
	g_ShaderProgram._Myptr() = new ShaderProgram();
	g_ShaderProgram->AddShader("shader.vs", GL_VERTEX_SHADER);
	g_ShaderProgram->AddShader("shader.fs", GL_FRAGMENT_SHADER);

	//g_ScaleLocation = g_ShaderProgram->GetUniformLocation("g_Scale");
	g_WorldLocation = g_ShaderProgram->GetUniformLocation("g_World");
	g_PerspectiveLocation = g_ShaderProgram->GetUniformLocation("g_Perspective");

	g_PerspectiveMatrix = Identity(); //Perspective(30.0f, g_WindowSizeX / g_WindowSizeY, 1.0f, 1000.0f);

	glutMainLoop();	 //passes control to GLUT which now begins its own internal loop

	return 0;
}

