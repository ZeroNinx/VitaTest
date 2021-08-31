#pragma once
#include <string>

using namespace std;

//文件路径
const string ContentPath = "ux0:app/ZERO00000/sce_sys/livearea/contents/";

//路径转换
inline string GetContentPath(const char* InPath)
{
    return move(ContentPath + InPath);
}
