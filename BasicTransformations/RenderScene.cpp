#include "CommonHeader.h"
#include "OGLWindow.h"
#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*float	fTriangleVertices[9];
float	fQuadVertices[12];
float	fTriangleColor[9];
float	fQuadColor[12];*/

float	fPyramidVertices[36];
float	fPyramidColors[36];

UINT	uiVBO[2];
UINT	uiVAO[1];

bool	bShowFPS = false;
bool	bVerticalSync = false;

CShader				shVertex, shFragment;
CShaderManager		smMain;

void InitScene(LPVOID lParam)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// front face
	fPyramidVertices[0] = 0.0f; fPyramidVertices[1] = 5.0f; fPyramidVertices[2] = 0.0f;
	fPyramidVertices[3] = -3.0f; fPyramidVertices[4] = 0.0f; fPyramidVertices[5] = 3.0f;
	fPyramidVertices[6] = 3.0f; fPyramidVertices[7] = 0.0f; fPyramidVertices[8] = 3.0f;

	// back
	fPyramidVertices[9] = 0.0f; fPyramidVertices[10] = 5.0f; fPyramidVertices[11] = 0.0f;
	fPyramidVertices[12] = 3.0f; fPyramidVertices[13] = 0.0f; fPyramidVertices[14] = -3.0f;
	fPyramidVertices[15] = -3.0f; fPyramidVertices[16] = 0.0f; fPyramidVertices[17] = -3.0f; 

	// left
	fPyramidVertices[18] = 0.0f; fPyramidVertices[19] = 5.0f; fPyramidVertices[20] = 0.0f;
	fPyramidVertices[21] = -3.0f; fPyramidVertices[22] = 0.0f; fPyramidVertices[23] = -3.0f;
	fPyramidVertices[24] = -3.0f; fPyramidVertices[25] = 0.0f; fPyramidVertices[26] = 3.0f;

	// right
	fPyramidVertices[27] = 0.0f; fPyramidVertices[28] = 5.0f; fPyramidVertices[29] = 0.0f;
	fPyramidVertices[30] = 3.0f; fPyramidVertices[31] = 0.0f; fPyramidVertices[32] = 3.0f;
	fPyramidVertices[33] = 3.0f; fPyramidVertices[34] = 0.0f; fPyramidVertices[35] = -3.0f;

	// color for pyramid

	for (int i = 0; i < 4; i++)
	{
		fPyramidColors[i * 9] = 1.0f; fPyramidColors[i * 9 + 1] = 0.0f; fPyramidColors[i * 9 + 2] = 0.0f;
		if (i < 2)
		{
			fPyramidColors[i * 9 + 1] = 0.0f; fPyramidColors[i * 9 + 4] = 1.0f; fPyramidColors[i * 9+ 5] = 0.0f;
			fPyramidColors[i * 9 + 2] = 0.0f; fPyramidColors[i * 9 + 7] = 0.0f; fPyramidColors[i * 9 + 8] = 1.0f;
		}
		else
		{
			fPyramidColors[i * 9 + 2] = 0.0f; fPyramidColors[i * 9 + 4] = 0.0f; fPyramidColors[i * 9 + 5] = 1.0f;
			fPyramidColors[i * 9 + 2] = 0.0f; fPyramidColors[i * 9 + 7] = 1.0f; fPyramidColors[i * 9 + 8] = 0.0f;
		}
	}

	glGenVertexArrays(1, uiVAO);
	glGenBuffers(2, uiVBO);

	glBindVertexArray(uiVAO[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramidVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramidColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	shVertex.LoadShader("transformations.vert", GL_VERTEX_SHADER);
	shFragment.LoadShader("Transformations.frag", GL_FRAGMENT_SHADER);
	smMain.CreateProgram();
	smMain.AddShaderToProgram(&shVertex);
	smMain.AddShaderToProgram(&shFragment);

	smMain.LinkProgram();
	smMain.UseProgram();

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
}

float			fRotationAngle = 0.0f;
const float		PIby180 = 3.1416f / 180.0f;

void RenderScene(LPVOID lParam)
{
	COGLControl		*oglControl = (COGLControl*)lParam;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(uiVAO[0]);
	int iModelviewLoc = glGetUniformLocation(smMain.GetProgramID(), "modelviewMatrix");
	int iProjectionLoc = glGetUniformLocation(smMain.GetProgramID(), "projectionMatrix");
	glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*oglControl->GetProjectionMatrix()));
	
	glm::mat4 mModelView = glm::lookAt(glm::vec3(0, 15, 40), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// pyramid in the middle
	glm::mat4 mCurrent = glm::rotate(mModelView, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(iModelviewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// left 
	mCurrent = glm::translate(mModelView, glm::vec3(-20.0f, 10.0f * float(sin(fRotationAngle * PIby180)), 0.0f));
	glUniformMatrix4fv(iModelviewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// right
	mCurrent = glm::translate(mModelView, glm::vec3(20.0f, -10.0f * float(sin(fRotationAngle * PIby180)), 0.0f));
	glUniformMatrix4fv(iModelviewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	//top
	mCurrent = glm::translate(mModelView, glm::vec3(20.0f * float(sin(fRotationAngle * PIby180)), 10.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(iModelviewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// scaling and rotating

	float fScaleVal = 1.5f + float(sin(fRotationAngle * PIby180)) * 0.5f;
	mCurrent = glm::translate(mModelView, glm::vec3(0.0f, -10.0f, 0.0f));
	mCurrent = glm::scale(mCurrent, glm::vec3(fScaleVal, fScaleVal, fScaleVal));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(iModelviewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));
	glDrawArrays(GL_TRIANGLES, 0, 12);

	fRotationAngle += appMain.SpeedOptimizedFloat(120.0f);

	if (Keys::OneKey(VK_F2))
	{
		bShowFPS = !bShowFPS;
		if (!bShowFPS)
			SetWindowText(appMain.m_hWnd, L"Roating triangles");
	}

	if (Keys::OneKey(VK_F3))
	{
		bVerticalSync = !bVerticalSync;
		oglControl->SetVerticalSync(bVerticalSync);
	}

	if (bShowFPS)
	{
		wchar_t buf[55];
		swprintf_s(buf, L"FPS: %d, V-Sync: %s", oglControl->GetFPS(), bVerticalSync ? L"on": L"off");
		SetWindowText(appMain.m_hWnd, buf);
	}

	oglControl->SwapTwoBuffers();
}

void ReleaseScene(LPVOID lParam)
{
	smMain.DeleteProgram();

	glDeleteVertexArrays(1, uiVAO);
	glDeleteBuffers(2, uiVBO);

	shVertex.DeleteShader();
	shFragment.DeleteShader();

	
}




