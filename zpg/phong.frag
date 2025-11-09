#version 330

#define MAX_LIGHTS 20

in vec4 worldPosition;
in vec3 worldNormal;
		 
uniform vec3 sourceObjectColor;
//uniform vec3 lightColor;
uniform vec3 cameraPosition;

out vec4 fragColor;

struct lightSource {
vec3 position;
vec3 color;
float intensity;
};

uniform lightSource lights [MAX_LIGHTS];

uniform int numLights;


float attenuation ( float d , float c , float l , float q , float intensity)
{

float att = 1.0 / ( c + l * d + q * d * d );

return clamp( att , 0.0 , 1.0 );
}

void main () 
{
    vec4 colorComponent = vec4(0.0f);
    for(int i = 0; i < numLights; i++){
        vec3 lightDir = normalize(lights[i].position - worldPosition.xyz);
        //handling negative oposite
        if(dot(lightDir, normalize(worldNormal)) <= 0.0)
                continue;

        //diffuse
        vec3 reflectDir = normalize(lightDir - worldPosition.xyz);
        float difference = max(dot(lightDir, normalize(worldNormal)), 0.0);
        vec4 diffuse = difference * vec4(sourceObjectColor,1.0);

        //specular
        vec3 cameraDir = normalize(cameraPosition - worldPosition.xyz);
        vec3 reflectVector = reflect(-reflectDir, worldNormal);
        float dotSpecular = pow(max(dot(normalize(reflectVector), cameraDir), 0.0), 32);

        //attenuation
        float attenuationFactor = attenuation(length(lights[i].position - worldPosition.xyz), 0.0, 0.09, 0.032, lights[i].intensity); //normally constants are 0.0, 0.09, 0.032

        //combination
        colorComponent += (diffuse + (dotSpecular * vec4(lights[i].color, 1.0))) * attenuationFactor;
    }

       if(numLights > 0)
       colorComponent /= float(numLights);

         //ambient
        vec3 decreasedObjectColor = sourceObjectColor.xyz * 0.25;
        vec4 ambient = vec4(decreasedObjectColor, 1.0);
    fragColor = ambient + colorComponent;
}