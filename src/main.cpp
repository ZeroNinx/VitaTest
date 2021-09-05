#define __psp2__

#include <psp2/kernel/processmgr.h>
#include <PVR_PSP2/GLES2/gl2.h>
#include <PVR_PSP2/EGL/eglplatform.h>
#include <PVR_PSP2/EGL/egl.h>

extern "C"
{
	#include <PVR_PSP2/gpu_es4/psp2_pvr_hint.h>
}

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

#include "util.h"

using namespace std;

//SCE
int _newlib_heap_size_user   = 100 * 1024 * 1024;
unsigned int sceLibcHeapSize = 50 * 1024 * 1024;

//EGL
EGLDisplay Display;
EGLConfig Config;
EGLSurface Surface;
EGLContext Context;
EGLint NumConfigs, MajorVersion, MinorVersion;
EGLint ConfigAttr[] =
{
	EGL_BUFFER_SIZE, EGL_DONT_CARE,
	EGL_DEPTH_SIZE, 16,
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	EGL_NONE
};

Shader* DrawShader;
Camera* PlayerCamera;
unsigned int VBO, VAO, EBO;
unsigned Texture, Texture2;
int cnt = 0;

//盒子的36个顶点
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

//SCEInit
void SCEInit()
{
	sceKernelLoadStartModule("vs0:sys/external/libfios2.suprx", 0, NULL, 0, NULL, NULL);
  	sceKernelLoadStartModule("vs0:sys/external/libc.suprx", 0, NULL, 0, NULL, NULL);

  	sceKernelLoadStartModule("app0:libgpu_es4_ext.suprx", 0, NULL, 0, NULL, NULL);
  	sceKernelLoadStartModule("app0:libIMGEGL.suprx", 0, NULL, 0, NULL, NULL);
	sceKernelLoadStartModule("app0:libGLESv2.suprx", 0, NULL, 0, NULL, NULL);
}

//初始化PVR_PSP2
void PVR_PSP2Init()
{
	PVRSRV_PSP2_APPHINT hint;
  	PVRSRVInitializeAppHint(&hint);
  	PVRSRVCreateVirtualAppHint(&hint);
}

//EGL初始化
void EGLInit()
{
	EGLBoolean Res;
	Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	Res = eglInitialize(Display, &MajorVersion, &MinorVersion);
	if (Res == EGL_FALSE)
	{
		sceClibPrintf("EGL initialize failed.\n");
		return;
	}

	eglChooseConfig(Display, ConfigAttr, &Config, 1, &NumConfigs);
	Surface = eglCreateWindowSurface(Display, Config, (EGLNativeWindowType)0, NULL);
	Context = eglCreateContext(Display, Config, EGL_NO_CONTEXT, NULL);
	eglMakeCurrent(Display, Surface, Surface, Context);
	eglSwapInterval(Display, (EGLint)1);

}

//EGL终止
void EGLEnd()
{
	eglDestroySurface(Display, Surface);
  	eglDestroyContext(Display, Context);
  	eglTerminate(Display);
}

//自定义初始化
void CustomInit()
{
	//防止载入图形时上下颠倒
	stbi_set_flip_vertically_on_load(true);
	
	//开启深度缓冲区
	glEnable(GL_DEPTH_TEST);
}

int main()
{
	SCEInit();
	PVR_PSP2Init();
	EGLInit();
	CustomInit();

	//初始化着色器
	DrawShader = new Shader(GetContentPath("Shader/VertexShader.glsl"), GetContentPath("Shader/FragmentShader.glsl"));
	if (!DrawShader->bSuccessfulInit)
	{
		sceClibPrintf("Shader Compile Failed\n");
		EGLEnd();
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

	//读取纹理
	int TextureWidth, TextureHeight, TextureChannel;
	unsigned char *Data = stbi_load(GetContentPath("box.jpg").c_str(), &TextureWidth, &TextureHeight, &TextureChannel, 0);
	if (!Data)
	{
		sceClibPrintf("Box texture read Failed\n");
		EGLEnd();
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

	//读取纹理2
	Data = stbi_load(GetContentPath("face.png").c_str(), &TextureWidth, &TextureHeight, &TextureChannel, 0);
	if (!Data)
	{
		sceClibPrintf("Face texture read Failed\n");
		EGLEnd();
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

	//模型矩阵（内部坐标->世界坐标）
	glm::mat4 ModelMat(1.0f);
	ModelMat = glm::rotate(ModelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0));

	//相机的反向矩阵（世界坐标->相机坐标）
	glm::mat4 ViewMat(1.0f);
	ViewMat = ViewMat = glm::translate(ViewMat, glm::vec3(0, 0, -3.0f));

	//投影矩阵（相机坐标->投影坐标）
	glm::mat4 ProjMat(1.0f);
	ProjMat = glm::perspective(glm::radians(45.0f), (float)960 / (float)544, 0.1f, 100.0f);

	//把变量Uniform到Shader
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMat));
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ViewMat"), 1, GL_FALSE, glm::value_ptr(ViewMat));
	glUniformMatrix4fv(glGetUniformLocation(DrawShader->GetID(), "ProjMat"), 1, GL_FALSE, glm::value_ptr(ProjMat));

	//创建相机
	//PlayerCamera = new Camera(glm::vec3(0, 0, 3.0f), glm::radians(5.0f),glm::radians(0.0f),0, glm::vec3(0, 1.0f, 0));

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
	}

	EGLEnd();
	return 0;
}