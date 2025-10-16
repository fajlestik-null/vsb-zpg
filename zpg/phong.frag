#version 330

in vec4 worldPosition;
in vec3 worldNormal;
		 
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cameraPosition;

out vec4 fragColor;

void main () 
{
    //diffuse
    vec3 lightToVector = normalize(lightPosition - worldPosition.xyz);
    float dotProduct = max(dot(lightToVector, normalize(worldNormal)), 0.0);
    vec4 diffuse = dotProduct * vec4(lightColor, 1.0);

    //ambient
    vec4 ambient = vec4( 0.1, 0.1, 0.1, 1.0);

    //specular
    vec3 cameraToVector = cameraPosition - worldPosition.xyz;
	vec3 reflectVector = reflect(-lightToVector, worldNormal);
	float dotSpecular = pow(max(dot(normalize(reflectVector), normalize(cameraToVector)), 0.0), 32);
    vec4 specular = dotSpecular * vec4(1.0,1.0, 1.0, 1.0);

    //combination
    fragColor = ambient + diffuse + specular;
}