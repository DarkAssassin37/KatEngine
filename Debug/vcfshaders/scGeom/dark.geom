#version 430 core
#define MAX_VERTS 128

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
	
	for(int i = 0; i < geom_INum; i += 3)
	{
		vec4 v1 = mvp * gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+0]], 1.0);
		vec4 v2 = mvp * gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+1]], 1.0);
		vec4 v3 = mvp * gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+2]], 1.0);

		float speed = 5.0;
		/*v1.xyz = animpm(v1.xyz, 2.0, vec3(0.0,0.4,0.0));
		v2.xyz = animpm(v2.xyz, 2.0, vec3(0.0,0.4,0.0));
		v3.xyz = animpm(v3.xyz, 2.0, vec3(0.0,0.4,0.0));*/

		normal = -cross((v2-v1).xyz, (v3-v1).xyz);
		vert1 = v1.xyz;
		vert2 = v2.xyz;
		vert3 = v3.xyz;


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