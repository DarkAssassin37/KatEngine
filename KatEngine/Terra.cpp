#include "Terra.h"
#include <glad/glad.h>
#include "Texture.h"
#include "Shader.h"
#include "ProgramShader.h"
#include "messages.h"
#include <vector>
#define glCheckError() glCheckError_(__FILE__, __LINE__) 


Terra::Terra(int vertWidth, int vertHeight, const char * texturePath)
{
	width = vertWidth;
	height = vertHeight;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//GLuint res;
	glGenBuffers(1, &vbo);
	//GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)4);



	Shader objGenShader("vcfshaders\\objGen.comp", Shader::COMPUTE_SHADER);
	ProgramShader objGenProg(objGenShader);

	glCheckError();

	glUseProgram(objGenProg);
	//glUniform1f(glGetUniformLocation(csprogShader, "roll"), interp*10);
	//auto loc = glGetUniformLocation(objGenProg, "eye");


	// gIndexBufferBinding is equal to 0 (same as the compute shader binding)
	int outBuffer = 0;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	float* zeros = new float[vertWidth * vertHeight * 16];
	for (int i = 0; i < vertWidth * vertHeight * 16; i++)
		zeros[i] = 0.0f;

	glBufferData(GL_SHADER_STORAGE_BUFFER, vertWidth * vertHeight * sizeof(float) * 16, zeros, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, outBuffer, vbo); //outbuffer binding = 0


	int outBuffer2 = 1;//do math!!!
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ebo);
	int indicescount = (vertWidth - 1) * (vertHeight - 1) * 6;
	GLuint* zerose = new GLuint[indicescount];
	for (int i = 0; i < indicescount; i++)
		zerose[i] = 0;

	glBufferData(GL_SHADER_STORAGE_BUFFER, indicescount * sizeof(GLuint), zerose, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, outBuffer2, ebo); //outbuffer binding = 0*/

	  // Submit job for the compute shader execution.
	  // GROUP_SIZE_HEIGHT = GROUP_SIZE_WIDTH = 8
	  // NUM_VERTS_H = NUM_VERTS_V = 16
	  // As the result the function is called with the following parameters:
	  // glDispatchCompute(2, 2, 1)

	  /*glDispatchCompute(
	  (NUM_VERTS_H % GROUP_SIZE_WIDTH + NUM_VERTS_H) / GROUP_SIZE_WIDTH,
	  (NUM_VERTS_V % GROUP_SIZE_HEIGHT + NUM_VERTS_V) / GROUP_SIZE_HEIGHT,
	  1);*/

	Texture heightmap(texturePath);

	heightmap.bindTexture(0);

	glDispatchCompute(vertWidth / GROUP_SIZE_WIDTH, vertHeight / GROUP_SIZE_HEIGHT, 1);
	//wait for it to end
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	// gIndexBufferBinding is equal to 0 (same as the compute shader binding)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, outBuffer, 0);


	precomputeEBO();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader onemoreVertShader("vcfshaders\\onemore.vert", Shader::VERTEX_SHADER);
	Shader onemoreFragShader("vcfshaders\\onemore.frag", Shader::FRAGMENT_SHADER);
	onemoreProgShader = ProgramShader(onemoreVertShader, onemoreFragShader);
}

void Terra::precomputeEBO()
{
	vector<unsigned> ebo;
	/*for (int i = 0; i < (NUM_VERTS_H - 1) * (NUM_VERTS_V - 1) * 6; i++)
	{
	ebo.push_back(i);
	ebo.push_back(i + NUM_VERTS_V);
	ebo.push_back(i +1);

	ebo.push_back(i);
	ebo.push_back(i + NUM_VERTS_V);
	ebo.push_back(i + 1);

	outBuffer2.data[offset * 6 + 3] = offset + 1;
	outBuffer2.data[offset * 6 + 4] = offsetNextRow + 0;
	outBuffer2.data[offset * 6 + 5] = offsetNextRow + 1;
	}*/

	for (int y = 0; y < height - 1; y++)
		for (int x = 0; x < width - 1; x++)
		{
			int off = y * height + x;
			ebo.push_back(off);
			ebo.push_back(off + width);
			ebo.push_back(off + 1);

			ebo.push_back(off + 1);
			ebo.push_back(off + width);
			ebo.push_back(off + width + 1);
		}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.size() * sizeof(unsigned int), &ebo[0], GL_STATIC_DRAW);
}

void Terra::Draw()
{
	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	//glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	// Bind Vertex and Fragment rendering shaders
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


	glUseProgram(onemoreProgShader);

	//glEnableVertexAttribArray(iLocPosition);
	//glEnableVertexAttribArray(iLocFillColor);

	// Draw points from VBO

	//glDrawArrays(GL_POINTS, 0, 512 * 512);
	glDrawElements(GL_TRIANGLES, (width - 1) * (height - 1) * 6, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

Terra::~Terra()
{
}
