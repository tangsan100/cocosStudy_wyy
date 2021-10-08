#version 330 core

// ------- 定义材质和光源的结构
// 材质
struct tMaterial{
	sampler2D diffuse; // 材质贴图
	sampler2D specular; // 光照贴图

	float shiness;	// 镜面发射衰减次幂
};


// 光源
struct tLight{
	vec3 position;
	vec3 dir;	// 光照方向，平行光才有
	float cutOff; // 聚光灯光夹角 的cos 值
	float outCutOff; // 边缘

	// 基础光照系数
	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数

	// 衰减系数
	float c;
	float l;
	float q;
};



// ----- 输入输出的变量
out vec4 FragColor;
in vec4 ourColor;
in vec2 ourUV;
in vec3 Normal;
in vec3 FragPos;


// 用户传入的参数
uniform tMaterial material;
uniform tLight  light;
uniform vec3 viewPos;
uniform bool blinn;


void main()
{

	// 环境光
	vec3 ambient = light.ambient*vec3(texture(material.diffuse,ourUV));

	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff =  dot(norm,lightDir);
	vec3 diffuse = light.diffuse*diff*vec3(texture(material.diffuse,ourUV));

	// 镜面反射
	vec3 viewDir = viewPos - FragPos;
	vec3 refectDir = reflect(-lightDir,norm);
	float spec = 0.0;
	if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(norm, halfwayDir), 0.0), material.shiness);
    }else {
		spec = pow(max(dot(refectDir,viewDir),0),material.shiness);
	}

	
	vec3 specular = light.specular*spec*vec3(texture(material.specular,ourUV));


	float dist = length(light.position - FragPos);
	float attenuation = 1.0f/(light.c + light.l*dist+ light.q*dist*dist);

	vec3 result = (ambient + diffuse + specular)*attenuation;
	
	FragColor = vec4(result,1.0);
}