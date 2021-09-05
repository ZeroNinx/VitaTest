#pragma once
#include <string>

using namespace std;

//文件路径
const string ContentPath = "app0:data/";

//路径转换
inline string GetContentPath(const char* InPath)
{
    return move(ContentPath + InPath);
}
