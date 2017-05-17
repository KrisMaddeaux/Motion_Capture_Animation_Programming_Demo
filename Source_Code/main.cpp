#include "include\GL\glew.h"		//OpenGL Extension Wrangler Library. Loads and provides easy access to all OpenGL extensions
#include "include\GL\freeglut.h"	//OpenGL Utility library. Provides simplified API for window management, event handeling, IO control, etc.

#include "SceneRenderer.h"
#include "MathClass.h"

const int g_WindowSizeX = 1024, g_WindowSizeY = 768;

std::unique_ptr<SceneRenderer> g_SceneRenderer;
std::shared_ptr<Camera> g_Camera;

void fn_SceneRenderer() {
	g_SceneRenderer->RenderScene();
}

void fn_KeyboardInput(unsigned char key, int x, int y) {
	g_Camera->KeyboardInput(key);
}

void fn_MouseInput(int x, int y) {
	g_Camera->MouseInput(x, y);
}

void fn_InitializeGlutCallbacks() {
	glutDisplayFunc(fn_SceneRenderer);	//Set up rendering/drawing callback. Called once a frame
	glutIdleFunc(fn_SceneRenderer);		//allows renderer to be continuously called when events are not being received
	glutKeyboardFunc(fn_KeyboardInput);
	glutMotionFunc(fn_MouseInput); 
}

int main(int argc, char *argv[]) {
	//set up glut
	glutInit(&argc, argv);	//initializes GLUT library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//GLUT_DOUBLE enables double buffering (drawing to a background buffer while another buffer is displayed)
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Advance Computer Animation Demo");

	//setup camera
	g_Camera = std::make_shared<Camera>(); 
	g_Camera->SetCameraPosition(0.0f, 0.0f, 2.0f);
	g_Camera->SetCameraTarget(0.0f, 0.0f, -3.0f);

	fn_InitializeGlutCallbacks();

	//set up glew, must be done after glut is initialized
	GLenum res = glewInit();
	if (res != GLEW_OK){
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return -1;
	}
	
	printf("GL version: %s\n", glGetString(GL_VERSION));

	g_SceneRenderer._Myptr() = new SceneRenderer(g_WindowSizeX, g_WindowSizeY, g_Camera);

	glutWarpPointer(g_WindowSizeX / 2.0f, g_WindowSizeY / 2.0f);	//once window opens have mouse go directly to middle of screen
	
	glutMainLoop();	 //passes control to GLUT which now begins its own internal loop

	return 0;
}

