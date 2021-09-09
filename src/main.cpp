extern "C"
{
	#include <psp2/kernel/modulemgr.h>
	#include <psp2/kernel/processmgr.h>

#ifdef USE_PVR_PSP2
	#include <PVR_PSP2/EGL/eglplatform.h>
	#include <PVR_PSP2/EGL/egl.h>
	#include <PVR_PSP2/gpu_es4/psp2_pvr_hint.h>
#else
	#include <vitaGL.h>
#endif

}

#include "Program/LearnOpengl.h"
#include "util.h"

#include <cmath>
#include <fstream>
#include <string>

using namespace std;

//SCE
int _newlib_heap_size_user   = 16 * 1024 * 1024;
unsigned int sceLibcHeapSize = 8 * 1024 * 1024;

#ifdef USE_PVR_PSP2

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
EGLint ContextAttributeList[] = 
{
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
};

//Module初始化
void ModuleInit()
{
	sceKernelLoadStartModule("vs0:sys/external/libfios2.suprx", 0, NULL, 0, NULL, NULL);
	sceKernelLoadStartModule("vs0:sys/external/libc.suprx", 0, NULL, 0, NULL, NULL);
	sceKernelLoadStartModule("app0:module/libgpu_es4_ext.suprx", 0, NULL, 0, NULL, NULL);
  	sceKernelLoadStartModule("app0:module/libIMGEGL.suprx", 0, NULL, 0, NULL, NULL);
	See("Module init OK");
}

//初始化PVR_PSP2
void PVR_PSP2Init()
{
	PVRSRV_PSP2_APPHINT hint;
  	PVRSRVInitializeAppHint(&hint);
  	PVRSRVCreateVirtualAppHint(&hint);
	See("PVE_PSP2 init OK.");
}

//EGL初始化
void EGLInit()
{
	EGLBoolean Res;
	Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(!Display)
	{
		See("EGL display get failed.");
		return;
	}

	Res = eglInitialize(Display, &MajorVersion, &MinorVersion);
	if (Res == EGL_FALSE)
	{
		See("EGL initialize failed. ");
		return;
	}

	Res = eglChooseConfig(Display, ConfigAttr, &Config, 1, &NumConfigs);
	if (Res == EGL_FALSE)
	{
		See("EGL config initialize failed. ");
		return;
	}

	Surface = eglCreateWindowSurface(Display, Config, (EGLNativeWindowType)0, nullptr);
	if(!Surface)
	{
		See("EGL surface create failed.");
		return;
	}

	Context = eglCreateContext(Display, Config, EGL_NO_CONTEXT, ContextAttributeList);
	if(!Context)
	{
		See("EGL content create failed.");
		return;
	}

	eglMakeCurrent(Display, Surface, Surface, Context);

	See("EGL init OK.");
}

//EGL终止
void EGLEnd()
{
	eglDestroySurface(Display, Surface);
  	eglDestroyContext(Display, Context);
  	eglTerminate(Display);
	See("EGL terminated.");
}
#endif

//SCE初始化
void SCEInit()
{
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	See("SCE init OK");
}



int main()
{

#ifdef USE_PVR_PSP2
	//全局初始化
	ModuleInit();
	PVR_PSP2Init();
	EGLInit();
#else
	vglInit(0x100000);
	See("VGL init OK.");
#endif
	SCEInit();
	See("All init OK.");

	//程序创建
	Program* MainProgram = new LearnOpengl();
	if( !MainProgram->Init())
	{
#ifdef USE_PVR_PSP2
		EGLEnd();
#else
		vglEnd();
#endif
		return -1;
	}
	See("LearnOpengl init OK.");

	//主循环
	while (!MainProgram->ShouldEnd)
	{
		//输入处理
		MainProgram->ProcessInput();

		//绘画
		MainProgram->Draw();

#ifdef USE_PVR_PSP2
		//显示
		eglSwapBuffers(Display,Surface);
#else 
		vglSwapBuffers(GL_FALSE);
#endif
	}

#ifdef USE_PVR_PSP2
	EGLEnd();
#else
	vglEnd();
#endif

	sceKernelExitProcess(0);
	return 0;
}