#ifndef BEZIERSHADER_H
#define BEZIERSHADER_H

#include "shaderuniform.h"
#include <glm/glm.hpp>

class BaseShader : public ShaderUniform {
public:
	virtual bool Init();

	void SetVP(const glm::mat4 &MVMatrix);

	void SetWorldMatrix(const glm::mat4 &ProjectionMatrix);

	void SetColorTex(unsigned int TextureUnit);

	void SetNormalTex(unsigned int TextureUnit);

	void SetCameraPos(const glm::vec3 &camera);

	void SetLightDirection(const glm::vec3 &light);

private:
	GLuint m_VPLocation;
	GLuint m_WorldLocation;
	GLuint m_camerapos;
	GLuint m_lightdir;
	GLuint m_colorLocation;
	GLuint m_normalLocation;
};

class LeanShader : public ShaderUniform {
public:
	virtual bool Init();

	void SetVP(const glm::mat4 &MVMatrix);

	void SetWorldMatrix(const glm::mat4 &ProjectionMatrix);

	void SetCameraPos(const glm::vec3 &camera);

	void SetLightDirection(const glm::vec3 &light);

	void SetMaxscale(const float sc);

	void SetColorTex(unsigned int TextureUnit);

	void SetBumpTex(unsigned int TextureUnit);

	void SetBumpVarianceTex(unsigned int TextureUnit);
private:
	GLuint m_sc;
	GLuint m_VPLocation;
	GLuint m_WorldLocation;
	GLuint m_camerapos;
	GLuint m_lightdir;
	GLuint m_colorLocation;
	GLuint m_bumpLocation;
	GLuint m_bumpvarLocation;
};

#endif
