#include "CommonHeader.h"
#include "OGLControls.h"

#include <GL/wglew.h>

bool COGLControl::s_bClassRegistered = false;
bool COGLControl::s_bGlewInitialized = false;

LRESULT CALLBACK MsgHandlerOGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch(uiMsg)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}

void COGLControl::RegisterOpenGLClass(HINSTANCE hInstance)
{
	if(s_bClassRegistered)
		return;
	WNDCLASSEX  wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wcex.lpfnWndProc = (WNDPROC) MsgHandlerOGLClass;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_MENUBAR+1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = OGL_CONTROL_CLASS;

	RegisterClassEx(&wcex);
	 
	s_bClassRegistered = true;
}

bool COGLControl::InitGLEW(HINSTANCE hInstance)
{
	if(s_bGlewInitialized)
		return true;
	RegisterOpenGLClass(hInstance);

	HWND hWndFake = CreateWindow(OGL_CONTROL_CLASS, L"FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	m_hDC = GetDC(hWndFake);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if(iPixelFormat == 0)
		return false;

	if(! SetPixelFormat(m_hDC, iPixelFormat, &pfd))
		return false;

	HGLRC hRCFake = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, hRCFake);

	bool bResult = true;

	if(! s_bGlewInitialized)
	{
		if(glewInit() != GLEW_OK)
		{
			MessageBox(*m_phWnd, L"Could not initialize GLEW", L"Fatal Error", MB_ICONERROR);
			bResult = false;
		}
		s_bGlewInitialized = true;
	}
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(hWndFake);

	return bResult;
}

bool COGLControl::InitOpenGL(HINSTANCE hInstance, HWND* hWnd, int iMajorVersion, int iMinorVersion, void (*fn_InitScene)(LPVOID), void (*fn_RenderScene)(LPVOID), void(*fn_ReleaseScene)(LPVOID), LPVOID lParam)
{
	if( !InitGLEW(hInstance))
		return false;

	m_phWnd = hWnd;
	m_hDC = GetDC(*m_phWnd);

	bool bError = false;
	PIXELFORMATDESCRIPTOR pfd;
	if(iMajorVersion <= 2)
	{
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		if(iPixelFormat == 0)
			return false;
		if(! SetPixelFormat(m_hDC, iPixelFormat, &pfd))
			return false;

		m_hGLRC = wglCreateContext(m_hDC);
		if(m_hGLRC)
			wglMakeCurrent(m_hDC, m_hGLRC);
		else 
			bError = true;
	}
	else if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
	{
		const int iPixelFormatAttributeList[] = 
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0
		};
		int iContextAttributes[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};

		int iPixelFormat, iNumFormat;
		wglChoosePixelFormatARB(m_hDC, iPixelFormatAttributeList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormat);

		if( ! SetPixelFormat(m_hDC, iPixelFormat, &pfd))
			return false;

		m_hGLRC = wglCreateContextAttribsARB(m_hDC, 0, iContextAttributes);
		if(m_hGLRC)
			wglMakeCurrent(m_hDC, m_hGLRC);
		else
			bError = true;

	}
	else bError = true;

	if(bError)
	{
		wchar_t sErrorMsg[255], sErrorTitle[255];
		swprintf_s(sErrorMsg, L"OpenGL %d %d is not supported ", iMajorVersion, iMinorVersion);
		swprintf_s(sErrorTitle, L"OLD Graphcis Card Detected");
		MessageBox(*m_phWnd, sErrorMsg, sErrorTitle, MB_ICONINFORMATION);
		return false;
	}

	RenderScene = fn_RenderScene;
	InitScene = fn_InitScene;
	ReleaseScene = fn_ReleaseScene;

	if(InitScene != NULL)
		InitScene(lParam);

	return true;
}


void COGLControl::Render(LPVOID lParam)
{
	if(RenderScene)
		RenderScene(lParam);
}


void COGLControl::ResizeOpenGLViewportFull()
{
	if(m_phWnd == NULL)
		return;

	RECT rRect;
	GetClientRect(*m_phWnd, &rRect);
	glViewport(0, 0, rRect.right, rRect.bottom);
}


void COGLControl::ReleaseOpenGLControl(LPVOID lParam)
{
	if(ReleaseScene)
		ReleaseScene(lParam);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hGLRC);
	ReleaseDC(*m_phWnd, m_hDC);

	m_phWnd = NULL;
}



void COGLControl::UnregisterOpenGLClass(HINSTANCE hInstance)
{
	if(s_bClassRegistered)
	{
		UnregisterClass(OGL_CONTROL_CLASS, hInstance);
		s_bClassRegistered = false;
	}
}

void COGLControl::SwapTwoBuffers()
{
	SwapBuffers(m_hDC);
}

void COGLControl::MakeCurrent()
{
	wglMakeCurrent(m_hDC, m_hGLRC);
}