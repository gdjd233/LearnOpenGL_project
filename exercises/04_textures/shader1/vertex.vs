#version 330 core
layout (location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor; // ��ɫ����������λ��ֵΪ 1
layout (location = 2) in vec2 aTexCoord; // �������������λ��ֵΪ 2

out vec2 TexCoord; // ��Ƭ����ɫ�����һ������

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}