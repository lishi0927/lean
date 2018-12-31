#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "tessshader.h"

#define MaxPoint 10000

struct BoundingBox
{
	glm::vec3 minpoint;
	glm::vec3 maxpoint;
	glm::vec3 boundingsize;
};

class BaseMesh {
public:
	BaseMesh();
	~BaseMesh();

	void LoadMesh(const char* filename);
	void InitVertices();

	virtual void init();

	BoundingBox m_bounding;

	glm::vec3 m_center;

protected:
	int m_facenumber;
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_coordinates;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::ivec3> m_faces;

	GLuint vertexbuffer, uvbuffer, normalbuffer;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
};

class Mesh : public BaseMesh{
public:
	Mesh();
	~Mesh();

	void render(int state);

	virtual void init();

	OriShader m_shader;	
};

class DisMesh : public BaseMesh
{
public:
	DisMesh();
	~DisMesh();

	virtual void init();
	void render();

	TessShader m_shader;

	void SetNormalTextureName(std::string name);
	void SetDisTextureName(std::string name);
	void SetParaName(std::string name);

private:
	std::string nortexname;
	std::string distexname;
	std::string paraname;

	GLuint m_nortex;
	GLuint m_distex;

	float scale, bias;
};


#endif
