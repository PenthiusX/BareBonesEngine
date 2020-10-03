//#version 330 core
//layout (triangles) in;
//layout (triangle_strip, max_vertices = 3) out;

//in VS_OUT {
//    vec2 gTexCoords;
//    vec3 gNormals;
//}gs_in[];

//out vec2 TexCoords;

//uniform float time;

//vec4 explode(vec4 position, vec3 normal)
//{
//    float magnitude = 10.0;
//    vec3 direction = normal * ((sin(time * 0.0005) + 1.0) * 0.5) * magnitude;
//    return position + vec4(direction, 0.0);
//}

//vec3 GetNormal()
//{
//    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//    return normalize(cross(a, b));
//}

//void main(){

//    vec3 normal = GetNormal();

//    gl_Position = explode(gl_in[0].gl_Position, normal);
//    TexCoords = gs_in[0].gTexCoords;
//    EmitVertex();
//    gl_Position = explode(gl_in[1].gl_Position, normal);
//    TexCoords = gs_in[1].gTexCoords;
//    EmitVertex();
//    gl_Position = explode(gl_in[2].gl_Position, normal);
//    TexCoords = gs_in[2].gTexCoords;
//    EmitVertex();
//    EndPrimitive();
//}


//The triangles parameter should be the same as the draw parameter passes in the draw function.

//points: when drawing GL_POINTS primitives (1).
//lines: when drawing GL_LINES or GL_LINE_STRIP (2).
//lines_adjacency: GL_LINES_ADJACENCY or GL_LINE_STRIP_ADJACENCY (4).
//triangles: GL_TRIANGLES, GL_TRIANGLE_STRIP or GL_TRIANGLE_FAN (3). X !!!we used this in draw()!!!
//triangles_adjacency : GL_TRIANGLES_ADJACENCY or GL_TRIANGLE_STRIP_ADJACENCY (6).

//#version 330 core
//layout (points) in;
//layout (line_strip, max_vertices = 2) out;

//void main() {
//    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
//    EmitVertex();

//    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
//    EmitVertex();

//    EndPrimitive();
//}

#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec2 gTexCoords;
    vec3 gNormals;
}gs_in[];

const float MAGNITUDE = 1.0;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void GenerateLine(int index)
{
    gl_Position =  gl_in[index].gl_Position;
    EmitVertex();
    gl_Position =  gl_in[index].gl_Position + (vec4(gs_in[index].gNormals, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
   // GenerateLine(1); // second vertex normal
    //GenerateLine(2); // third vertex normal
}
