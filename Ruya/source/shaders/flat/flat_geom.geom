#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 normals[]; // 3 normals for triangle, one for each vertex
out vec3 surfaceNormalInLocalSpace;
out vec3 fragPositionInObjSpace;

void main() {    
    vec4 v1 = gl_in[0].gl_Position;
    vec4 v2 = gl_in[1].gl_Position;
    vec4 v3 = gl_in[2].gl_Position;

    // pass through triangle
    gl_Position = v1;
    EmitVertex();   
    gl_Position = v2;
    EmitVertex();   
    gl_Position = v3;
    EmitVertex();   
    EndPrimitive();

    // take average of vertex normals. If normals are surface normals the result will be the same.
    // otherwise vertex normals (used for smooth shading) will be converted to surface normals;
    surfaceNormalInLocalSpace = (normals[0] + normals[1] + normals[2]) / 3.0;

    // all fragments in triangle will be processed the same as the fragment at the center
    // of the triangle, this way fragment positions aren't interpolated.
    fragPositionInObjSpace = (vec3(v1) + vec3(v2) + vec3(v3))/3.0;
}  