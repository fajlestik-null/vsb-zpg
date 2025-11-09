#version 330

in vec4 worldPosition;
in vec3 worldNormal;
uniform vec3 viewPosition;
uniform vec3 sourceObjectColor;
out vec4 fragColor;

void main () {
    vec3 spotDir = vec3(1, 0, 1);
    vec3 lightPosition = vec3(-1.0,1.0,-1.0);
    vec3 camVector = normalize(vec3(5,5,0) - worldPosition.xyz);
    vec3 lightVector = normalize(lightPosition - worldPosition.xyz);
    vec3 reflectVector = normalize(reflect(-lightVector, normalize(worldNormal)));
    float diff = max(dot(normalize(lightVector), normalize(worldNormal)), 0.0);
    float spec = pow(max(dot(reflectVector, camVector), 0.0), 32);
    
    float spot = dot(normalize(spotDir), -lightVector);


    if (spot < 0.99) {
       spec = 0; diff = 0;
     }
    spot=(spot-0.99)/(1-0.99);
    fragColor = vec4(0.1, 0.1, 0.1, 1.0) + (0.01 + (diff+spec) * spot) * vec4(0.385, 0.647, 0.812, 1.0);
}