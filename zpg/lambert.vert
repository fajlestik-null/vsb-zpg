#version 330

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main () 
{
    mat3 localNormal = transpose(inverse(mat3(modelMatrix)));
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0f);
    worldPosition = modelMatrix * vec4(vp, 1.0f);
    worldNormal = localNormal * vn;
};