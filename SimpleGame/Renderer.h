#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cassert>

#include "Dependencies\glew.h"
#include "LoadPng.h"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	bool IsInitialized();
	void ReloadAllShaderPrograms(); // 0923 
	void DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a);
	void DrawFullScreenColor(float r, float g, float b, float a);	//1013
	void DrawFS();
	void DrawTexture(float x, float y, float sx, float sy, GLuint TexID);
	void DrawDebugTexture();
	void DrawFBOs(); // 0125

	// LECTURE 2
	void DrawTest();
	// 0922
	void DrawParticle();
	// 1013
	void DrawGridMesh();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string* target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects();
	void GetGLPosition(float x, float y, float* newX, float* newY);
	void GenerateParticles(int numParticles);
	void CompileAllShaderPrograms();
	void DeleteAllShaderPrograms();
	void CreateGridMesh(int x, int y);	//1013
	GLuint CreatePngTexture(char* filePath, GLuint samplingMethod); // 1117
	void CreateFBOs(); // /125

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

	// M_Grid
	GLuint m_GridMeshVertexCount = 0;
	GLuint m_GridMeshVBO = 0;
	GLuint m_GridMeshShader = 0;

	// Full Screen
	GLuint m_VBOFullScreen = 0;
	GLuint m_FullScreenShader = 0;

	// 1021 For raindrop effect
	float m_Points[100 * 4];

	// for fragment shader factory 1111
	GLuint m_VBOFS = 0;
	GLuint m_FSShader = 0;

	// 1117
	GLuint m_RGBTexture = 0;
	GLuint m_ChaTexture = 0;

	// 1124
	GLuint m_0Texture = 0;
	GLuint m_1Texture = 0;
	GLuint m_2Texture = 0;
	GLuint m_3Texture = 0;
	GLuint m_4Texture = 0;
	GLuint m_5Texture = 0;
	GLuint m_6Texture = 0;
	GLuint m_7Texture = 0;
	GLuint m_8Texture = 0;
	GLuint m_9Texture = 0;
	GLuint m_NumTexture = 0;

	// 1125 Texture
	GLuint m_TexVBO = 0;
	GLuint m_TexShader = 0;

	// FBO Color buffers
	GLuint m_RT0_0 = 0;
	GLuint m_RT0_1 = 0;
	GLuint m_RT1_0 = 0;
	GLuint m_RT1_1 = 0;
	GLuint m_RT2 = 0;
	GLuint m_RT3 = 0;
	GLuint m_RT4 = 0;
	
	GLuint m_FBO0 = 0;
	GLuint m_FBO1 = 0;
	GLuint m_FBO2 = 0;
	GLuint m_FBO3 = 0;
	GLuint m_FBO4 = 0;
	GLuint m_FBO5 = 0;


};

