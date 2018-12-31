#include "Mesh.h"
#include <windows.h>

#include <iostream>
#include <vector>
// Include GLEW
#include <GL/glew.h>
#include <GL/gl.h>
#include "omp.h"  

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

#include "camera.h"
#include "tessshader.h"
#include "texture.h"
#include "data_process.h"
#include <string>

bool keys[1024];
Camera m_camera;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define TextureWidth 1024
#define TextureHeight 1024

#define pi 3.1415926

LeanShader m_leanshader;
BaseShader m_baseshader;
Mesh m_teapot;
GLuint m_colortex, m_normaltex, m_bumptex, m_bumpvartex;

glm::vec3 lightdir;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		keys[key] = true;
	else
		if (action == GLFW_RELEASE)
			keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		exit(0);
}

void mousecallback(GLFWwindow* window, double xpos, double ypos) {
	if (xpos != 0 && ypos != 0)
		m_camera.SetViewByMouse(xpos, ypos);

	glfwSetCursorPos(window, 0, 0);
}

void InitLeanShader()
{
	m_leanshader.Init();
	vector<float> bump = read_file_doubles("teapot/bump_data");
	vector<float> bump_var = read_file_doubles("teapot/bump_variance_data");

	glGenTextures(1, &m_bumptex);
	glBindTexture(GL_TEXTURE_2D, m_bumptex);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, TextureWidth, TextureHeight, 0, GL_RG, GL_FLOAT, &bump[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &m_bumpvartex);
	glBindTexture(GL_TEXTURE_2D, m_bumpvartex);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, TextureWidth, TextureHeight, 0, GL_RGB, GL_FLOAT, &bump_var[0]);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void InitBaseShader()
{
	m_baseshader.Init();
	m_normaltex = loadTexture("teapot/185_norm.png");
}

void Init()
{
	InitBaseShader();
	m_teapot.LoadMesh("teapot/teapot.obj");
	m_teapot.init();
	m_colortex = loadTexture("teapot/185.png");

	lightdir = glm::vec3(1.0, 1.0, 1.0);
	lightdir = glm::normalize(lightdir);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	m_camera.SetPerspective(glm::radians(60.0f), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01, 100000);

	m_camera.PositionCamera(0, 60, -1100, glm::radians(180.0f), glm::radians(0.0f));
}

void RenderBaseShader()
{
	m_baseshader.Enable();
	m_baseshader.SetColorTex(0);
	m_baseshader.SetNormalTex(1);
	m_baseshader.SetVP(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());
	m_baseshader.SetWorldMatrix(glm::mat4(1.0));
	m_baseshader.SetCameraPos(m_camera.GetPosition());
	m_baseshader.SetLightDirection(lightdir);
	
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colortex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_normaltex);

}

void RenderLeanShader()
{
	m_leanshader.Enable();
	m_leanshader.SetColorTex(0);
	m_leanshader.SetBumpTex(1);
	m_leanshader.SetBumpVarianceTex(2);
	m_leanshader.SetVP(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());
	m_leanshader.SetWorldMatrix(glm::mat4(1.0));
	m_leanshader.SetCameraPos(m_camera.GetPosition());
	m_leanshader.SetLightDirection(lightdir);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colortex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumptex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumpvartex);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	RenderBaseShader();

	m_teapot.render(0);
}