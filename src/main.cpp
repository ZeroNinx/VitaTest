#include <vitasdk.h>
#include <vitaGL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "debugScreen.h"

#include "util.h"

using namespace std;

//日志
string GlobalLog = "";

Shader *DrawShader;
unsigned int VBO, VAO, EBO;
unsigned Texture, Texture2;
int cnt = 0;

float vertices[] =
	{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

//每个盒子的位移
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)};

void PrintMsg(const char *Msg)
{
	GlobalLog += Msg;
	vglEnd();
	psvDebugScreenInit();
	psvDebugScreenPrintf("%s", GlobalLog.c_str());
	sceKernelDelayThread(300 * 1000000);
	sceKernelExitProcess(0);
}

int main()
{

	stbi_set_flip_vertically_on_load(true);

	//初始化图形
	vglInit(0x800000);

	//启用运行时编译Shader
	vglEnableRuntimeShaderCompiler(GL_TRUE);

	//开启深度缓冲区
	glEnable(GL_DEPTH_TEST);

	//初始化着色器
	DrawShader = new Shader(GetContentPath("Shader/VertexShader.cg"), GetContentPath("Shader/FragmentShader.cg"), GlobalLog);
	if (!DrawShader->bSuccessfulInit)
	{
		PrintMsg("Shader Compile Failed");
		return 0;
	}
	DrawShader->Use();

	//绑定Location对应的变量
	glBindAttribLocation(DrawShader->GetID(), 0, "aPos");
	glBindAttribLocation(DrawShader->GetID(), 1, "aTexCoord");

	//创建VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GlobalLog += "VBO generate success\n";

	//读取纹理
	int TextureWidth, TextureHeight, TextureChannel;
	unsigned char *Data = stbi_load(GetContentPath("box.jpg").c_str(), &TextureWidth, &TextureHeight, &TextureChannel, 0);
	if (!Data)
	{
		PrintMsg("Box texture read Failed");
		return 0;
	}

	//创建纹理
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureWidth, TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(Texture);
	stbi_image_free(Data);

	//导入纹理
	glUniform1i(glGetUniformLocation(DrawShader->GetID(), "BoxTexture"), 0);

	GlobalLog += "Box texture generate success\n";

	//读取纹理2
	Data = stbi_load(GetContentPath("face.png").c_str(), &TextureWidth, &TextureHeight, &TextureChannel, 0);
	if (!Data)
	{
		PrintMsg("Face texture read Failed");
		return 0;
	}

	//创建纹理2
	glGenTextures(1, &Texture2);
	glBindTexture(GL_TEXTURE_2D, Texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	glGenerateMipmap(Texture2);
	stbi_image_free(Data);

	//导入纹理2
	glUniform1i(glGetUniformLocation(DrawShader->GetID(), "FaceTexture"), 1);

	GlobalLog += "Face texture generate success\n";

	//模型矩阵（内部坐标->世界坐标）
	glm::mat4 ModelMat(1.0f);
	ModelMat = glm::rotate(ModelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0));

	//相机的反向矩阵（世界坐标->相机坐标）
	glm::mat4 ViewMat(1.0f);
	ViewMat = glm::translate(ViewMat, glm::vec3(0, 0, -3.0f));

	//投影矩阵（相机坐标->投影坐标）
	glm::mat4 ProjMat(1.0f);
	ProjMat = glm::perspective(glm::radians(45.0f), (float)960 / (float)544, 0.1f, 100.0f);

	//把变量Uniform到Shader
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ModelMat"), 1, GL_TRUE, glm::value_ptr(ModelMat));
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ViewMat"), 1, GL_TRUE, glm::value_ptr(ViewMat));
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ProjMat"), 1, GL_TRUE, glm::value_ptr(ProjMat));

	GlobalLog += "Matrix generate success\n";

	GlobalLog += "\nPrepare to Draw.\n";

	while (true)
	{
		//清除缓冲区
		glClearColor(0.5, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//绑定材质
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);

		//绑定顶点到Location
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//绘制10个盒子
		for (int i = 0; i < 10; i++)
		{
			float Angle = fmod(20.0f * i + 0.01 * cnt, 360.0f);

			ModelMat = glm::mat4(1.0f);
			ModelMat = glm::translate(ModelMat, cubePositions[i]);
			ModelMat = glm::rotate(ModelMat, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ModelMat"), 1, GL_TRUE, glm::value_ptr(ModelMat));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			cnt++;
		}

		vglSwapBuffers(GL_FALSE);
	}

	vglEnd();
	return 0;
}