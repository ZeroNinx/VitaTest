#pragma once

#include <vitaGL.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	
	// ��������ȡ��������ɫ��
	Shader(std::string&& VertexPath, std::string&& FragmentPath, std::string& Log);

	// ʹ��/�������
	void Use();

	// ��ȡ����ID
	unsigned int GetID();

	// ɾ����ɫ��
	void Delete();

	// ���ݲ���
	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

public:

	// ����ID
	unsigned int ID;

	// �Ƿ��ʼ���ɹ�
	bool bSuccessfulInit = false;

	// �Ƿ����ɹ�
	bool bSuccessfulCompile = false;

	// ������ɫ��
	std::string VertexShaderCode;
	
	// Ƭ����ɫ��
	std::string FragmentShaderCode;

};
