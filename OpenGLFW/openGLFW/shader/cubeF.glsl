#version 330 core


// ----- 输入输出的变量

out vec4 FragColor;

in vec2 ourUV;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D diffuse;

uniform samplerCube skybox;
uniform vec3 viewPos;


void main()
{
	// -----------灰度测试---------------
	// vec4 color = texture(diffuse,ourUV);
	// float avg = (0.2126*color.r + 0.7152*color.g+ 0.0722*color.b)/3.0;

	// FragColor = vec4(avg,avg,avg,color.a);

	// FragColor = texture(diffuse,ourUV);



	// --------------------------环境反射测试-------------------------------
	// vec3 I 		= normalize(FragPos - viewPos);
	// vec3 R 		= reflect(I,normalize(Normal));
	// FragColor 	= vec4(texture(skybox,R).rgb,1.0);



	// ----------------------------环境折射测试-----------------
	float ratio = 1.00 / 1.52;
	vec3 I 		= normalize(FragPos - viewPos);
	vec3 R 		= refract(I,normalize(Normal),ratio);
	FragColor 	= vec4(texture(skybox,R).rgb,1.0);
};