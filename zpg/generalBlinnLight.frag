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
            ambient +=  vec4( objectColor * lights[i].intensity * material.ambientFactor, 1.0);
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
            vec3 reflectDir = normalize(lights[i].position - worldPosition.xyz);
            vec3 cameraDir = normalize(cameraPosition - worldPosition.xyz);
            vec3 halfWayDir = normalize(reflectDir + cameraDir);  //replacing reflectDir with halfWayDir
            specular = pow(max(dot(normalize(cameraPosition),normalize(halfWayDir)), 0.0), material.shininess);
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

// Standard Attenuation (No subtraction)
float calculateAttenuation(float dist, float c, float l, float q) {
    return 1.0 / (c + l * dist + q * dist * dist);
}

void main() {
    vec3 objectColor = sourceObjectColor;
    if(hasTexture == 1) {
        objectColor = texture(textureUnitID, uv).rgb;
    }

    // 1. Geometry Vectors
    vec3 N = normalize(worldNormal);
    vec3 V = normalize(cameraPosition - worldPosition); // View Vector

    vec3 totalLighting = vec3(0.0);

    for(int i = 0; i < numLights; i++) {
        
        // --- AMBIENT ---
        if(lights[i].type == 0) {
            // Accumulate (+=) rather than overwrite (=)
            totalLighting += objectColor * lights[i].intensity * material.ambientFactor;
            continue;
        }

        vec3 L; // Light Direction
        float attenuation = 1.0;

        // --- CALCULATE L & ATTENUATION ---
        if(lights[i].type == 1) { // Directional
            L = normalize(lights[i].position); 
        } 
        else { // Point or Spot
            vec3 difference = lights[i].position - worldPosition;
            float dist = length(difference);
            L = normalize(difference);
            
            // Standard attenuation constants (example: 1.0, 0.14, 0.07)
            attenuation = calculateAttenuation(dist, 1.0, 0.14, 0.07); 
        }

        // --- SPOTLIGHT CALCULATION ---
        float spotFactor = 1.0;
        if(lights[i].type == 3) {
            float theta = dot(normalize(lights[i].direction), -L);
            float cutOff = 0.93; // Hardcoded per your logic
            
            if(theta < cutOff) {
                spotFactor = 0.0;
            } else {
                // Soft edge smoothing
                spotFactor = (theta - cutOff) / (1.0 - cutOff);
            }
        }

        // Skip calculation if light is blocked or too far
        if(spotFactor > 0.0) {
            
            // --- DIFFUSE ---
            float diff = max(dot(N, L), 0.0);
            
            // --- BLINN-PHONG SPECULAR ---
            // 1. Calculate Halfway Vector (H) = L + V
            vec3 H = normalize(L + V);
            
            // 2. Dot Product: Normal . Halfway
            float NdotH = max(dot(N, H), 0.0);
            
            // 3. Specular term
            float spec = pow(NdotH, material.shininess);

            // Calculate final light contribution
            vec3 diffuseColor = diff * material.diffuseFactor * objectColor;
            vec3 specularColor = spec * material.specularFactor * lights[i].color; // Light color usually affects specular

            // Combine
            totalLighting += (diffuseColor + specularColor) * lights[i].intensity * attenuation * spotFactor;
        }
    }

    fragColor = vec4(totalLighting, 1.0);
}