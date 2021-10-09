#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // 转换为 0-1 之间的深度值
    lightDistance = lightDistance / far_plane;
    
    // 修改深度值
    gl_FragDepth = lightDistance;
}