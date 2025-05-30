#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor; 
uniform vec3 lightPos;  // 光源位置
uniform vec3 viewPos; // 观察的位置

in vec3 Normal; // 法向量
in vec3 FragPos;  // 每个片段的世界位置

void main()
{
    // 设置环境光照
    // 环境因子
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    // 计算光源和片段位置之间的方向向量,让两个向量相减的方式计算向量差
    vec3 norm = normalize(Normal);
    // 光的方向向量是光源位置向量与片段位置向量之间的向量差
    vec3 lightDir = normalize(lightPos - FragPos);
    // 对norm和lightDir向量进行点乘，计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面强度
    float specularStrength = 0.5;

    // 计算视线方向向量，和对应的沿着法线轴的反射向量
    vec3 viewDir = normalize(viewPos - FragPos);
    // reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）
    vec3 reflectDir = reflect(-lightDir, norm);
    // 计算镜面分量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // 颜色结果
    vec3 result = (ambient + diffuse + specular) * objectColor;


    FragColor = vec4(result, 1.0);
}