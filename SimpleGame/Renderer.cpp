#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	// 0923 Compile All Shaders
	CompileAllShaderPrograms();

	//Create VBOs
	CreateVertexBufferObjects();

	// Create Grid Mesh 1013
	CreateGridMesh(1000, 1000);

	// Create Particles
	GenerateParticles(10000);

	// 1021 Fill Points
	int index = 0;
	for (int i = 0; i < 400; i++)
	{
		float x = 2 * ((float)rand() / (float)RAND_MAX) - 1;
		float y = 2 * ((float)rand() / (float)RAND_MAX) - 1;
		float st = 10 * ((float)rand() / (float)RAND_MAX);
		float lt = ((float)rand() / (float)RAND_MAX);
		m_Points[index] = x; index++;
		m_Points[index] = y; index++;
		m_Points[index] = st; index++;
		m_Points[index] = lt; index++;
	}

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}


void Renderer::CompileAllShaderPrograms()
{	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");

	// LECTURE4 (0916)
	m_TestShader = CompileShaders("./Shaders/Test.vs", "./Shaders/Test.fs");

	// LECTURE4 (0916)
	m_ParticleShader = CompileShaders("./Shaders/Particles.vs", "./Shaders/Particles.fs");

	// 1013
	m_GridMeshShader = CompileShaders("./Shaders/GridMesh.vs", "./Shaders/GridMesh.fs");

	// 1014
	m_FullScreenShader = CompileShaders("./Shaders/FullScreen.vs", "./Shaders/FullScreen.fs");

}

void Renderer::DeleteAllShaderPrograms()
{
	glDeleteShader(m_SolidRectShader);
	glDeleteShader(m_TestShader);
	glDeleteShader(m_ParticleShader);
	glDeleteShader(m_GridMeshShader);
	glDeleteShader(m_FullScreenShader);
}


bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::ReloadAllShaderPrograms()
{
	DeleteAllShaderPrograms();
	CompileAllShaderPrograms();
}


