#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform vec3 lightPos;  // ��Դλ��
uniform vec3 viewPos; // �۲��λ��

in vec3 Normal; // ������
in vec3 FragPos;  // ÿ��Ƭ�ε�����λ��

void main()
{
    // ���û�������
    // ��������
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    // �����Դ��Ƭ��λ��֮��ķ�������,��������������ķ�ʽ����������
    vec3 norm = normalize(Normal);
    // ��ķ��������ǹ�Դλ��������Ƭ��λ������֮���������
    vec3 lightDir = normalize(lightPos - FragPos);
    // ��norm��lightDir�������е�ˣ������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ�졣���ֵ�ٳ��Թ����ɫ
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // ����ǿ��
    float specularStrength = 0.5;

    // �������߷����������Ͷ�Ӧ�����ŷ�����ķ�������
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴���Ǵ�Ƭ��ָ���Դ������ǰ���Ǽ���lightDir����ʱ��������˳�������
    vec3 reflectDir = reflect(-lightDir, norm);
    // ���㾵�����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // ��ɫ���
    vec3 result = (ambient + diffuse + specular) * objectColor;


    FragColor = vec4(result, 1.0);
}