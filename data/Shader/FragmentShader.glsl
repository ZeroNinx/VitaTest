#version 100

precision highp float;

varying highp vec2 TexCoord;

uniform sampler2D BoxTexture;
uniform sampler2D FaceTexture;


vec4 mix(vec4 ColorA, vec4 ColorB, float MixValue)
{
    return vec4( (ColorA.rgb * MixValue) + (ColorB.rgb * (1.0 - MixValue)), 1.0);
}

void main()
{
    vec4 FaceColor = texture2D(FaceTexture, TexCoord);
    vec4 BoxColor = texture2D(BoxTexture, TexCoord);
    gl_FragColor = mix(FaceColor, BoxColor, sign(FaceColor.a));
}