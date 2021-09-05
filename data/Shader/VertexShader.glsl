layout (location = 6) in vec3 aPos; 
layout (location = 7) in vec2 aTexCoord;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;

out vec2 TexCoord;

void main()
{
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}