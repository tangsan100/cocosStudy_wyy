#version 330 core


// ----- 输入输出的变量

out vec4 FragColor;

in vec2 ourUV;

uniform sampler2D diffuse;

float kernelSharp[9] = float[](
	-1.0,-1.0,-1.0,
	-1.0, 9.0,-1.0,
	-1.0,-1.0,-1.0
);

float kernel[9] = float[](
	1.0/16,2.0/16,1.0/16,
	2.0/16,4.0/16,2.0/16,
	1.0/16,2.0/16,1.0/16
);


vec3 kernelProcess(){
	float offsetX = 1.0/800;
	float offsetY = 1.0/600;

	vec2 offsets[9]  = vec2[](
		vec2(-offsetX,offsetY), // top-letf
		vec2(0.0,offsetY),		// top-center
		vec2(offsetX,offsetY),  // top-right
		vec2(-offsetX,0.0),		// center-left
		vec2(0.0,0.0),			// center
		vec2(offsetX,0.0),		// center-right
		vec2(-offsetX,-offsetY),// bottom-left
		vec2(0.0,-offsetY),		// bottom-center
		vec2(offsetX,-offsetY)	// bottom- right
	);

	vec3 sampleTex[9];
	for (int i=0; i < 9; ++i){
		sampleTex[i] = vec3(texture(diffuse,ourUV.st + offsets[i]));
	}

	vec3 col = vec3(0.0);
	for(int i = 0 ; i < 9; ++i){
		col += sampleTex[i]*kernel[i];
	}

	return col;

}

void main()
{
	vec3 col = kernelProcess();
	FragColor = vec4(col,1.0);
	// FragColor = texture(diffuse,ourUV);
};