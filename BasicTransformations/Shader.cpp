#include "Shader.h"

CShader::CShader()
{
	bLoaded = false;
}


bool	CShader::LoadShader(string sFile, int a_shaderType)
{
	FILE* fp = NULL;
	fopen_s(&fp, sFile.c_str(), "rt");
	if(! fp)
		return false;

	vector<string> sLines;
	char sSingleLine[255];
	while( fgets(sSingleLine, 255, fp))
		sLines.push_back(sSingleLine);
	fclose(fp);

	const char** sProgram = new const char*[sLines.size()];
	for(unsigned int i = 0; i < sLines.size() ; i++)
		sProgram[i] = sLines[i].c_str();

	uiShader = glCreateShader(a_shaderType);
	glShaderSource(uiShader,sLines.size(), sProgram, NULL);
	glCompileShader(uiShader);

	delete[] sProgram;
	int iCompilationStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iCompilationStatus);

	if(iCompilationStatus == GL_FALSE)
		return false;

	iType = a_shaderType;
	bLoaded = true;

	return true;
}

bool	CShader::IsLoaded()
{
	return bLoaded;
}

UINT	CShader::GetShaderID()
{
	return uiShader;
}

void	CShader::DeleteShader()
{
	if(! IsLoaded())
		return;
	bLoaded = false;
	glDeleteShader(uiShader);
}





		CShaderManager::CShaderManager()
{
	bLinked = false;
}

void	CShaderManager::CreateProgram()
{
	uiProgram = glCreateProgram();
}

bool	CShaderManager::AddShaderToProgram(CShader* shShader)
{
	if(! shShader->IsLoaded())
		return false;

	glAttachShader(uiProgram, shShader->GetShaderID());
	return true;
}

bool	CShaderManager::LinkProgram()
{
	glLinkProgram(uiProgram);
	int iLinkStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iLinkStatus);
	bLinked = iLinkStatus == GL_TRUE;
	return true;
}

void	CShaderManager::DeleteProgram()
{
	if(! bLinked)
		return;
	bLinked = false;
	glDeleteProgram(uiProgram);
}

void	CShaderManager::UseProgram()
{
	if(bLinked)
		glUseProgram(uiProgram);
}

UINT	CShaderManager::GetProgramID()
{
	return uiProgram;
}