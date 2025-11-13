#version 330
out vec4 fragColor;
in vec3 worldPosition;
uniform samplerCube textureUnitID;

void main () {
     fragColor = texture(textureUnitID, worldPosition);
}