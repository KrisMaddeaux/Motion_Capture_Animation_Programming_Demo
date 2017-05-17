#include "SceneRenderer.h"

SceneRenderer::SceneRenderer(float a_WindowSizeX, float a_WindowSizeY, std::shared_ptr<Camera> a_Camera) {
	//Create and set up camera
	m_Camera = a_Camera;


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	//Clear framebuffer using specified colour

											//Create vertex buffer
	Vec3f l_vertices[4];
	l_vertices[0] = Vec3f(-1.0f, -1.0f, 0.0f);
	l_vertices[1] = Vec3f(0.0f, -1.0f, 1.0f);
	l_vertices[2] = Vec3f(1.0f, -1.0f, 0.0f);
	l_vertices[3] = Vec3f(0.0f, 1.0f, 0.0f);

	glGenBuffers(1, &m_VBO);	//generates buffer object (in this case g_VBO). First parameter specifies the number of objects you want to create	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);	//Bind handle to a target name. he target GL_ARRAY_BUFFER means that the buffer will contain an array of vertices  
	glBufferData(			//Fill buffer object with data. Call takes:
		GL_ARRAY_BUFFER,	// target name (same as what we used for binding)
		sizeof(l_vertices),	// size of the data in bytes
		l_vertices,			//address of the array of vertices
		GL_STATIC_DRAW);	//flag that indicates the usage pattern for this data. Since we are not going to change the buffer contents we specify GL_STATIC_DRAW

							//Create Index buffer
	unsigned int l_indices[] = { 0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(l_indices), l_indices, GL_STATIC_DRAW);


	//Create shader program
	m_ShaderProgram._Myptr() = new ShaderProgram();
	m_ShaderProgram->AddShader("shader.vs", GL_VERTEX_SHADER);
	m_ShaderProgram->AddShader("shader.fs", GL_FRAGMENT_SHADER);

	//Set uniform variables so the CPU and GPU can communicate
	m_MVPLocation = m_ShaderProgram->GetUniformLocation("g_MVP");

	m_PerspectiveMatrix = Perspective(60.0f, a_WindowSizeX / a_WindowSizeY, 1.0f, 100.0f);

}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::RenderScene() {
	UpdateScene();
	DrawScene();
}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::UpdateScene() {


}
//--------------------------------------------------------------------------------------------------------------------
void SceneRenderer::DrawScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	//---------------------
	m_ShaderProgram->Enable();

	static float l_Scale = 0.0f;
	l_Scale += 0.1f;

	Mat4f l_world = Translate(0.0f, 0.0f, 5.0f) * Rotate(0.0f, l_Scale, 0.0f);
	Mat4f l_WP = m_PerspectiveMatrix * m_Camera->GetViewMatrix() * l_world;
	glUniformMatrix4fv(m_MVPLocation, 1, GL_TRUE, &l_WP.m[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); //bind buffer again in preperation for draw call
	glVertexAttribPointer(	//tells the pipeline how to interpret the data inside the buffer
		0,					//Index of the attribute or shader being used. 0 for deafault/fixed pipeline 
		3,					//number of components in the attribute (such as 3 for X,Y,Z positions)
		GL_FLOAT,			//Data type of each component
		GL_FALSE,			// indicates whether we want our attribute to be normalized before it is used in the pipeline
		0,					//called the 'stride', is the number of bytes between two instances of that attribute in the buffer
		0);					//offset inside the structure where the pipeline will find our attribute

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	m_ShaderProgram->Disable();
	//---------------------

	glutSwapBuffers();	//Swap front and back framebuffers
}
//--------------------------------------------------------------------------------------------------------------------