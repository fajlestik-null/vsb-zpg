/*#version 330
#define MAX_LIGHTS 8
in vec3 worldPosition;
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
    vec4 ambient = vec4( objectColor * material.ambientFactor, 1.0);
    float specular = 0.0;
    vec4 diffuse = vec4(0.0);
    for(int i = 0; i < numLights; i++) {

        if(lights[i].type == 0) {
            ambient =  vec4( objectColor * lights[i].intensity * material.ambientFactor, 1.0);
            continue;
        }
        else if(lights[i].type == 1) {
            lightDirection = normalize(lights[i].position);
        }
        else if(lights[i].type == 2) {
            lightDirection = normalize(lights[i].position - worldPosition);
            att = attenuation( length(lights[i].position - worldPosition), 1.0, 0.14, 0.7, lights[i].distance ); //1.0, 0.14, 0.7
        }
        else if(lights[i].type == 3) {
            lightDirection = normalize(lights[i].position - worldPosition);
            att = attenuation( length(lights[i].position - worldPosition), 0.0, 0.9, 0.32, lights[i].distance );

            spot = dot(normalize(lights[i].direction), -lightDirection);

        }

        if(lights[i].type != 0) {
            if(dot(lightDirection, normalize(worldNormal)) <= 0.0)
                continue;
            vec3 reflectDir = reflect(-lightDirection, normalize(worldNormal));
            specular = pow(max(dot(normalize(cameraPosition), normalize(reflectDir)), 0.0), material.shininess);
            float dotProduct = max(dot(lightDirection, normalize(worldNormal)), 0.0);
            diffuse = dotProduct * vec4( objectColor, 1.0);

            if (lights[i].type == 3) {

                if (spot < 0.93) {
                specular = 0; 
                    diffuse = vec4(0.0);
                }
                spot=(spot-0.93)/(1-0.93);
            }
            
            finalColor += (diffuse * vec4(material.diffuseFactor,1.0f)  + (specular * vec4(lights[i].color, 1.0) * vec4(material.specularFactor,1.0f))) * spot * att * lights[i].intensity; // texture(textureUnitID,uv) -> in case of missing texture returns 0
        }
    
    }

    fragColor = ambient + finalColor + texture(textureUnitID,uv) * 0.01;
}*/
#version 330
#define MAX_LIGHTS 8

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

out vec4 fragColor;

uniform vec3 cameraPosition;
uniform sampler2D textureUnitID;
uniform vec3 sourceObjectColor;
uniform int hasTexture;

struct lightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int type; // 0 = ambient, 1 = directional, 2 = point, 3 = spot
    vec3 direction;
    float distance; // Note: This is no longer used for attenuation math
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

// --- STANDARD ATTENUATION (No subtraction) ---
// d = current distance, c = constant, l = linear, q = quadratic
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

void main() {
    vec3 objectColor = sourceObjectColor;
    if(hasTexture == 1) {
        objectColor = texture(textureUnitID, uv).rgb;
    }

    // 1. Calculate Geometry Vectors ONCE
    vec3 N = normalize(worldNormal);
    vec3 V = normalize(cameraPosition - worldPosition); // View Vector

    // 2. Initialize Accumulators
    vec3 totalAmbient = vec3(0.0);
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for(int i = 0; i < numLights; i++) {
        
        // --- AMBIENT ---
        if(lights[i].type == 0) {
            totalAmbient += objectColor * lights[i].intensity * material.ambientFactor;
            continue;
        }

        vec3 L; 
        float att = 1.0;
        float spot = 1.0;

        // --- LIGHT VECTORS & ATTENUATION ---
        if(lights[i].type == 1) { // Directional
            L = normalize(lights[i].position);
        }
        else { // Point or Spot
            vec3 diff = lights[i].position - worldPosition;
            float dist = length(diff);
            L = normalize(diff);
            
            // --- DYNAMIC ATTENUATION CALCULATION ---
            // We calculate the factors so the light fades to 0 at "lights[i].distance"
            float range = lights[i].distance;
            
            // Safety check to prevent division by zero
            if(range <= 0.0) range = 1.0; 

            // Standard formulas to map distance to attenuation factors:
            // Linear ~ 4.5 / range
            // Quadratic ~ 75.0 / range^2
            float linear = 4.5 / range;
            float quadratic = 75.0 / (range * range);

            // Pass these calculated values instead of hardcoded 0.9 / 0.32
            // Use 1.0 for constant to prevent infinite brightness at center
            att = attenuation(dist, 1.0, linear, quadratic, range);
            
            if(lights[i].type == 3) {
                 spot = dot(normalize(lights[i].direction), -L);
            }
        }

        if(att > 0.0) {
            
            // --- SPOTLIGHT CUTOFF ---
            if(lights[i].type == 3) {
                if (spot < 0.93) spot = 0.0;
                else spot = (spot - 0.93) / (1.0 - 0.93);
            }

            if(spot > 0.0) {
                
                // Diffuse Check (N dot L)
                float NdotL = max(dot(N, L), 0.0);
                
                if(NdotL > 0.0) {
                    
                    // Diffuse
                    totalDiffuse += (NdotL * objectColor * material.diffuseFactor) * lights[i].color * lights[i].intensity * att * spot;

                    // --- PURE PHONG SPECULAR ---
                    // 1. Calculate Reflection Vector: reflect(Incident, Normal)
                    // Note: reflect() expects the vector pointing TO the surface (-L)
                    vec3 R = reflect(-L, N);
                    
                    // 2. Specular: Dot(Reflection, View)
                    float RdotV = max(dot(R, V), 0.0);
                    
                    float spec = pow(RdotV, material.shininess);
                    
                    totalSpecular += (spec * material.specularFactor) * lights[i].color * lights[i].intensity * att * spot;
                }
            }
        }
    }

    vec3 result = totalAmbient + totalDiffuse + totalSpecular;
    fragColor = vec4(result, 1.0);
}