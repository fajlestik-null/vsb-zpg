#version 330

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vu;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;

uniform float w = 1.0f;

void main () 
{
    vec4 vpProject = vec4(vp*w,w);
    vec4 wp = modelMatrix * vpProject;

    mat3 localNormal = transpose(inverse(mat3(modelMatrix)));
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vpProject;
    worldPosition = wp.xyz / wp.w;
    worldNormal = localNormal * vn;
    uv = 10*vu;
};