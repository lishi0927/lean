#include "shader.h"
#include "tessshader.h"

bool BaseShader::Init()
{
	shader = LoadShaders("shader/basevert.glsl", "shader/basefrag.glsl");

	m_WorldLocation = glGetUniformLocation(shader, "gWorldMatrix");
	
	m_VPLocation = glGetUniformLocation(shader, "gVPMatrix");

	m_camerapos = glGetUniformLocation(shader, "cameraPos");

	m_lightdir = glGetUniformLocation(shader, "lightdir");

	m_colorLocation = glGetUniformLocation(shader, "colortex");

	m_normalLocation = glGetUniformLocation(shader, "normaltex");

	return true;
}

void BaseShader::SetVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void BaseShader::SetWorldMatrix(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WorldLocation, 1, GL_FALSE, &WVP[0][0]);
}

void BaseShader::SetCameraPos(const glm::vec3 &camera)
{
	glUniform3f(m_camerapos, camera.x, camera.y, camera.z);
}

void BaseShader::SetLightDirection(const glm::vec3 &light)
{
	glUniform3f(m_lightdir, light.x, light.y, light.z);
}

void BaseShader::SetColorTex(unsigned int TextureUnit)
{
	glUniform1i(m_colorLocation, TextureUnit);
}

void BaseShader::SetNormalTex(unsigned int TextureUnit)
{
	glUniform1i(m_normalLocation, TextureUnit);
}

bool LeanShader::Init()
{
	shader = LoadShaders("shader/leanvert.glsl", "shader/leanfrag.glsl");

	m_WorldLocation = glGetUniformLocation(shader, "gWorldMatrix");

	m_VPLocation = glGetUniformLocation(shader, "gVPMatrix");

	m_camerapos = glGetUniformLocation(shader, "cameraPos");

	m_lightdir = glGetUniformLocation(shader, "lightdir");

	m_sc = glGetUniformLocation(shader, "sc");

	m_colorLocation = glGetUniformLocation(shader, "colortex");

	m_bumpLocation = glGetUniformLocation(shader, "bumptex");

	m_bumpvarLocation = glGetUniformLocation(shader, "bumpvartex");

	return true;
}

void LeanShader::SetVP(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_FALSE, &WVP[0][0]);
}

void LeanShader::SetWorldMatrix(const glm::mat4 &WVP)
{
	glUniformMatrix4fv(m_WorldLocation, 1, GL_FALSE, &WVP[0][0]);
}

void LeanShader::SetCameraPos(const glm::vec3 &camera)
{
	glUniform3f(m_camerapos, camera.x, camera.y, camera.z);
}

void LeanShader::SetLightDirection(const glm::vec3 &light)
{
	glUniform3f(m_lightdir, light.x, light.y, light.z);
}

void LeanShader::SetMaxscale(const float sc)
{
	glUniform1f(m_sc, sc);
}

void LeanShader::SetColorTex(unsigned int TextureUnit)
{
	glUniform1i(m_colorLocation, TextureUnit);
}

void LeanShader::SetBumpTex(unsigned int TextureUnit)
{
	glUniform1i(m_bumpLocation, TextureUnit);
}

void LeanShader::SetBumpVarianceTex(unsigned int TextureUnit)
{
	glUniform1i(m_bumpvarLocation, TextureUnit);
}
