#pragma once
extern "C"
{
    #include <psp2/kernel/clib.h>
}

#include <string>

//Attribute对应的Location
#define AttributeLocation_Position  0
#define AttributeLocation_TexCoord  1
#define AttributeLocation_Normal    2

using namespace std;

//文件路径
const string ContentPath = "app0:data/";

//路径转换
inline string GetContentPath(const char* InPath)
{
    return move(ContentPath + InPath);
}

//快速输出日志
inline void See(const string &InStr)
{
    sceClibPrintf("[-----ZERO00000-----]");
    sceClibPrintf(InStr.c_str());
    sceClibPrintf("\n");
}

inline void See(const char *InStr)
{
    sceClibPrintf("[-----ZERO00000-----]");
    sceClibPrintf(InStr);
    sceClibPrintf("\n");
}