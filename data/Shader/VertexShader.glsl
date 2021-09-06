#version 100

attribute vec3 aPos; 
attribute vec2 aTexCoord;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;

varying vec2 TexCoord;

void main()
{
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}