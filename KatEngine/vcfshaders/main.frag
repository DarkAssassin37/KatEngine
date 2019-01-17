#version 430 core
in vec4 v_v4Position;
in vec4 v_v4Normal;
out vec4 fcolor;

in vec4 fragPosLightSpace;

uniform sampler2D colorMap1;
uniform sampler2D colorMap2;
uniform sampler2D shadowMap;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPosition;

uniform	mat3 normalMatrix;
uniform mat3 lightDirMatrix;

uniform	vec3 dirlightColor;
uniform	vec3 lightDir;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 
#define NR_POINT_LIGHTS 2
uniform PointLight pointLights[2];

vec3 ambient;
float ambientStrength = 0.35f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 10.0f;

vec3 ambientPoint = vec3(0.0f);
vec3 diffusePoint = vec3(0.0f);
vec3 specularPoint = vec3(0.0f);

void computeLightComponentsDirectional()
{		
	vec3 normalEye = normalize(normalMatrix * v_v4Normal.xyz);	
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	
	vec3 viewDirN = normalize(camPosition - v_v4Position.xyz);
	vec3 halfVector = normalize(lightDirN + viewDirN);

	//compute ambient light
	ambient = ambientStrength * dirlightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * dirlightColor;
	
	//compute specular light
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);	
	specular = specularStrength * specCoeff * dirlightColor;
}

void CalcPointLight(PointLight light)
{
	vec3 viewDirN = normalize(camPosition - v_v4Position.xyz);
	vec3 fragPos = (model * v_v4Position).xyz;
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(v_v4Normal.xyz, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, v_v4Normal.xyz);
    float spec = pow(max(dot(viewDirN, reflectDir), 0.0), shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));  

    // combine results
    ambientPoint  += light.ambient  * attenuation;
    diffusePoint  += light.diffuse  * diff * attenuation;
    specularPoint += light.specular * spec * attenuation;
	//ambient = vec3(0.0f);
	//specular = vec3(0.0f);
	//diffuse = vec3(attenuation);
	//diffuse = clamp(diffuse,0.0,1.0);
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
	float linear = 5.0;
	float quadratic = 10.0;
	vec3 color;
	computeLightComponentsDirectional();
		PointLight p1 = {
		vec3(0.1, 0.2, 0), //pos
		1.0,
		linear,
		quadratic,
		vec3(1.0f, 0.78f, 0.78f) * 0.1f, //ambient
		vec3(0.51f, 0.86f, 1.0f), //diffuse
		vec3(0.31f, 0.648f, 0.78f) //specular
	};
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
       CalcPointLight(pointLights[i]);
	//CalcPointLight(pointLights[0]);

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
	vec3 difcolor = texture(colorMap1, uv1*10.0).rgb * heightBlend + texture(colorMap2, normPos.xz * 10.0).rgb * (1 - heightBlend);
	color = difcolor;
	color *= shadow * 0.5 + 0.5;

	color = color * min(ambient + diffuse + specular, 1.0f);

	color +=  min(ambientPoint + diffusePoint + specularPoint, 1.0f);

	float snow = dot(vec3(0,1,0), v_v4Normal.xyz);
	snow += pow(1.0 - v_v4Position.y,5.0);
	snow = min(snow, 1.0);

	color = color * (1-snow) + snow * vec3(1.0,1.0,1.0);

	vec3 fogColor = vec3(0.9f,0.9f,1.0f)*0.7;

	float fogDensity = 0.7f;
	float fragmentDistance = length(projection * view * model * v_v4Position);
	fragmentDistance = 1.0 - fragmentDistance;
	float fogStr = exp(-pow(fragmentDistance * fogDensity, 2));

	fogStr = 1.0 - fogStr;
	fogStr = clamp(fogStr, 0.0, 1.0);

	color = color * (1-fogStr) + fogStr * fogColor;


	//color.xyz = normalizedCoords;
	//color.z = normalizedCoords.z;
	//color.xyz = vec3(texture(shadowMap, normalizedCoords.xy).r);
	//color.xyz = vec3(shadow);
	//color = vec4(1.0);
	fcolor = vec4(color, 1.0);
}
