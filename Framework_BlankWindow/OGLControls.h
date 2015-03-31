#pragma once
#define OGL_CONTROL_CLASS L"OGL_CONTROL_CLASS"

class COGLControl
{
	public:
			// Main Controlling functions
			bool			InitOpenGL(HINSTANCE, HWND*, int iMajorVersion, int iMinorVersion, void (*fn_InitScene)(LPVOID), void (*fn_RenderScene)(LPVOID), void (*fn_ReleaseScene) (LPVOID), LPVOID lParam);
			
			void			ResizeOpenGLViewportFull();
			void			Render(LPVOID lParam);
			void			ReleaseOpenGLControl(LPVOID lParam);

			// To be called without object, global functions
			static void		RegisterOpenGLClass(HINSTANCE);
			static void		UnregisterOpenGLClass(HINSTANCE);

			// Helper Functions without which you can't render 
			void			MakeCurrent();
			void			SwapTwoBuffers();

	private:
			// Context stoarage
			HDC				m_hDC;
			HWND			*m_phWnd;
			HGLRC			m_hGLRC;

			// GLSL version
			int				iMajorVersion;
			int				iMinorVersion;

			// variables conveying state to external world
			static bool		s_bClassRegistered;
			static bool		s_bGlewInitialized;

			// Function POinters
			void			(*InitScene)(LPVOID);
			void			(*RenderScene)(LPVOID);
			void			(*ReleaseScene)(LPVOID);

			bool			InitGLEW(HINSTANCE);

};

LRESULT		CALLBACK	MsgHandlerOGLClass(HWND, UINT, WPARAM, LPARAM);