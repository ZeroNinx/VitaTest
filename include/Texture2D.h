#pragma once
#include <string>

class Texture2D
{
public:

    //构造函数
    Texture2D(std::string&& Filename);

    //返回是否有效
    bool IsValid();

    //获取ID
    unsigned int GetID();

private:
    
    //ID
    unsigned int ID;

    //宽
    int Width;
    
    //高
    int Height;

    //通道
    int Channels;

    //是否有效
    bool bIsValid=false;
};
