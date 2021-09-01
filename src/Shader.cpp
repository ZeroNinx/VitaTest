#include "Shader.h"

using namespace std;

Shader::Shader(std::string&& VertexPath, string&& FragmentPath,string& Log)
{
	//������ɫ��
	unsigned int VertexShader, FragmentShader;
	int Result;
	char ErrMsg[512];

	const char *VertexShaderCode;
	const char *FragmentShaderCode;

	//���ļ�·���л�ȡ����/Ƭ����ɫ��
	fstream FS;
	stringstream SS;
	string VertexStr, FragStr;

	//��ȡVertexShader
	FS.clear();
	SS.str("");
	FS.open(VertexPath, ios::in);
	if (!FS.is_open())
	{
		Log += ("Vertex shader file open failed.\n");
		return;
	}
	SS << FS.rdbuf();
	VertexStr = SS.str();
	VertexShaderCode = VertexStr.c_str();
	FS.close();

	//��ȡFragmentShader
	FS.clear();
	SS.str("");
	FS.open(FragmentPath, ios::in);
	if (!FS.is_open())
	{
		Log += ("Fragment shader file open failed.\n");
		return;
	}
	SS << FS.rdbuf();
	FragStr = SS.str();
	FragmentShaderCode = FragStr.c_str();
	FS.close();

	// ������ɫ��
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &VertexShaderCode, NULL);
	glCompileShader(VertexShader);

	// ��ӡ�����������еĻ���
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, ErrMsg);
		Log += "Vertex shader compile failed: \n";
		Log += ErrMsg;
		Log+="\n";
		return;
	}

	// Ƭ����ɫ��
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &FragmentShaderCode, NULL);
	glCompileShader(FragmentShader);

	// ��ӡ�����������еĻ���
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, ErrMsg);
		Log += "Fragment shader compile failed: \n";
		Log += ErrMsg;
		return;
	}

	// ��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, VertexShader);
	glAttachShader(ID, FragmentShader);
	glLinkProgram(ID);

	// ��ӡ���Ӵ�������еĻ���
	glGetProgramiv(ID, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, ErrMsg);
		Log += "Shader program linking failed: ";
		Log += ErrMsg;
		return;
	}

	//��Ǵ����ɹ�
	bSuccessfulInit = true;
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
