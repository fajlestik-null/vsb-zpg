#version 330

in vec4 worldPosition;
in vec3 worldNormal;
		 
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPosition;

out vec4 fragColor;

void main () 
{
    vec3 lightToVector = normalize(lightPosition - worldPosition.xyz);
    vec3 mirrorVector = reflect(-lightToVector, worldNormal);
    float specular = pow(max(dot(normalize(mirrorVector), normalize(cameraPosition)), 0.0), 32);
    float dotProduct = max(dot(lightToVector, normalize(worldNormal)), 0.0);
    vec4 diffuse = dotProduct * vec4(lightColor, 1.0);
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);
    fragColor = ambient + diffuse + (specular * vec4(1.0));

    /*vec3 lightToVector = normalize(lightPosition - worldPosition);
    float dotProduct = max(dot(nlightToVector, normalize(worldNormal)), 0.0);
    vec4 diffuse = dotProduct * vec4(lightColor, 1.0);
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);
    vec3 cameraToVector = cameraPosition - worldPosition;
	vec3 reflectVector = reflect(-lightToVector, worldNormal);
	float dotSpecular = pow(max(dot(normalize(reflectVector), normalize(cameraPosition)), 0.0), 32);
    float specular = dotSpecular * vec4(1.0,1.0, 1.0, 1.0);
    fragColor = ambient + diffuse + specular;*/
}