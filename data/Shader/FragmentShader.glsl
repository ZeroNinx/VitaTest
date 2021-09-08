#version 100

precision highp float;

varying vec3 FragPos; //片段位置
varying vec2 TexCoord; //纹理坐标
varying vec3 Normal; //法向量

uniform sampler2D BoxTexture; //盒子贴图
uniform sampler2D FaceTexture; //脸贴图

uniform vec3 AmbientColor; //环境颜色
uniform float AmbientStrength; //环境光强

uniform vec3 LightPos; //光源位置
uniform vec3 LightColor; //光源颜色
uniform float LightStrength; //光照强度

uniform float SpecularStrength; //高光强度
uniform int SpecularShininessStrength; //泛光度
uniform vec3 ViewPos; //视图位置

//自定义纹理混合
vec4 mix(vec4 ColorA, vec4 ColorB, float MixValue)
{
    return vec4( (ColorA.rgb * MixValue) + (ColorB.rgb * (1.0 - MixValue)), 1.0);
}

void main()
{
    //计算纹理颜色
    vec4 FaceColor = texture2D(FaceTexture, TexCoord);
    vec4 BoxColor = texture2D(BoxTexture, TexCoord);
    vec4 TextureColor = mix(FaceColor, BoxColor, sign(FaceColor.a));

    //环境光照
    vec3 FinalAmbient = AmbientStrength * AmbientColor;
    
    //漫反射
    vec3 NormalDir = normalize(Normal);
    vec3 LightDir = normalize(FragPos - LightPos);
    float DiffStrength = max(dot(NormalDir, LightDir), 0.0);
    vec3 FinalDiffuse = DiffStrength * LightStrength * LightColor;

    //高光处理
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, NormalDir);
    float SpecularShininess = pow(max(dot(ViewDir, ReflectDir), 0.0), SpecularShininess);
    vec3 FinalSpecular = SpecularStrength * SpecularShininess * LightStrength * LightColor;
    
    gl_FragColor = TextureColor * vec4( (FinalAmbient + FinalDiffuse + FinalSpecular), 1.0);
}