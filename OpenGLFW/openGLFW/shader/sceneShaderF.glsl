#version 330 core

// ------- 定义材质和光源的结构
// 材质
struct tMaterial{
	sampler2D diffuse; // 材质贴图
	sampler2D specular; // 光照贴图

	float shiness;	// 镜面发射衰减次幂
};

// 平行光
struct DircetLight{
	vec3 dir;

	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数
};

// 点光源
struct PointLight{
	vec3 pos;

	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数

	float c;
	float l;
	float q;
};

// 聚光灯
struct SpotLight{
	vec3 pos;
	vec3 dir;

	vec3 ambient; // 环境光
	vec3 diffuse;	// 漫反射系数
	vec3 specular;	// 镜面反射系数

	float cutOff; // 聚光灯光夹角 的cos 值
	float outCutOff; // 边缘

	float c;
	float l;
	float q;
};


// ----- 输入输出的变量

out vec4 FragColor;
in vec4 ourColor; 
in vec2 ourUV;
in vec3 Normal; // 法线
in vec3 FragPos; // 纹理坐标


#define MAX_LIGHT_COUT 4

// 用户传入的参数
uniform tMaterial 		Material;
uniform DircetLight  	dLight;
uniform PointLight 		pLight[MAX_LIGHT_COUT];
uniform SpotLight		sLight;

// 观察者的位置
uniform vec3 viewPos;



// 平行光的计算
vec3 calculateDirLight(DircetLight Light){
	vec3 lightDir = Light.dir;
	vec3 normal = normalize(Normal);

	// ---------------环境光------------
		vec3  ambient = vec3(texture(Material.diffuse,ourUV))*Light.ambient;


		// 阳光在法线方向上的分量
		float diff =  max(dot(lightDir,normal),0.0);
		vec3 diffuse = Light.diffuse*diff*vec3(texture(Material.diffuse,ourUV));


		// --------------- 镜面反射------
		// 观察方向
		vec3 viewDir = normalize(viewPos - FragPos);
		// 反射光向量
		vec3 refectDir = normalize(reflect(-lightDir,normal));
		// 反射光在观察方向的投影，求32次方
		float spec = pow(max(dot(viewDir,refectDir),0.0),Material.shiness);
		// 最终的反射强度结果
		vec3 specular = Light.specular*spec*vec3(texture(Material.specular,ourUV)); 

	vec3 result =  (ambient + diffuse + specular);
	result.x = max(result.x,0.0);
	result.y = max(result.y,0.0);
	result.z = max(result.z,0.0);

	return result;
}

// 点光源的计算
vec3 calculatePointLight(PointLight Light){

	// 衰减计算
	float dist = length(Light.pos - FragPos);
	float attenuation = 1.0f/(Light.c + Light.l*dist + Light.q*dist*dist);

	DircetLight dirLight;
	dirLight.dir 		= (Light.pos - FragPos);
	dirLight.ambient 	= Light.ambient;
	dirLight.diffuse 	= Light.diffuse;
	dirLight.specular	= Light.specular;

	vec3 result = calculateDirLight(dirLight);
	return result * attenuation;
	// return (ambient + diffuse + specular) * attenuation;
}

// 聚光灯的计算
vec3 calculateSpotLight(SpotLight Light){

	// ----距离衰减系数
	float dist = length(Light.pos - FragPos); // 指向光源
	float attenuation = 1.0f/(Light.c + Light.l*dist+ Light.q*dist*dist); // 点光源 的衰减

// -------------- 边缘衰减系数-------
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(Light.pos - FragPos);
	float theta = dot(lightDir,normalize(-Light.dir));

	float epsilon = Light.cutOff - Light.outCutOff;
	float instensity = clamp((theta - Light.outCutOff)/epsilon,0.0,1.0);



	// ---------------环境光------------
		vec3  ambient = Light.ambient * vec3(texture(Material.diffuse,ourUV));


		// 阳光在法线方向上的分量
		float diff =  max(dot(lightDir,normal),0.0);
		vec3 diffuse = Light.diffuse*diff*vec3(texture(Material.diffuse,ourUV));


		// --------------- 镜面反射------
		// 观察方向
		vec3 viewDir = normalize(viewPos - FragPos);
		// 反射光向量
		vec3 refectDir = normalize(reflect(-lightDir,normal));
		// 反射光在观察方向的投影，求32次方
		float spec = pow(max(dot(viewDir,refectDir),0.0),Material.shiness);
		// 最终的反射强度结果
		vec3 specular = Light.specular*spec*vec3(texture(Material.specular,ourUV)); 


		// 边缘衰减系数
		diffuse *= instensity;
		specular *= instensity;

		// ------最终结果
		// 光照强弱= 环境光+漫反射光 + 镜面反射光
		vec3 result = (ambient + diffuse + specular) * attenuation;
		return result;

}

void main()
{
	vec3 result = vec3(0.0f);
	result += calculateDirLight(dLight);

	
	for (int i = 0; i < MAX_LIGHT_COUT; ++i){
		result += calculatePointLight(pLight[i]);
	}

	// vec3 result = vec3(0.0f);
	// result += calculatePointLight(pLight[0]);
	// result += calculatePointLight(pLight[1]);
	// result += calculatePointLight(pLight[2]);
	// result += calculatePointLight(pLight[3]);
	result += calculateSpotLight(sLight);
	
	FragColor = vec4(result, 1.0f);
};