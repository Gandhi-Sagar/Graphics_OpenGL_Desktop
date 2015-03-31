#include "CommonHeader.h"
#include "OGLControls.h"

class COGLWindow
{
	public:
			HWND			m_hWnd;
			COGLControl		m_oOGLControl;

			// Helper Functions
			void			ResetTimer();
			void			UpdateTimer();
			float			SpeedOptimizedFloat(float);

			//Functions while starting out
			bool			InitializeApp(wstring);
			void			RegisterAppClass(HINSTANCE hAppInstance);
			bool			CreateAppWindow(wstring);

			// Functions while terminating
			void			AppBody();
			void			ShutDown();

			// Getter and Setters
			HINSTANCE		GetInstance();
			
			// Callbacks
			LRESULT	CALLBACK	MsgHandler(HWND, UINT, WPARAM, LPARAM);

	private:
			// Control variables
			HINSTANCE		m_hInstance;
			wstring			m_sAppName;
			HANDLE			m_hMutex;

			// State variables
			bool			m_bAppActive;
			DWORD			m_dLastFrame;
			float			m_fFrameInterval;
};

namespace Keys
{
	int				Key(int);
	int				OneKey(int);
	extern char		kp[256];
}

extern COGLWindow appName;

void InitScene(LPVOID);
void RenderScene(LPVOID);
void ReleaseScene(LPVOID);