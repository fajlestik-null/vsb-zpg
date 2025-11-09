#version 330

in vec4 worldPosition;
in vec3 worldNormal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 sourceObjectColor;
         
out vec4 fragColor;
         
void main () 
{
    vec3 lightDir = lightPosition - worldPosition.xyz;
    float difference = max(dot(normalize(lightDir), normalize(worldNormal)), 0.0);
    vec4 objectColor = vec4( sourceObjectColor, 1.0);
    vec3 decreasedObjectColor = objectColor.xyz * 0.1;
    vec4 ambient = vec4(decreasedObjectColor, 1.0);
    fragColor = ambient + (difference * objectColor); 
}