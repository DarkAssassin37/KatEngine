#version 430 core
in vec4 v_v4Position;
in vec4 v_v4Normal;
out vec4 fcolor;

in vec4 fragPosLightSpace;

uniform sampler2D colorMap1;
uniform sampler2D colorMap2;
uniform sampler2D shadowMap;


uniform mat4 view;

uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;

uniform	vec3 dirlightColor;
uniform	vec3 lightDir;

vec3 ambient;
float ambientStrength = 0.5f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 64.0f;

void computeLightComponentsDirectional()
{		

	vec4 dirFragPosEye = v_v4Position;//nu uita: facut aici transormare pt ca luminile punctiforme sunt coord scena

	vec3 cameraPosEye =  (-view * vec4(0.0f)).xyz;//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(normalMatrix * v_v4Normal.xyz);	
	
	//compute light direction
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	

	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - dirFragPosEye.xyz);
	
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
	//compute ambient light
	ambient = ambientStrength * dirlightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * dirlightColor;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = specularStrength * specCoeff * dirlightColor;
}


float computeShadow()
{	
	// perform perspective divide
    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(normalizedCoords.z > 1.0f)
        return 0.0f;
    // Transform to [0,1] range
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, normalizedCoords.xy).r;    
    // Get depth of current fragment from light's perspective
    float currentDepth = normalizedCoords.z;
    // Check whether current frag pos is in shadow
    float bias = 0.001f;
    float shadow = currentDepth - bias> closestDepth  ? 0.0f : 1.0f;

    return shadow;	
}


void main()
{
	vec3 color;
	computeLightComponentsDirectional();

	float shadow = computeShadow();

    vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    normalizedCoords = normalizedCoords * 0.5f + 0.5f;

	vec3 normPos;
	normPos.xz = (v_v4Position.xz + 1.0) * 0.5;
	normPos.y = v_v4Position.y;

	float heightBlend = pow(normPos.y, 0.5);

	vec2 uv1 = normPos.xz;
	uv1 += sin(normPos.y*2.0);
	color = texture(colorMap1, uv1*10.0).rgb * heightBlend + texture(colorMap2, normPos.xz * 10.0).rgb * (1 - heightBlend);
	color *= shadow * 0.8 + 0.2;

	color = color * min((ambient + (1.0f - shadow*0.5)*diffuse) + (1.0f - shadow*0.5)*specular, 1.0f);


	color = vec3(length(vec3(1.0,1.0,0.0).xyz - v_v4Position.xyz));
	//color.xyz = normalizedCoords;
	//color.z = normalizedCoords.z;
	//color.xyz = vec3(texture(shadowMap, normalizedCoords.xy).r);
	//color.xyz = vec3(shadow);
	//color = vec4(1.0);
	fcolor = vec4(color, 1.0);
}