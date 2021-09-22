#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// in VS_OUT {
//     vec3 color;
// } gs_in[];


// out vec3 fColor;
in vec3 normal[];
uniform mat4 pMatrix;

// void build_house(vec4 position)
// {    
//     fColor = gs_in[0].color; // since there's only one input vertex
//     gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   
//     EmitVertex();   
//     gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
//     EmitVertex();
//     gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
//     EmitVertex();
//     gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
//     EmitVertex();
//     gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top
//     fColor = vec3(1.0, 1.0, 1.0);
//     EmitVertex();

//     EndPrimitive();
// }

void process_line(int index)
{
    vec4 pos = gl_in[index].gl_Position;
    float strip = 0.3;

     gl_Position = pMatrix *pos;
    EmitVertex();
    gl_Position = pMatrix * (pos + vec4(normal[index], 0.0) * strip);
    EmitVertex();
    EndPrimitive();
}

void main() {    
    process_line(0);
    process_line(1);
    process_line(2);
}