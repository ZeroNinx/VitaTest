#include "Texture2D.h"

#include "stb_image.h"
#include "util.h"

using namespace std;

Texture2D::Texture2D(string &&Filename)
{
    //读取文件
    unsigned char *Data = stbi_load(Filename.c_str(), &Width, &Height, &Channels, 0);
    if (!Data)
    {
        See(Filename + "load Failed");
        return;
    }

    //指定格式
    GLint Format = (Channels == 3 ? GL_RGB : GL_RGBA);

    //创建纹理
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
    glGenerateMipmap(ID);
    stbi_image_free(Data);

    bIsValid = true;
}

unsigned int Texture2D::GetID()
{
    return ID;
}

bool Texture2D::IsValid()
{
    return bIsValid;
}
