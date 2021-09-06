#include "Shader.h"
#include "util.h"

using namespace std;

Shader::Shader(std::string&& VertexPath, string&& FragmentPath)
{
	//编译着色器
	unsigned int VertexShader, FragmentShader;
	int Result;
	char ErrMsg[512];

	const char *VertexShaderCode;
	const char *FragmentShaderCode;

	//从文件路径中获取顶点/片段着色器
	fstream FS;
	stringstream SS;
	string VertexStr, FragStr;

	//读取VertexShader
	FS.clear();
	SS.str("");
	FS.open(VertexPath, ios::in);
	if (!FS.is_open())
	{
		See("Vertex shader file open failed.");
		return;
	}
	SS << FS.rdbuf();
	VertexStr = SS.str();
	VertexShaderCode = VertexStr.c_str();
	FS.close();

	//读取FragmentShader
	FS.clear();
	SS.str("");
	FS.open(FragmentPath, ios::in);
	if (!FS.is_open())
	{
		See("Fragment shader file open failed.");
		return;
	}
	SS << FS.rdbuf();
	FragStr = SS.str();
	FragmentShaderCode = FragStr.c_str();
	FS.close();

	// 顶点着色器
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	if(!VertexShader)
	{
		See("Vertex shader create failed.");
	}
	glShaderSource(VertexShader, 1, &VertexShaderCode, nullptr);
	glCompileShader(VertexShader);

	// 打印编译错误（如果有的话）
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, ErrMsg);
		See(ErrMsg);
		return;
	}
	See("Vertext shader compile OK.");


	// 片段着色器
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if(!FragmentShader)
	{
		See("Fragment shader create failed.");
	}
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, nullptr);
	glCompileShader(FragmentShader);

	// 打印编译错误（如果有的话）
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, ErrMsg);
		See(ErrMsg);
		return;
	}
	See("Fragment shader compile OK.");

	// 着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glLinkProgram(ID);

	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, ErrMsg);
		See(ErrMsg);
		return;
	}

	//标记创建成功
	bIsValid = true;
}

bool Shader::IsValid()
{
	return bIsValid;
}

void Shader::Use()
{
	glUseProgram(ID);
}

unsigned int Shader::GetID()
{
	return ID;
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
