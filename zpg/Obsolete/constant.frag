#version 330

in vec3 fragmentColor;

out vec3 fragColor;

uniform vec3 sourceObjectColor;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main () {
        fragColor = sourceObjectColor;
}

