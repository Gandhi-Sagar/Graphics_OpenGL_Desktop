#include "CommonHeader.h"
#include "OGLWindow.h"

void InitScene(LPVOID lParam)
{
	// Initialize your opengl state variables here
	// these generally last 1st render frame, then you can update if needed

	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
}

void RenderScene(LPVOID lParam)
{
	COGLControl		*oglControl = (COGLControl*)lParam;

	glClear(GL_COLOR_BUFFER_BIT);
	oglControl->SwapTwoBuffers();
}