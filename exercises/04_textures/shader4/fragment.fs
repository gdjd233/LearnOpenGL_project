#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;
in vec3 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float factor;

void main()
{
    // 按时间更改
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), abs(sin(factor * 0.6)));

    // 按上下键更改
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), factor);
}