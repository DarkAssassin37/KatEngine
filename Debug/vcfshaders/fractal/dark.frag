#version 430 core

in vec3 normal;
in vec3 vert;
in vec3 vert1;
in vec3 vert2;
in vec3 vert3;

out vec4 color;

uniform float time;

float mapl(float x, float a, float b, float c, float d)
{
	return clamp(c+(d-c)*(x-a)/(b-a),c,d);
}

vec3 bary()
{
	vec3 v0 = vert2 - vert1, v1 = vert3 - vert1, v2 = vert - vert1;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
	return vec3(u,v,w);
}

float minDist(vec3 baryCoord)
{
	return min(min(baryCoord.x,baryCoord.y),baryCoord.z);
}

float falloff(float dist, float intesity)
{
	return 1.0 / (pow(1.0/(intesity * intesity), 50.0*dist));
}

vec3 mixColors(vec3 surface, vec3 light, float lint)
{
    return mix(surface * light, light, pow(lint, 20.0)) * lint;
}

float animpm(float speed, float amount)
{
	return sin(time * speed) * amount;
}

void main()
{
	vec3 basecol = vec3(0.1, 0.0, 0.5);
	//vec3 partcol = vec3(1.0, 0.1, 0.7);//pink neon
	vec3 partcol = vec3(0.7, 0.1, 1.0);//blue
	vec3 light = vec3(1.0, 1.0, 1.0);
	

	vec3 baryCoord = bary();
	float mDist = minDist(baryCoord);

	//float edgeDist = falloff(mDist, 0.6 + animpm(5.0, 0.1)) + 0.3 * falloff(mDist, 0.96 + animpm(5.0, 0.03));
	float edgeDist = falloff(mDist, 0.8) + 0.4 * falloff(mDist, 0.96);


	//float lint = pow(edgeDist - 0.2 + animpm(5.0, 0.1), 0.454545);//0.454545
	float lint = edgeDist;//0.454545

	lint = clamp(lint, 0.0, 1.0);

	vec3 surfColor = edgeDist * partcol + (1.0 - edgeDist) * basecol * 0.8;

	//color = vec4(mixColors(surfColor,light,lint), 1.0);
	float intesity = (1.0-gl_FragCoord.z*0.99);

	//intesity = 1.0 - intesity;
	intesity = 1.0-clamp(falloff(intesity, 0.8) + 0.4 * falloff(intesity, 0.96),0.0,1.0);
    //intesity = pow(intesity, 0.1);
	color.xyz = vec3(intesity) + mixColors(surfColor,light,lint) * intesity;
	color.w = 1.0;
}
