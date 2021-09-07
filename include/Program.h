#pragma once
extern "C"
{
    #include <psp2/ctrl.h>
}

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Texture2D.h"

#include "util.h"

class Program
{
public:

    Program(){};

    //初始化
    virtual bool Init() = 0;

    //输入处理
    virtual void ProcessInput() = 0;

    //进行绘画
    virtual void Draw() = 0;

    //关闭标志
    bool ShouldEnd = false;

};