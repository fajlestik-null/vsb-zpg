#version 330
#define MAX_LIGHTS 8

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;
in mat3 TBN;

out vec4 fragColor;

uniform vec3 cameraPosition;
uniform sampler2D textureUnitID;
uniform sampler2D normalMapUnitID;
uniform vec3 sourceObjectColor;
uniform int hasTexture;
uniform int hasNormalMap;

struct lightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int type; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
    vec3 direction;
    float distance;
};

uniform lightSource lights[MAX_LIGHTS];
uniform int numLights;

struct materialType {
    vec3 ambientFactor;
    vec3 diffuseFactor;
    vec3 specularFactor;
    int shininess;
};

uniform materialType material;

float attenuation ( float d , float c , float l , float q, float distance)
{
// d = distance from the light to the fragment
// c = constant attenuation factor
// l = linear attenuation factor
// q = quadratic attenuation factor
return 1.0 / (c + l * d + q * (d * d));
}

void main() {
    vec3 objectColor = sourceObjectColor;
    if(hasTexture == 1) {
        objectColor = texture(textureUnitID, uv).rgb;
    }

    //NORMAL MAPPING
    vec3 N; // Normal Vector
    if(hasNormalMap == 1) {
        vec3 normalFromMap = texture(normalMapUnitID, uv).rgb;
        normalFromMap = normalFromMap * 2.0 - 1.0;   
        N = normalize(TBN * normalFromMap); 
    } else {
        N = normalize(worldNormal);
    }

    vec3 V = normalize(cameraPosition - worldPosition); // View Vector

    vec3 totalLighting = vec3(0.0);

    for(int i = 0; i < numLights; i++) {
        
        if(lights[i].type == 0) {
            //AMBIENT
            totalLighting += objectColor * lights[i].intensity * material.ambientFactor;
            continue;
        }

        vec3 L; // Light Direction
        float att = 1.0;
        float spot = 1.0;

        if(lights[i].type == 1) { 
            //DIRECTIONAL
            L = normalize(lights[i].position); 
        } 
        else {  
            // POINT & SPOT
            vec3 diff = lights[i].position - worldPosition;
            float dist = length(diff);
            L = normalize(diff);
            
            if (dist > lights[i].distance) continue;


            float range = lights[i].distance;
            if(range <= 0.0)
            {
                range = 1.0;
            }
            float linear = 4.5 / range;
            float quadratic = 75.0 / (range * range);

            att = attenuation(dist, 1.0, linear, quadratic, range);
            
            if(lights[i].type == 3) {
                 spot = dot(normalize(lights[i].direction), -L);
            }
        }

        //SPOT
        if(att > 0.0) {
            if(lights[i].type == 3) {
                if (spot < 0.93) spot = 0.0;
                else spot = (spot - 0.93) / (1.0 - 0.93);
            }
        }

        if(spot > 0.0) {
            
            //DIFFUSE
            float diff = max(dot(N, L), 0.0);
            
            //SPECULAR
            vec3 H = normalize(L + V);
            float NdotH = max(dot(N, H), 0.0);
            float spec = pow(NdotH, material.shininess);



            vec3 diffuseColor = diff * material.diffuseFactor * objectColor;
            vec3 specularColor = spec * material.specularFactor * lights[i].color;

            totalLighting += (diffuseColor + specularColor) * lights[i].intensity * att * spot;
        }
    }

    fragColor = vec4(totalLighting, 1.0);
}