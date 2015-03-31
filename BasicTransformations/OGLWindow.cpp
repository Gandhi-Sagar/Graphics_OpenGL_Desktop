#include "CommonHeader.h"
#include "OGLWindow.h"

COGLWindow	appMain;
char Keys::kp[256] = {0};

bool	COGLWindow::InitializeApp(wstring sAppName)
{
	m_sAppName = sAppName;
	m_hMutex = CreateMutex(NULL, 1, m_sAppName.c_str());
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, L"Applcation is already Running", L"Already Running", MB_ICONINFORMATION | MB_OK);
		return 0;
	}
	return 1;
}

LRESULT	CALLBACK	COGLWindow::MsgHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	switch(uiMsg)
	{
		case WM_PAINT:
			BeginPaint(hWnd, &ps);					
			EndPaint(hWnd, &ps);
			break;

		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		case WM_ACTIVATE:
		{
			switch(LOWORD(wParam))
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					m_bAppActive = true;
					break;
				case WA_INACTIVE:
					m_bAppActive = false;
					break;
			}
			break;
		}

		case WM_SIZE:
			m_oOGLControl.ResizeOpenGLViewportFull();
			m_oOGLControl.SetProjection3D(45.0f, float(LOWORD(lParam)) / float(HIWORD(lParam)), 0.001f, 1000.0f);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}


LRESULT CALLBACK GlobalMessageHandler(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	return appMain.MsgHandler(hWnd, uiMsg, wParam, lParam);
}

void	COGLWindow::RegisterAppClass(HINSTANCE hInstance)
{
	WNDCLASSEX		wndclsex;
	memset(&wndclsex, 0, sizeof(WNDCLASSEX));

	wndclsex.cbSize = sizeof(WNDCLASSEX);
	wndclsex.style = CS_OWNDC;
	
	wndclsex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	wndclsex.hIcon = LoadIcon(m_hInstance, IDI_WINLOGO);
	wndclsex.hIconSm = LoadIcon(m_hInstance, IDI_WINLOGO);
	wndclsex.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
	wndclsex.hInstance = m_hInstance;

	wndclsex.lpfnWndProc = GlobalMessageHandler;
	wndclsex.lpszClassName = m_sAppName.c_str();

	wndclsex.lpszMenuName = NULL;

	RegisterClassEx(&wndclsex);
	DWORD err = GetLastError();
}

bool COGLWindow::CreateAppWindow(wstring sWindowName)
{
	m_hWnd = CreateWindowEx(0, m_sAppName.c_str(), sWindowName.c_str(), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
		0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
		NULL, m_hInstance, NULL);

	DWORD err = GetLastError();
	
	if(! m_oOGLControl.InitOpenGL(m_hInstance, &m_hWnd, 3, 1, InitScene, RenderScene, ReleaseScene, &m_oOGLControl))
		return false;
	ShowWindow(m_hWnd, SW_SHOW);
	ShowWindow(m_hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(m_hWnd);

	return true;
}

void COGLWindow::ResetTimer()
{
	m_dLastFrame = GetTickCount();
	m_fFrameInterval = 0.0f;
}

void COGLWindow::UpdateTimer()
{
	DWORD dwCurr = GetTickCount();
	m_fFrameInterval = float(dwCurr - m_dLastFrame) * 0.001f;
	m_dLastFrame = dwCurr;
}

void COGLWindow::AppBody()
{
	MSG msg;
	while (1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if(m_bAppActive)
		{
			UpdateTimer();
			m_oOGLControl.Render(&m_oOGLControl);
		}
		else 
			Sleep(200);
	}
}

void COGLWindow::ShutDown()
{
	m_oOGLControl.ReleaseOpenGLControl(&m_oOGLControl);

	DestroyWindow(m_hWnd);
	UnregisterClass(m_sAppName.c_str(), m_hInstance);
	COGLControl::UnregisterOpenGLClass(m_hInstance);
	ReleaseMutex(m_hMutex);
}

float COGLWindow::SpeedOptimizedFloat(float fVal)
{
	return fVal * m_fFrameInterval;
}

HINSTANCE COGLWindow::GetInstance()
{
	return m_hInstance;
}

int Keys::Key(int iKey)
{
	return (GetAsyncKeyState(iKey) >> 15)&1;
}

int Keys::OneKey(int iKey)
{
	if(Key(iKey) && !kp[iKey])
	{
		kp[iKey] = 1;
		return 1;
	}

	if(! Key(iKey))
		kp[iKey] = 0;

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if(! appMain.InitializeApp(L"Simple Triangle Using Shader"))
		return 0;
	appMain.RegisterAppClass(hInstance);

	if( !appMain.CreateAppWindow(L"Simple Triangle Using Shader"))
		return 0;
	appMain.ResetTimer();
	appMain.AppBody();
	appMain.ShutDown();

	return 0;
}
