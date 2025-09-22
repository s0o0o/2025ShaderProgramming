#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);

	// LECTURE 2
	void DrawTest();

	// 0922
	void DrawParticle();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float *newX, float *newY);
	void GenerateParticles(int numParticles);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	/////////////////
	// LECTURE 2

	GLuint m_VBOTestPos = 0; // 아이디를 만들어서 
	GLuint m_VBOTestCol = 0; // 아이디를 만들어서 
	

	// LECTURE 4 (0916)
	GLuint m_TestShader = 0; // 아이디를 만들어서 

	// TIME 관련
	float m_time = 0;

	// PARTICLE
	GLuint m_ParticleShader = 0;
	GLuint m_VBOParticle = 0;
	GLuint m_VBOParticleVertexCount = 0;
};

