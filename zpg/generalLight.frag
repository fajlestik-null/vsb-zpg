#version 330
#define MAX_LIGHTS 8
in vec4 worldPosition;
in vec3 worldNormal;
in vec2 uv;

uniform vec3 cameraPosition;
uniform sampler2D textureUnitID;
uniform vec3 sourceObjectColor;
uniform int hasTexture;
out vec4 fragColor;

uniform vec3 lightPosition;

struct lightSource {
vec3 position;
vec3 color;
float intensity;
int type; // 0 = ambient, 1 = directional, 2 = point, 3 = reflector
vec3 direction;
float distance;

};

uniform lightSource lights [MAX_LIGHTS];

uniform int numLights;

float attenuation ( float d , float c , float l , float q, float distance)
{

// d = distance from the light to the fragment
// c = constant attenuation factor
// l = linear attenuation factor
// q = quadratic attenuation factor

if ( distance == 0.0 )
    return 0.0;

if(distance == 1.0f)
    return 1.0f;

distance = (1.0 - distance);

float att = (1.0 / ( c + l * d + q * d * d ) ) - distance;

return clamp( att , 0.0 , 1.0);
}

void main () {

    vec3 objectColor = sourceObjectColor;

    if(hasTexture == 1)
    {
            objectColor = texture(textureUnitID,uv).xyz;
    }


    vec4 finalColor = vec4(0.0);
    vec3 lightDirection = vec3(1.0);
    float att = 1.0;
    float spot = 1.0;
    vec4 ambient =  vec4( objectColor * 0.1, 1.0);
    float specular = 0.0;
    vec4 diffuse = vec4(0.0);
    for(int i = 0; i < numLights; i++) {

        if(lights[i].type == 0) {
            ambient =  vec4( objectColor * lights[i].intensity, 1.0);
            continue;
        }
        else if(lights[i].type == 1) {
            lightDirection = normalize(lights[i].position);
        }
        else if(lights[i].type == 2) {
            lightDirection = normalize(lights[i].position - worldPosition.xyz);
            att = attenuation( length(lights[i].position - worldPosition.xyz), 1.0, 0.14, 0.7, lights[i].distance ); //1.0, 0.14, 0.7
        }
        else if(lights[i].type == 3) {
            lightDirection = normalize(lights[i].position - worldPosition.xyz);
            att = attenuation( length(lights[i].position - worldPosition.xyz), 0.0, 0.9, 0.32, lights[i].distance );

            spot = dot(normalize(lights[i].direction), -lightDirection);

        }

        if(lights[i].type != 0) {
            if(dot(lightDirection, normalize(worldNormal)) <= 0.0)
                continue;
            vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
            specular = pow(max(dot(normalize(cameraPosition), normalize(reflectDir)), 0.0), 32);
            float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
            diffuse = dotProduct * vec4( objectColor, 1.0);

            if (lights[i].type == 3) {

                if (spot < 0.93) {
                specular = 0; 
                    diffuse = vec4(0.0);
                }
                spot=(spot-0.93)/(1-0.93);
            }
            
            finalColor += (diffuse  + (specular * vec4(lights[i].color, 1.0))) * spot * att * lights[i].intensity; // texture(textureUnitID,uv) -> in case of missing texture returns 0
        }
    
    }

    fragColor = ambient + finalColor + texture(textureUnitID,uv) * 0.01;
}