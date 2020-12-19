#pragma once
#include "RenderEngine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>

class Demo :
	public RenderEngine
{
public:
	Demo();
	~Demo();
private:
	GLuint shaderProgram, VBO, VAO, EBO, texture, VBO2, VAO2, EBO2, texture2, VBO3, VAO3, EBO3, texture3, 
		VBO4, VAO4, EBO4, texture4, VBO5, VAO5, EBO5, texture5, VBO6, VAO6, EBO6, texture6, VBO7, VAO7, EBO7, texture7, VBO8, VAO8, EBO8, texture8;
	float angle = 0;
	float viewCamX, viewCamY, viewCamZ, upCamX, upCamY, upCamZ, posCamX, posCamY, posCamZ, CAMERA_SPEED, fovy;
	virtual void Init();
	virtual void DeInit();
	virtual void Update(double deltaTime);
	virtual void Render();
	virtual void ProcessInput(GLFWwindow *window);
	void BuildWindow();
	void DrawKaki1();
	void BuildKaki2();
	void DrawKaki2();
	void BuildKaki3();
	void DrawKaki3();
	void BuildKaki4();
	void BuildPapan();
	void DrawWindow();
	void DrawKaki4();
	void BuildFloor();
	void BuildColoredWall();
	void DrawPapan();
	void BuildKaki1();
	void DrawFloor();
	void DrawColoredWall();
	void MoveCamera(float speed);
	void StrafeCamera(float speed);
	void RotateCamera(float speed);
	void InitCamera();

	GLuint karpetVBO, karpetVAO, karpetEBO, karpetTexture;
	GLuint motifKarpetVBO, motifKarpetVAO, motifKarpetEBO, motifKarpetTexture;
	void BuildKarpet();
	void DrawKarpet();
	void BuildMotifKarpet();
	void DrawMotifKarpet();

	GLuint bukuVBO, bukuVAO, bukuEBO, bukuTexture;
	GLuint coverBukuVBO, coverBukuVAO, coverBukuEBO, coverBukuTexture;
	void BuildBuku();
	void DrawBuku();
	void BuildCoverBuku();
	void DrawCoverBuku();

	GLuint VBOL, VAOL, EBOL, textureL,DepanRakBukuTexture, DepanRakBukuVAO, DepanRakBukuVBO, DepanRakBukuEBO;
	void BuildRakBuku();
	void BuildDepanRakBuku();
	void DrawRakBuku();
	void DrawDepanRakBuku();
};

