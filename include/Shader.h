#pragma once

#include <PVR_PSP2/GLES2/gl2.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	
	// 构造器读取并构建着色器
	Shader(std::string&& VertexPath, std::string&& FragmentPath);

	// 使用/激活程序
	void Use();

	// 获取程序ID
	unsigned int GetID();

	// 删除着色器
	void Delete();

	// 传递参数
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

	// 程序ID
	unsigned int ID;

	// 是否初始化成功
	bool bSuccessfulInit = false;

	// 是否编译成功
	bool bSuccessfulCompile = false;

	// 顶点着色器
	std::string VertexShaderCode;
	
	// 片段着色器
	std::string FragmentShaderCode;

};