void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	/////////////////////////////////////
	///////////		LECTURE 2~
	/////////////////////////////////////

	float temp = 0.5f;
	float size = 0.25f;
	float testPos[]
		=
	{
		(0.f - temp) * size, (0.f - temp) * size, 0.f, 0.5,
		(1.f - temp) * size, (0.f - temp) * size, 0.f, 0.5,
		(1.f - temp) * size, (1.f - temp) * size, 0.f, 0.5,	// �ݽð�������� ���ؽ� ��Ƶ־���

		(0.f - temp) * size, (0.f - temp) * size, 0.f,  0.5,
		(1.f - temp) * size, (1.f - temp) * size, 0.f,  0.5,
		(0.f - temp) * size, (1.f - temp) * size, 0.f,  0.5, // Quad1

		(0.f - temp) * size, (0.f - temp) * size, 0.f,  1.0,
		(1.f - temp) * size, (0.f - temp) * size, 0.f,  1.0,
		(1.f - temp) * size, (1.f - temp) * size, 0.f,  1.0,	// �ݽð�������� ���ؽ� ��Ƶ־���

		(0.f - temp) * size, (0.f - temp) * size, 0.f, 1.0,
		(1.f - temp) * size, (1.f - temp) * size, 0.f, 1.0,
		(0.f - temp) * size, (1.f - temp) * size, 0.f, 1.0, // Quad2
	};


	glGenBuffers(1, &m_VBOTestPos);	// (����, ���۷����� �Ѱ���)	
	// �̷��Ը� �ϸ� gpu�� VBO�� ���� �޸𸮰� ���� ������ ���� ����.. �����͸� �÷������
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// testID��� �ָ� ���ڴٴ�.. Ȱ��ȭ�ϰڴٴ� �ǹ�(���� �����Ѱ�..)
	glBufferData(GL_ARRAY_BUFFER, sizeof(testPos), testPos, GL_STATIC_DRAW);

	///////////		LECTURE 3


	float testColor[]
		=
	{
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,		// Triangle1	// �ݽð�������� ���ؽ� ��Ƶ־���
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,

		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,
		1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f
	};


	glGenBuffers(1, &m_VBOTestCol);	// (����, ���۷����� �Ѱ���)	
	// �̷��Ը� �ϸ� gpu�� VBO�� ���� �޸𸮰� ���� ������ ���� ����.. �����͸� �÷������
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestCol);	// testID��� �ָ� ���ڴٴ�.. Ȱ��ȭ�ϰڴٴ� �ǹ�(���� �����Ѱ�..)
	glBufferData(GL_ARRAY_BUFFER, sizeof(testColor), testColor, GL_STATIC_DRAW);

	float fullRect[]
		=
	{
		-1.f , -1.f , 0.f, -1.f , 1.f , 0.f, 1.f , 1.f,  0.f, //Triangle1
		-1.f , -1.f , 0.f,  1.f , 1.f , 0.f, 1.f , -1.f, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBOFullScreen);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullRect), fullRect, GL_STATIC_DRAW);

}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];

	size_t slen = strlen(pShaderText);
	if (slen > INT_MAX) {
		// Handle error
	}
	GLint len = (GLint)slen;

	Lengths[0] = len;
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawTest()
{
	//Program select
	glUseProgram(m_TestShader);

	m_time += 0.005;
	int uTimeLoc = glGetUniformLocation(m_TestShader, "u_Time");
	glUniform1f(uTimeLoc, m_time);

	int aPosLoc = glGetAttribLocation(m_TestShader, "a_Position"); // a_Position��� �Լ��� �޾ƿͼ�
	int aRadiusLoc = glGetAttribLocation(m_TestShader, "a_Radius"); // 0922
	int aColLoc = glGetAttribLocation(m_TestShader, "a_Color"); // lecture3 �÷� �ֱ�

	glEnableVertexAttribArray(aPosLoc);	// enable ���������, attribute�� ����ڰ� �Է��� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// �޾ƿ°� bind�ϰ�
	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT,		// x,y,z ��ǥ�� 3���� �о�Ͷ�
		GL_FALSE, sizeof(float) * 4, 0);

	glEnableVertexAttribArray(aRadiusLoc);	// enable ���������, attribute�� ����ڰ� �Է��� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestPos);	// �޾ƿ°� bind�ϰ�
	glVertexAttribPointer(
		aRadiusLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4,
		(GLvoid*)(sizeof(float) * 3));	// ��� ù �� �о�ö���.. �� ó�� ��ġ? �׷��� radius���� 4��°���� �����ϴϱ� 0 �� �ƴ϶�..

	glEnableVertexAttribArray(aColLoc);	// enable ���������, attribute�� ����ڰ� �Է��� ��
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTestCol);	// �޾ƿ°� bind�ϰ�
	glVertexAttribPointer(
		aColLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * 4, 0);	// m_VBOTestCol ������ �ѹ濡 � �о�÷�? -> 4���ϱ� ����(��Ʈ���̵嵵 ����)
	glDrawArrays(GL_TRIANGLES, 0, 12);		// ��Ʈ���̵�� ��� �ǳʶ۷� �̷���

	glDisableVertexAttribArray(aPosLoc);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawParticle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program select
	GLuint shader = m_ParticleShader;

	glUseProgram(shader);

	m_time += 0.005;
	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_time);
	int uForceLoc = glGetUniformLocation(shader, "u_Force");
	glUniform3f(uForceLoc, std::sin(m_time), 0, 0);

	int aPosLoc = glGetAttribLocation(shader, "a_Position"); // a_Position��� �Լ��� �޾ƿͼ�
	int aValueLoc = glGetAttribLocation(shader, "a_Value"); // 0922
	int aColLoc = glGetAttribLocation(shader, "a_Color"); // lecture3 �÷� �ֱ�
	int aSTimeLoc = glGetAttribLocation(shader, "a_STime"); // 0923
	int aVelLoc = glGetAttribLocation(shader, "a_Vel"); // 0923
	int aLifeTImeLoc = glGetAttribLocation(shader, "a_LifeTime"); // 0929
	int aMassLoc = glGetAttribLocation(shader, "a_Mass"); // 0929
	int aPeriodLoc = glGetAttribLocation(shader, "a_Period"); // 0929

	int stride = 15;
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);	// �޾ƿ°� bind�ϰ�
	glEnableVertexAttribArray(aPosLoc);	// enable ���������, attribute�� ����ڰ� �Է��� �� 
	glEnableVertexAttribArray(aValueLoc);	// enable ���������, attribute�� ����ڰ� �Է��� ��
	glEnableVertexAttribArray(aColLoc);
	glEnableVertexAttribArray(aSTimeLoc);
	glEnableVertexAttribArray(aVelLoc);
	glEnableVertexAttribArray(aLifeTImeLoc);
	glEnableVertexAttribArray(aMassLoc);
	glEnableVertexAttribArray(aPeriodLoc);

	glVertexAttribPointer(
		aPosLoc, 3, GL_FLOAT,		// x,y,z ��ǥ�� 3���� �о�Ͷ�
		GL_FALSE, sizeof(float) * stride, 0);	// ��Ʈ���̵�� ��� �ǳʶ۷� �̷���

	glVertexAttribPointer(
		aValueLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 3));	// ��� ù �� �о�ö���.. �� ó�� ��ġ? �׷��� radius���� 4��°���� �����ϴϱ� 0 �� �ƴ϶�..

	glVertexAttribPointer(
		aColLoc, 4, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 4));	// m_VBOTestCol ������ �ѹ濡 � �о�÷�? -> 4���ϱ� ����(��Ʈ���̵嵵 ����)

	glVertexAttribPointer(
		aSTimeLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 8));

	glVertexAttribPointer(
		aVelLoc, 3, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 9));

	glVertexAttribPointer(
		aLifeTImeLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 12));

	glVertexAttribPointer(
		aMassLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 13));

	glVertexAttribPointer(
		aPeriodLoc, 1, GL_FLOAT,
		GL_FALSE, sizeof(float) * stride,
		(GLvoid*)(sizeof(float) * 14));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOParticleVertexCount);		// ��Ʈ���̵�� ��� �ǳʶ۷� �̷���
	//glDrawArrays(GL_TRIANGLES, 0, 6);		// ��Ʈ���̵�� ��� �ǳʶ۷� �̷���

	glDisableVertexAttribArray(aPosLoc);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
}


