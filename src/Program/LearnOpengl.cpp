#include "Program/LearnOpengl.h"

LearnOpengl::LearnOpengl():Program()
{

}

bool LearnOpengl::Init()
{

#ifdef USE_PVR_PSP2
#else
	vglEnableRuntimeShaderCompiler(GL_TRUE);
#endif

    //开启深度缓冲区
	glEnable(GL_DEPTH_TEST);
    See("Preinit OK.");

	//创建VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //填充数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //初始化着色器
#ifdef USE_PVR_PSP2
    DrawShader = Shader(GetContentPath("Shader/VertexShader.glsl"), GetContentPath("Shader/FragmentShader.glsl"));
#else
	DrawShader = Shader(GetContentPath("Shader/VertexShader.cg"), GetContentPath("Shader/FragmentShader.cg"));
#endif
    
    if (!DrawShader.IsValid())
    {
        See("Shader init failed");
        return false;
    }
    DrawShader.Use();
    See("Shader program init OK.");

    //绑定Location对应的变量
    DrawShader.BindAttribute("aPos", AttributeLocation_Position);
    DrawShader.BindAttribute("aTexCoord", AttributeLocation_TexCoord);
    DrawShader.BindAttribute("aNormal", AttributeLocation_Normal);

    //创建纹理
    BoxTexture =new Texture2D(GetContentPath("Resource/box.jpg"));
    if (!BoxTexture->IsValid())
    {
        See("Box texture read Failed");
        return false;
    }
    See("Box texture loaded.");

    FaceTexture =new Texture2D(GetContentPath("Resource/face.png"));
    if (!FaceTexture->IsValid())
    {
        See("Face texture read Failed");
        return false;
    }
    See("Face texture loaded.");

    //导入纹理
    DrawShader.UniformInt("BoxTexture", 0);
    DrawShader.UniformInt("FaceTexture", 1);

    //创建相机
    PlayerCamera = Camera(glm::vec3(0, 0, 3.0f), 5.0f, 180.0f, 0, glm::vec3(0, 1.0f, 0));
    See("Camera created.");

    return true;
}

void LearnOpengl::ProcessInput()
{
    //输入数据结构体
    SceCtrlData CtrlData;
    const int DeadZone = 20;
    const float MoveSpeed = 0.07;

    //接收输入
    sceCtrlPeekBufferPositive(0, &CtrlData, 1);

    //See("L Sticks: " + to_string((int)CtrlData.lx) + "," + to_string((int)CtrlData.ly));
    //See("R Sticks: " + to_string((int)CtrlData.rx) + "," + to_string((int)CtrlData.ry));

    //按键通过&计算判定对否按下
    if(CtrlData.buttons & SCE_CTRL_START)
    {
        //按下Start键关闭
        ShouldEnd = true;
    }
    
    //左摇杆
    //摇杆坐标轴没有负轴，数值是从0到255
    int LX=CtrlData.lx - 255/2;
    int LY=CtrlData.ly - 255/2;
    if (abs(LX) > DeadZone || abs(LY) > DeadZone)
    {
        //前后
        if(abs(LY) > abs(LX))
        {
            if(LY > 0)
            {
                PlayerCamera.CameraMoveSpeedZ = -MoveSpeed;
            }
            else
            {
                PlayerCamera.CameraMoveSpeedZ = MoveSpeed;
            }
        }
        else//左右
        {
            if(LX > 0)
            {
                PlayerCamera.CameraMoveSpeedX = MoveSpeed;
            }
            else
            {
                PlayerCamera.CameraMoveSpeedX = -MoveSpeed;
            }
        }
    }
    else
    {
        PlayerCamera.CameraMoveSpeedZ = 0;
        PlayerCamera.CameraMoveSpeedX = 0;
    }

    //右摇杆控制视角
    int RX = CtrlData.rx - 255/2;
    int RY = CtrlData.ry - 255/2;
    if (abs(RX) > DeadZone || abs(RY) > DeadZone)
    {
        PlayerCamera.ProcessMouseMovement(RX / 3, RY / 3);
    }
}

void LearnOpengl::Draw()
{
    //输入处理
    ProcessInput();

    //绑定Attribute数据并启用
#ifdef USE_PVR_PSP2
	glVertexAttribPointer(AttributeLocation_Position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(AttributeLocation_TexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(AttributeLocation_Normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
#else
	glVertexAttribPointer(AttributeLocation_Position, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(AttributeLocation_TexCoord, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(AttributeLocation_Normal, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
#endif
    glEnableVertexAttribArray(AttributeLocation_Position);
    glEnableVertexAttribArray(AttributeLocation_TexCoord);
    glEnableVertexAttribArray(AttributeLocation_Normal);

    //模型矩阵（内部坐标->世界坐标）
    glm::mat4 ModelMat(1.0f);
    ModelMat = glm::rotate(ModelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0));

    //相机的反向矩阵（世界坐标->相机坐标）
    glm::mat4 ViewMat(1.0f);
    ViewMat = PlayerCamera.GetViewMatrix();

    //投影矩阵（相机坐标->投影坐标）
    glm::mat4 ProjMat(1.0f);
    ProjMat = glm::perspective(glm::radians(45.0f), (float)960 / (float)544, 0.1f, 100.0f);

    //环境光照
    glm::vec3 AmbientColor = glm::vec3(1.2f, 1.0f, 2.0f);
    float AmbientStrength = 0.2f;

    //光源
    glm::vec3 LightPos = glm::vec3(-5.0f, 5.0f, -5.0f);
    glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    float LightStrength = 0.5f;

    //高光
    float SpecularStrength = 0.01f;
    float SpecularShininessStrength = 128.0f;

    //把变量Uniform到Shader
    DrawShader.UniformFloatMat4("ModelMat", ModelMat);
    DrawShader.UniformFloatMat4("ViewMat", ViewMat);
    DrawShader.UniformFloatMat4("ProjMat", ProjMat);

    DrawShader.UniformFloatVec3("AmbientColor", AmbientColor);
    DrawShader.UniformFloat("AmbientStrength", AmbientStrength);

    DrawShader.UniformFloatVec3("LightColor", LightColor);
    DrawShader.UniformFloatVec3("LightPos", LightPos);
    DrawShader.UniformFloat("LightStrength", LightStrength);

    DrawShader.UniformFloat("SpecularStrength", SpecularStrength);
    DrawShader.UniformFloat("SpecularShininessStrength",SpecularShininessStrength);

    DrawShader.UniformFloatVec3("ViewPos", PlayerCamera.GetPosition());

    //清除缓冲区
    glClearColor(0.5, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绑定材质
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BoxTexture->GetID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, FaceTexture->GetID());

    PlayerCamera.UpdateCameraPosition();

    //绘制10个盒子
    for (int i = 0; i < 10; i++)
    {
        // float Angle = fmod(20.0f * i + 0.02 * cnt, 360.0f);

        ModelMat = glm::mat4(1.0f);
        ModelMat = glm::translate(ModelMat, cubePositions[i]);
        //ModelMat = glm::rotate(ModelMat, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));

        DrawShader.UniformFloatMat4("ModelMat",ModelMat);

        glm::mat4 TranspostInverseModelMat = glm::transpose(glm::inverse(ModelMat));
        DrawShader.UniformFloatMat4("TranspostInverseModelMat", TranspostInverseModelMat);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        cnt++;
    }
}