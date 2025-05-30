#version 330 core
out vec4 FragColor;

in vec3 Normal; // ������
in vec3 FragPos;  // ÿ��Ƭ�ε�����λ��
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos; // �۲��λ��
uniform Light light;
uniform Material material;

void main()
{
    // ���û�������
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    // �����Դ��Ƭ��λ��֮��ķ�������,��������������ķ�ʽ����������
    vec3 norm = normalize(Normal);
    // ��ķ��������ǹ�Դλ��������Ƭ��λ������֮���������
    vec3 lightDir = normalize(light.position - FragPos);
    // ��norm��lightDir�������е�ˣ������Դ�Ե�ǰƬ��ʵ�ʵ�������Ӱ�졣���ֵ�ٳ��Թ����ɫ
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));  


    // ����ǿ��
    // �������߷����������Ͷ�Ӧ�����ŷ�����ķ�������
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴���Ǵ�Ƭ��ָ���Դ������ǰ���Ǽ���lightDir����ʱ��������˳�������
    vec3 reflectDir = reflect(-lightDir, norm);
    // ���㾵�����
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * (vec3(1.0) - vec3(texture(material.specular, TexCoords))); // �ڰ׷�ת

    // ��ɫ���
     vec3 result = ambient + diffuse + specular;


    FragColor = vec4(result, 1.0);
}