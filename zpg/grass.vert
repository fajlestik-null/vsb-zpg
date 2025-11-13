#version 330

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vu;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;
out vec2 uv;

void main () 
{
    mat3 localNormal = transpose(inverse(mat3(modelMatrix)));
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    worldPosition = modelMatrix * vec4(vp, 1.0f);
    worldNormal = localNormal * vn;
    uv = 10*vu;
};