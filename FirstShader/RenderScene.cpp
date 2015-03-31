#include "CommonHeader.h"
#include "OGLWindow.h"
#include "Shader.h"

float	fTriangleVertices[9];
float	fQuadVertices[12];
float	fTriangleColor[9];
float	fQuadColor[12];

UINT	uiVBO[4];
UINT	uiVAO[2];

CShader				shVertex, shFragment;
CShaderManager		smMain;

void InitScene(LPVOID lParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// triangle
	fTriangleVertices[0] = -0.4f; fTriangleVertices[1] = 0.1f; fTriangleVertices[2] = 0.0f;
	fTriangleVertices[3] = 0.4f; fTriangleVertices[4] = 0.1f; fTriangleVertices[5] = 0.0f;
	fTriangleVertices[6] = 0.0f; fTriangleVertices[7] = 0.7f; fTriangleVertices[8] = 0.0f;

	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;

	//Quad
	fQuadVertices[0] = -0.2f; fQuadVertices[1] = -0.1f; fQuadVertices[2] = 0.0f;
	fQuadVertices[3] = -0.2f; fQuadVertices[4] = -0.6f; fQuadVertices[5] = 0.0f;
	fQuadVertices[6] = 0.2f; fQuadVertices[7] = -0.1f; fQuadVertices[8] = 0.0f;
	fQuadVertices[9] = 0.2f; fQuadVertices[10] = -0.6f; fQuadVertices[11] = 0.0f;

	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[5] = 0.0f;
	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[8] = 1.0f;
	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

	glGenVertexArrays(2, uiVAO);
	glGenBuffers(4, uiVBO);

	glBindVertexArray(uiVAO[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(uiVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	shVertex.LoadShader("shaders/hello_world.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("shaders/hello_world.frag", GL_FRAGMENT_SHADER);
	smMain.CreateProgram();
	smMain.AddShaderToProgram(&shVertex);
	smMain.AddShaderToProgram(&shFragment);

	smMain.LinkProgram();
	smMain.UseProgram();
}

void RenderScene(LPVOID lParam)
{
	COGLControl		*oglControl = (COGLControl*)lParam;

	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindVertexArray(uiVAO[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(uiVAO[1]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	oglControl->SwapTwoBuffers();
}

void ReleaseScene(LPVOID lParam)
{
	smMain.DeleteProgram();

	shVertex.DeleteShader();
	shFragment.DeleteShader();

	glDeleteVertexArrays(2, uiVAO);
	glDeleteBuffers(4, uiVBO);
}




