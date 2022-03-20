#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 normal[]; // 3 normals for triangle, one for each vertex
out vec3 surfaceNormalInLocalSpace;
out vec3 fragPositionInObjSpace;

void main() {    
    // triangle vertices 
    vec4 v0 = gl_in[0].gl_Position;
    vec4 v1 = gl_in[1].gl_Position;
    vec4 v2 = gl_in[2].gl_Position;
    
    // take average of vertex normals. If normals are surface normals the result will be the same.
    // otherwise vertex normals (used for smooth shading) will be converted to surface normals;
    vec3 surfaceNormal = (normal[0] + normal[1] + normal[2]) / 3.0;

    // all fragments in triangle will be processed the same as the fragment at the center
    // of the triangle, this way fragment positions aren't interpolated.
    //fragPositionInObjSpace = (vec3(v0) + vec3(v1) + vec3(v2))/3.0;
    vec3 fragmentPosition = vec3(gl_in[0].gl_Position);
    
    
    // pass through triangle
    gl_Position = v0*2;
    surfaceNormalInLocalSpace  = surfaceNormal;
    fragPositionInObjSpace = fragmentPosition;
    EmitVertex();   

    gl_Position = v1*2;
    surfaceNormalInLocalSpace  = surfaceNormal;
    fragPositionInObjSpace = fragmentPosition;
    EmitVertex();   

    gl_Position = v2*2;
    surfaceNormalInLocalSpace  = surfaceNormal;
    fragPositionInObjSpace = fragmentPosition;
    EmitVertex();   
    EndPrimitive();
}  