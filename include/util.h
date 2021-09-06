#pragma once
#include <psp2/kernel/clib.h>
#include <string>

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