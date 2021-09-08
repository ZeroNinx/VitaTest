#version 100

attribute vec3 aPos; //顶点位置
attribute vec2 aTexCoord; //纹理坐标
attribute vec3 aNormal; //法向量

uniform mat4 ModelMat; //模型矩阵
uniform mat4 ViewMat; //相机矩阵
uniform mat4 ProjMat; //投影矩阵
uniform mat4 TranspostInverseModelMat;//模型矩阵左上角的逆矩阵的转矩阵（用于计算法向量）

varying vec3 FragPos; //片段位置
varying vec2 TexCoord; //纹理坐标
varying vec3 Normal; //法向量

void main()
{
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(aPos, 1.0);
    FragPos = vec3(ModelMat * vec4(aPos, 1.0));
    
    TexCoord = aTexCoord;
    Normal = mat3(TranspostInverseModelMat) * aNormal;
}