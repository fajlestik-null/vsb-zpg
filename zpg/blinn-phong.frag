#version 330

in vec4 worldPosition;
in vec3 worldNormal;
		 
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 cameraPosition;

out vec4 fragColor;

void main () 
{
    //diffuse
    vec3 reflectDir = normalize(lightPosition - worldPosition.xyz);
    float difference = max(dot(reflectDir, normalize(worldNormal)), 0.0);
   /* vec4 objectColor = vec4(worldNormal.x, worldNormal.y, worldNormal.z, 1.0);
    vec4 diffuse = difference * objectColor;*/
    vec4 objectColor = vec4(objectColor, 1.0);
    vec4 diffuse = difference * objectColor;

    //ambient
    vec3 decreasedObjectColor = objectColor.xyz * 0.1;
    vec4 ambient = vec4( decreasedObjectColor, 1.0);

    //specular
    vec3 cameraDir = normalize(cameraPosition - worldPosition.xyz);
    vec3 halfWayDir = normalize(reflectDir + cameraDir);    //replacing reflectDir with halfWayDir
	float dotSpecular = pow(max(dot(halfWayDir, normalize(worldNormal)), 0.0), 32);
    vec4 specular = dotSpecular * vec4(1.0); //specular vector * reflect color

    //combination
    fragColor = ambient + diffuse + specular;
}