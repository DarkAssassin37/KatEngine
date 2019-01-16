#include "TerraGen.h"
#include <glad/glad.h>
#include "Shader.h"
#include "ProgramShader.h"
#include "Texture.h"
#include  "messages.h"
#define glCheckError() glCheckError_(__FILE__, __LINE__) 



TerraGen::TerraGen()
{
	Shader onemoreVertShader("vcfshaders\\onemore.vert", Shader::VERTEX_SHADER);
	Shader onemoreFragShader("vcfshaders\\onemore.frag", Shader::FRAGMENT_SHADER);
	onemoreProgShader = ProgramShader(onemoreVertShader, onemoreFragShader);

	glCheckError();
	generatedVAO = GenerateObject();
}

void TerraGen::Draw()
{
	DrawArrayVAO(generatedVAO, onemoreProgShader);
}


void TerraGen::DrawArrayVAO(GLuint vao, GLuint program)
{
	//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	//glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	// Bind Vertex and Fragment rendering shaders
	glBindVertexArray(vao);

	glUseProgram(program);

	//glEnableVertexAttribArray(iLocPosition);
	//glEnableVertexAttribArray(iLocFillColor);

	// Draw points from VBO

	glDrawArrays(GL_POINTS, 0, 512 * 512);
	glBindVertexArray(0);
}

GLuint TerraGen::GenerateObject()
{
	auto vbo = Precompute();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;
}

GLuint TerraGen::Precompute()
{
	int NUM_VERTS_H = 512;
	int NUM_VERTS_V = 512;
	int GROUP_SIZE_WIDTH = 8;
	int GROUP_SIZE_HEIGHT = 8;
	GLuint res;
	glGenBuffers(1, &res);
	GLuint ebo;
	glGenBuffers(1, &ebo);

	Shader objGenShader("vcfshaders\\objGen.comp", Shader::COMPUTE_SHADER);
	ProgramShader objGenProg(objGenShader);

	glCheckError();

	glUseProgram(objGenProg);
	//glUniform1f(glGetUniformLocation(csprogShader, "roll"), interp*10);
	//auto loc = glGetUniformLocation(objGenProg, "eye");


	// gIndexBufferBinding is equal to 0 (same as the compute shader binding)
	int outBuffer = 0;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, res);
	float* zeros = new float[NUM_VERTS_H * NUM_VERTS_V * 16];
	for (int i = 0; i < NUM_VERTS_H *NUM_VERTS_V; i++)
		zeros[i] = 0.0f;

	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_VERTS_H *NUM_VERTS_V * sizeof(float) * 16, zeros, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, outBuffer, res); //outbuffer binding = 0


	int outBuffer2 = 1;//do math!!!
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ebo);
	GLuint* zerose = new GLuint[NUM_VERTS_H * NUM_VERTS_V * 16];
	for (int i = 0; i < NUM_VERTS_H *NUM_VERTS_V; i++)
		zerose[i] = 0.0f;

	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_VERTS_H * NUM_VERTS_V * sizeof(float) * 16, zerose, GL_STATIC_DRAW);
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

	Texture heightmap("Textures\\Map.png");

	heightmap.bindTexture(0);

	glDispatchCompute(NUM_VERTS_H / GROUP_SIZE_WIDTH, NUM_VERTS_V / GROUP_SIZE_HEIGHT, 1);
	//wait for it to end
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	// gIndexBufferBinding is equal to 0 (same as the compute shader binding)
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, outBuffer, 0);

	return res;
}


TerraGen::~TerraGen()
{
}
