#version 410 core

in vec3 normal;
in vec4 fragPosEye;
in vec4 fragPosLightSpace;
in vec4 fragPosLightSpace2;
in vec2 fragTexCoords;

out vec4 fColor;

//lighting
uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;
uniform	vec3 lightColor;
uniform	vec3 lightDir;

uniform mat3 lightDirMatrix2;
uniform	vec3 lightColor2;
uniform	vec3 lightDir2;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;
uniform sampler2D shadowMap2;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 64.0f;

vec3 o;

void computeLightComponents()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}

void computeLightComponents2()
{		
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);	
	
	//compute light direction
	vec3 lightDirN2 = normalize(lightDirMatrix2 * lightDir2);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector2 = normalize(lightDirN2 + viewDirN);
		
	//compute ambient light
	ambient += ambientStrength * lightColor2;
	
	//compute diffuse light
	diffuse += max(dot(normalEye, lightDirN2), 0.0f) * lightColor2;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector2, normalEye), 0.0f), shininess);
	specular += specularStrength * specCoeff * lightColor2;
}

float computeShadow()
{	
	// perform perspective divide
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    vec3 normalizedCoords2 = fragPosLightSpace2.xyz / fragPosLightSpace2.w;
	bool useShadow = true;
	bool useShadow2 = true;
    if(normalizedCoords.z > 1.0f)
        useShadow = false;
	if(normalizedCoords2.z > 1.0f)
        useShadow2 = false;
    // Transform to [0,1] range
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;
    normalizedCoords2 = normalizedCoords2 * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;    
    float closestDepth2 = texture(shadowMap2, normalizedCoords2.xy).r;    
    // Get depth of current fragment from light's perspective
    float currentDepth = normalizedCoords.z;
    float currentDepth2 = normalizedCoords2.z;
    // Check whether current frag pos is in shadow
    float bias = 0.005f;
    float shadow = currentDepth - bias> closestDepth  ? 1.0f : 0.0f;
    float shadow2 = currentDepth2 - bias> closestDepth2 ? 1.0f : 0.0f;

    return (shadow + shadow2) * 0.5;	
}

void main() 
{
	computeLightComponents();
	
	float shadow = computeShadow();
	
	//modulate with diffuse map
	ambient *= vec3(texture(diffuseTexture, fragTexCoords));
	diffuse *= vec3(texture(diffuseTexture, fragTexCoords));
	//modulate woth specular map
	specular *= vec3(texture(specularTexture, fragTexCoords));
	
	//modulate with shadow
	vec3 color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);
    
    fColor = vec4(color, 1.0f);
    //fColor = vec4(o, 1.0f);
}
