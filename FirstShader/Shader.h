#include "CommonHeader.h"


class CShader
{
	public:
			CShader();

			bool	LoadShader(string, int);
			void	DeleteShader();

			bool	IsLoaded();
			UINT	GetShaderID();

	private:
			UINT	uiShader;
			int		iType;		//vertex or fragment shader
			bool	bLoaded;
};


class CShaderManager
{
	public:
			CShaderManager();
			void	CreateProgram();
			void	DeleteProgram();

			bool	AddShaderToProgram(CShader*);
			bool	LinkProgram();

			void	UseProgram();
			UINT	GetProgramID();

	private:
			UINT	uiProgram;
			bool	bLinked;
};

