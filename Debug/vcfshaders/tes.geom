#version 430 core
#define MAX_VERTS 128

layout(points) in;
layout(triangle_strip, max_vertices = MAX_VERTS) out;

uniform mat4 mvp;

uniform vec3 geom_Vert[MAX_VERTS];
uniform int geom_Ind[MAX_VERTS];
uniform int geom_INum;

out vec3 normal;

void main()
{
	
	for(int i = 0; i < geom_INum; i += 3)
	{
		vec4 v1 = gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+0]], 1.0);
		vec4 v2 = gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+1]], 1.0);
		vec4 v3 = gl_in[0].gl_Position +  mvp * vec4(geom_Vert[geom_Ind[i+2]], 1.0);

		normal = -cross((v2-v1).xyz, (v3-v1).xyz);

		gl_Position = v1; EmitVertex();
		gl_Position = v2; EmitVertex();
		gl_Position = v3; EmitVertex();
		/*for(int v = 0; v < 3; v++)
		{
		
			gl_Position = gl_in[0].gl_Position +  mvp * verts[inds[i+v]];
			normal = mvp * verts[inds[i+v]] 
			EmitVertex();
		}*/
		EndPrimitive();
	}
}