#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // ���û�������
    // ��������
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    // �����Դ��Ƭ��λ��֮��ķ�������,��������������ķ�ʽ����������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // ��������֮��ĽǶ�Խ������������ͻ�ԽС
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // ����ǿ��
    float specularStrength = 0.5;

    // �������߷����������Ͷ�Ӧ�����ŷ�����ķ�������
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // ���㾵�����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // ��ɫ���
    vec3 result = (ambient + diffuse + specular) * objectColor;


    FragColor = vec4(result, 1.0);
}