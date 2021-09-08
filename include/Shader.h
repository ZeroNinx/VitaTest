#pragma once

#include <PVR_PSP2/GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	
	Shader();
	Shader(std::string&& VertexPath, std::string&& FragmentPath);

	//返回是否有效
	bool IsValid();

	// 使用/激活程序
	void Use();

	// 获取程序ID
	unsigned int GetID();

	// 删除着色器
	void Delete();

	//Attrubute参数
	void BindAttribute(const std::string& Name, GLuint Location)
	{
		glBindAttribLocation(ID, Location, Name.c_str());
	}

	// Uniform参数
	void UniformInt(const std::string& Name, int Value) const
	{
		glUniform1i(glGetUniformLocation(ID, Name.c_str()), Value);
	}
	void UniformFloat(const std::string& Name, float Value) const
	{
		glUniform1f(glGetUniformLocation(ID, Name.c_str()), Value);
	}
	void UniformFloatMat4(const std::string& Name, glm::mat4& Value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
	}
	void UniformFloatVec3(const std::string& Name, glm::vec3& Value) const
	{
		glUniform3fv(glGetUniformLocation(ID, Name.c_str()), 1, glm::value_ptr(Value));
	}
	void UniformFloatVec4(const std::string& Name, glm::vec4& Value) const
	{
		glUniform4fv(glGetUniformLocation(ID, Name.c_str()), 1, glm::value_ptr(Value));
	}

private:

	// 程序ID
	unsigned int ID;

	// 是否初始化成功
	bool bIsValid = false;

	// 顶点着色器
	std::string VertexShaderCode;
	
	// 片段着色器
	std::string FragmentShaderCode;

};
