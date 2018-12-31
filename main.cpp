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


LeanShader m_leanshader;
BaseShader m_baseshader;
BaseMesh m_teapot;
GLuint m_colortex, m_normaltex, m_bumptex, m_bumpvartex;

glm::vec3 lightdir;
bool flag;
float maxscale;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		keys[key] = true;
	else
		if (action == GLFW_RELEASE)
			keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		exit(0);
	else if (keys[GLFW_KEY_K])
	{
		flag = !flag;
	}
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

	FILE *f = fopen("teapot/maxscale", "rt");

	while (1) {
		double value = 0.0;
		if (fscanf(f, "%lf", &value) != 1) {
			break;
		}
		maxscale = value;
		if (feof(f)) { break; }
	}
	fclose(f);

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
	flag = false;
}

void Init()
{
	InitLeanShader();
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

	m_camera.PositionCamera(0.0, 0.0, 60.0, glm::radians(0.0f), glm::radians(0.0f));
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
	m_baseshader.SetSSAAflag(flag);
	
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
	m_leanshader.SetMaxscale(maxscale);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colortex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_bumptex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_bumpvartex);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_MULTISAMPLE);

	RenderBaseShader();

	m_teapot.render(0);

/*	glViewport(0.5*WINDOW_WIDTH, 0, 0.5*WINDOW_WIDTH, WINDOW_HEIGHT);*/

//	RenderLeanShader();

//	m_teapot.render(0);
}

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2048);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bezier Patch", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
							 // Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
	//glfwSetCursorPosCallback(window, mousecallback);

	//vsync

	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	//wglSwapIntervalEXT(1);
	wglSwapIntervalEXT(0);

	memset(keys, 0, sizeof(keys));

	Init();

	GLfloat lastTime = glfwGetTime();
	GLfloat currentTime;

	int frame_count = 1;

	glfwSetCursorPos(window, 1024 / 2, 720 / 2);


	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		currentTime = glfwGetTime();

		m_camera.CameraKeyMove(keys, currentTime - lastTime);
		render();
		lastTime = currentTime;

		// Swap buffers
		glfwSwapBuffers(window);
		frame_count++;

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}