void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::GenerateParticles(int numParticles)
{
	int floatCountPerVertex = 3 + 1 + 4 + 1 + 3 + 1 + 1 + 1;
	// x,y,z, value, r,g,b,a, sTime(�����Ǵ½ð�), vx,vy,vz, lifeTime, mass, period
	int verticesCountPerParticle = 6;
	int floatCountPerParticle =
		floatCountPerVertex * verticesCountPerParticle;
	int totalVerticesCount = numParticles * verticesCountPerParticle;
	int totlaFloatCount = floatCountPerVertex * totalVerticesCount;

	float* vertices = new float[totlaFloatCount];

	for (int i = 0; i < numParticles; ++i) {
		float x, y, z, value, r, g, b, a;
		x = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		y = 0; //((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		z = 0.f;

		value = ((float)rand() / (float)RAND_MAX);

		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = 1;// ((float)rand() / (float)RAND_MAX);

		float size;
		size = ((float)rand() / (float)RAND_MAX) * 0.01f;
		float sTime = ((float)rand() / (float)RAND_MAX) * 2.0; // 0~2 ���� �� 

		float vx, vy, vz;
		vx = 0; // (((float)rand() / (float)RAND_MAX) * 2.f - 1.f) * 1.5f;	// 
		vy = 0; // ((((float)rand() / (float)RAND_MAX) * 2.f - 1.f) + 2.5f) * 1.5f;	// 
		vz = 0.f;

		float lifeTime;
		lifeTime = (float)rand() / ((float)RAND_MAX);

		float mass = (float)rand() / ((float)RAND_MAX) + 1;
		float period = (float)rand() / ((float)RAND_MAX);

		int index = i * floatCountPerVertex * verticesCountPerParticle;
		vertices[index] = x - size; index++;	// v1
		vertices[index] = y - size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

		vertices[index] = x + size; index++;	// v2
		vertices[index] = y + size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

		vertices[index] = x - size; index++;	// v3
		vertices[index] = y + size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

		vertices[index] = x - size; index++;	// v4
		vertices[index] = y - size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

		vertices[index] = x + size; index++;	// v5
		vertices[index] = y - size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

		vertices[index] = x + size; index++;	// v3
		vertices[index] = y + size; index++;
		vertices[index] = z; index++;
		vertices[index] = value; index++;
		vertices[index] = r; index++;
		vertices[index] = g; index++;
		vertices[index] = b; index++;
		vertices[index] = a; index++;
		vertices[index] = sTime; index++;
		vertices[index] = vx; index++;
		vertices[index] = vy; index++;
		vertices[index] = vz; index++;
		vertices[index] = lifeTime; index++;
		vertices[index] = mass; index++;
		vertices[index] = period; index++;

	}

	glGenBuffers(1, &m_VBOParticle);	// (����, ���۷����� �Ѱ���)	
	// �̷��Ը� �ϸ� gpu�� VBO�� ���� �޸𸮰� ���� ������ ���� ����.. �����͸� �÷������
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOParticle);	// testID��� �ָ� ���ڴٴ�.. Ȱ��ȭ�ϰڴٴ� �ǹ�(���� �����Ѱ�..)
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * totlaFloatCount,
		vertices, GL_STATIC_DRAW);

	delete[] vertices;

	m_VBOParticleVertexCount = totalVerticesCount;
}


// 251013
void Renderer::CreateGridMesh(int x, int y)
{
	float basePosX = -1.f;
	float basePosY = -1.f;
	float targetPosX = 1.f;
	float targetPosY = 1.f;

	int pointCountX = x;
	int pointCountY = y;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX * pointCountY * 2];
	float* vertices = new float[(pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3];
	m_GridMeshVertexCount = (pointCountX - 1) * (pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y * pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y * pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1) * pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}


	glGenBuffers(1, &m_GridMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (pointCountX - 1) * (pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);

	delete[] point;
	delete[] vertices;
}



void Renderer::DrawGridMesh()
{
	m_time += 0.005;

	//1021
	float points[12] = { 0,0,2,2,
						0.5,0,3,3,
						-0.5,0,4,4 };

	int shader = m_GridMeshShader;
	glUseProgram(shader);

	int uTimeLoc = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTimeLoc, m_time);

	// 1021 points�� array�� �־.. ����
	int uPointsLoc = glGetUniformLocation(shader, "u_Points");
	glUniform4fv(uPointsLoc, 100, m_Points);	// uPoints�� array�� glUniform4fv


	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_GridMeshVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_GridMeshVertexCount);
	//glDrawArrays(GL_LINE_STRIP, 0, m_GridMeshVertexCount);

	glDisableVertexAttribArray(attribPosition);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawFullScreenColor(float r, float g, float b, float a)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int shader = m_FullScreenShader;
	float newX, newY;

	//Program select
	glUseProgram(shader);

	glUniform4f(glGetUniformLocation(shader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOFullScreen);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
