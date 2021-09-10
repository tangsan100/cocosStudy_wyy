#version 330 core

// ------- 定义材质和光源的结构
// 材质
struct tMaterial{
	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数

	float shiness;	// 镜面发射衰减次幂
};

// 光源
struct tLight{
	vec3 pos;

	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数
};



// ----- 输入输出的变量

out vec4 FragColor;
in vec4 ourColor;
in vec2 ourUV;
in vec3 Normal;
in vec3 FragPos;


// 用户传入的参数
uniform tMaterial Material;
uniform tLight  Light;
uniform sampler2D ourTexure;
uniform vec3 viewPos;

void main()
{
	// ---------------环境光------------
	vec3  ambient = Light.ambient * Material.ambient;

	// ---------------漫反射 ------
	vec3 normal = normalize(Normal);
	// 阳光照射角度向量
	vec3 lightDir = normalize(Light.pos - FragPos);

	// 阳光在法线方向上的分量
	float diff =  max(dot(lightDir,normal),0.0);
	vec3 diffuse = Light.diffuse*diff*Material.diffuse;


	// --------------- 镜面反射------
	// 反射强度
	float specularStrength = 0.5f;
	// 观察方向
	vec3 viewDir = normalize(viewPos - FragPos);
	// 反射光向量
	vec3 refectDir = normalize(reflect(-lightDir,normal));
	// 反射光在观察方向的投影，求32次方
	float spec = pow(max(dot(viewDir,refectDir),0),Material.shiness);
	// 最终的反射强度结果
	vec3 specular = Light.specular*spec*Material.specular; 

	// ------最终结果
	// 光照强弱= 环境光+漫反射光 + 镜面反射光
	vec3 result = ambient + diffuse + specular;
	FragColor = texture(ourTexure,ourUV) * vec4(result, 1.0f);
};