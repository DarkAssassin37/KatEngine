#version 430 core
#define MAX_VERTS 48

layout(points) in;
layout(triangle_strip, max_vertices = MAX_VERTS) out;

uniform mat4 mvp;

uniform vec3 geom_Vert[MAX_VERTS];
uniform int geom_Ind[MAX_VERTS];
uniform int geom_INum;
uniform float time;

out vec3 normal;

out vec3 vert;
out vec3 vert1;
out vec3 vert2;
out vec3 vert3;
out float glint;

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

vec3 animpm(vec3 v, float speed, vec3 amount)
{
	return vec3(v.x + sin(v.x + time * speed) * amount.x,
		v.y + sin(v.y + time * speed)* amount.y,
		v.z + sin(v.z + time * speed)* amount.z);
}

void main()
{
	float speed = 5.0;
	
	int depth_limit = 1;

	vec4 curr_Point = mvp * gl_in[0].gl_Position;
	curr_Point /= curr_Point.w;

	curr_Point.xyz = animpm(curr_Point.xyz, 1.5, vec3(0.1 * (1.0 - curr_Point.z) * curr_Point.x,0.1 *  (1.0 - curr_Point.z) * curr_Point.y,0.0));

	float trisize = 0.1 * (1.0 - curr_Point.z);
	if(curr_Point.z > 0.85)
	{
			vec4 v1 = curr_Point +  vec4(0.0,1.0,0.0,0.0) * trisize;
			vec4 v2 = curr_Point +  vec4(-1.0,-1.0,0.0,0.0) * trisize;
			vec4 v3 = curr_Point +  vec4(1.0,-1.0,0.0,0.0) * trisize;

			/*v1.xyz = animpm(v1.xyz, 2.0, vec3(0.0,0.4,0.0));
			v2.xyz = animpm(v2.xyz, 2.0, vec3(0.0,0.4,0.0));
			v3.xyz = animpm(v3.xyz, 2.0, vec3(0.0,0.4,0.0));*/

			normal = -cross((v2-v1).xyz, (v3-v1).xyz);
			vert1 = v1.xyz;
			vert2 = v2.xyz;
			vert3 = v3.xyz;

			float mDist = length(gl_in[0].gl_Position.xyz - vec3(0,0,0)) / 20;
			glint = falloff(mDist, 0.6 + animpm(5.0, 0.1)) + 0.3 * falloff(mDist, 0.90 + animpm(5.0, 0.03));

			gl_Position = v1; vert = v1.xyz; EmitVertex();
			gl_Position = v2; vert = v2.xyz; EmitVertex();
			gl_Position = v3; vert = v3.xyz; EmitVertex();
			/*for(int v = 0; v < 3; v++)
			{
		
				gl_Position = gl_in[0].gl_Position +  mvp * verts[inds[i+v]];
				normal = mvp * verts[inds[i+v]] 
				EmitVertex();
			}*/
			EndPrimitive();
			return;
	}

	for(int i = 0; i < geom_INum; i += 3)//geom_INum
	{
		vec4 v1 = curr_Point +  vec4(geom_Vert[geom_Ind[i+0]]*0.008* trisize, 0.0);
		vec4 v2 = curr_Point +  vec4(geom_Vert[geom_Ind[i+1]]*0.008* trisize, 0.0);
		vec4 v3 = curr_Point +  vec4(geom_Vert[geom_Ind[i+2]]*0.008* trisize, 0.0);

		/*v1.xyz = animpm(v1.xyz, 2.0, vec3(0.0,0.4,0.0));
		v2.xyz = animpm(v2.xyz, 2.0, vec3(0.0,0.4,0.0));
		v3.xyz = animpm(v3.xyz, 2.0, vec3(0.0,0.4,0.0));*/

		normal = -cross((v2-v1).xyz, (v3-v1).xyz);
		vert1 = v1.xyz;
		vert2 = v2.xyz;
		vert3 = v3.xyz;

		float mDist = length(gl_in[0].gl_Position.xyz - vec3(0,0,0)) / 20;
		glint = falloff(mDist, 0.6 + animpm(5.0, 0.1)) + 0.3 * falloff(mDist, 0.90 + animpm(5.0, 0.03));

		gl_Position = v1; vert = v1.xyz; EmitVertex();
		gl_Position = v2; vert = v2.xyz; EmitVertex();
		gl_Position = v3; vert = v3.xyz; EmitVertex();
		/*for(int v = 0; v < 3; v++)
		{
		
			gl_Position = gl_in[0].gl_Position +  mvp * verts[inds[i+v]];
			normal = mvp * verts[inds[i+v]] 
			EmitVertex();
		}*/
		EndPrimitive();
	}
}