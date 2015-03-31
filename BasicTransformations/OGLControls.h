#pragma once
#define OGL_CONTROL_CLASS L"OGL_CONTROL_CLASS"

#include <glm/gtc/matrix_transform.hpp>

class COGLControl
{
	public:
			COGLControl::COGLControl();

			bool			InitOpenGL(HINSTANCE, HWND*, int iMajorVersion, int iMinorVersion, void (*fn_InitScene)(LPVOID), void (*fn_RenderScene)(LPVOID), void (*fn_ReleaseScene) (LPVOID), LPVOID lParam);
			
			void			ResizeOpenGLViewportFull();
			void			SetProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar);
			glm::mat4*		GetProjectionMatrix();
			void			Render(LPVOID lParam);
			void			ReleaseOpenGLControl(LPVOID lParam);

			// To be called without object, global functions
			static void		RegisterOpenGLClass(HINSTANCE);
			static void		UnregisterOpenGLClass(HINSTANCE);

			void			MakeCurrent();
			void			SwapTwoBuffers();

			bool			SetVerticalSync(bool);
			int				GetFPS();

	private:
			// Context stoarage
			HDC				m_hDC;
			HWND			*m_phWnd;
			HGLRC			m_hGLRC;

			// GLSL version
			int				m_iMajorVersion;
			int				m_iMinorVersion;
			int				m_iFPSCount;
			int				m_iCurrentFPS;
			clock_t			m_tLastSecond;

			// matrices

			glm::mat4		m_mProjection;

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