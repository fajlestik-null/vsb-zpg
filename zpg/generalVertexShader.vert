#version 330

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vu;
layout(location = 3) in vec3 vt;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform int hasNormalMap;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;
out mat3 TBN; //MUST BE IMPLEMENTED IN EVERY FUNCTIONAL VERTEX SHADER (IF NOT USING -> SET TBN = mat3(1.0); ) -> ELSE LINKER FAILURE

uniform float w = 1.0f;

void main () 
{
    vec4 vpProject = vec4(vp*w,w);
    vec4 wp = modelMatrix * vpProject;
    worldPosition = wp.xyz / wp.w;


    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vpProject;
    uv = vu;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 N = normalize(normalMatrix * vn);
    worldNormal = N;

    if (hasNormalMap == 1) {

        vec3 T = normalize(normalMatrix * vt);
        
        T = normalize(T - dot(T, N) * N);
        
        vec3 B = cross(N, T);
        
        TBN = mat3(T, B, N);
    } 
    else {

        TBN = mat3(1.0); 
    }
};