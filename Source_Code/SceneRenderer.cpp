#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(float a_WindowSizeX, float a_WindowSizeY, std::shared_ptr<Camera> a_Camera) {
	m_elapsedTime = 0.0f;
	m_deltaTime = 0.0f;

	//Create and set up camera
	m_Camera = a_Camera;


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//Clear framebuffer using specified colour
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//Create shader program
	m_ShaderProgram._Myptr() = new ShaderProgram();
	m_ShaderProgram->AddShader("Shaders\\shader.vs", GL_VERTEX_SHADER);
	m_ShaderProgram->AddShader("Shaders\\shader.fs", GL_FRAGMENT_SHADER);

	//Set uniform variables so the CPU and GPU can communicate
	m_MVPLocation = m_ShaderProgram->GetUniformLocation("g_MVP");

	m_PerspectiveMatrix = Perspective(60.0f, a_WindowSizeX / a_WindowSizeY, 1.0f, 1000.0f);

	/////////////////////////////////////
	//LOAD ASSETS HERE
	/////////////////////////////////////

	//load animation skeleton from BVH file
	m_skeleton.LoadBVHFile("Animations\\walking_Take_001.bvh"); 

	//load model from OBJ file
	//m_model.LoadOBJFile("Models\\mainCharacter.obj");

}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::RenderScene() {
	UpdateScene();
	DrawScene();
}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::UpdateScene() {
	//Get elapsed and delta time 
	float l_newElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	m_deltaTime = l_newElapsedTime - m_elapsedTime;
	m_elapsedTime = l_newElapsedTime;

	//update skeleton
	m_skeleton.UpdateSkeleton(Vec3f(0.0f, -10.0f, 50.0f), 180.0f, m_deltaTime, false); 
}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::DrawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	//---------------------
	m_ShaderProgram->Enable();

	//draw skeleton
	m_skeleton.DrawSkeleton(m_MVPLocation, m_PerspectiveMatrix, m_Camera->GetViewMatrix());

	//draw model
	/*Mat4f l_world = Translate(0.0f, 0.0f, 0.0f);
	Mat4f l_WP = m_PerspectiveMatrix * m_Camera->GetViewMatrix() * l_world;
	glUniformMatrix4fv(m_MVPLocation, 1, GL_TRUE, &l_WP.m[0]);
	m_model.DrawOBJ();*/

	m_ShaderProgram->Disable();
	//---------------------

	glutSwapBuffers();	//Swap front and back framebuffers
}
//--------------------------------------------------------------------------------------------------------------------