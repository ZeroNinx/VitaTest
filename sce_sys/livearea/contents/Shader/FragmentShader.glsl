in vec2 TexCoord;

uniform sampler2D BoxTexture;
uniform sampler2D FaceTexture;

out vec4 FragColor;

void main()
{
    FragColor = mix(texture(FaceTexture, TexCoord), texture(BoxTexture,TexCoord),0.7);
}