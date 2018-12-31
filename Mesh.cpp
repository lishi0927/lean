#include "Mesh.h"
#include "texture.h"
#include <fstream>
#include <string>

using namespace std;

BaseMesh::BaseMesh()
{
	m_vertices.clear();
	m_coordinates.clear();
	m_normals.clear();
	m_facenumber = 0;

	m_bounding.minpoint = glm::vec3(MaxPoint, MaxPoint, MaxPoint);
	m_bounding.maxpoint = glm::vec3(-MaxPoint, -MaxPoint, -MaxPoint);

	vertices.clear();
	uvs.clear();
	normals.clear();
}

BaseMesh::~BaseMesh()
{
	m_vertices.clear();
	m_coordinates.clear();
	m_normals.clear();

	vertices.clear();
	uvs.clear();
	normals.clear();
}


void BaseMesh::LoadMesh(const char* filename)
{
	fstream f(filename);
	char line[256];
	float f1, f2, f3;
	while (!f.getline(line, 255).eof())
	{
		line[255] = '\0';
		if (line[0] == 'v')
		{
			if (sscanf(line, "v %f %f %f", &f1, &f2, &f3) == 3)
			{
				this->m_vertices.push_back(glm::vec3(f1, f2, f3));
				if (f1 > m_bounding.maxpoint.x) m_bounding.maxpoint.x = f1;
				if (f1 < m_bounding.minpoint.x) m_bounding.minpoint.x = f1;
				if (f2 > m_bounding.maxpoint.y) m_bounding.maxpoint.y = f2;
				if (f2 < m_bounding.minpoint.y) m_bounding.minpoint.y = f2;
				if (f3 > m_bounding.maxpoint.z) m_bounding.maxpoint.z = f3;
				if (f3 < m_bounding.minpoint.z) m_bounding.minpoint.z = f3;
			}
			else if (sscanf(line, "vt %f %f", &f1, &f2) == 2)
			{
				this->m_coordinates.push_back(glm::vec2(f1, f2));
			}
			else if (sscanf(line, "vn %f %f %f", &f1, &f2, &f3) == 3)
			{
				this->m_normals.push_back(glm::vec3(f1, f2, f3));
			}
		}
		else if (line[0] == 'f')
		{
			int i11, i12, i13, i21, i22, i23, i31, i32, i33, i41, i42, i43;
			if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &i11, &i12, &i13, &i21, &i22, &i23, &i31, &i32, &i33, &i41, &i42, &i43) == 12)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i21, i22, i23));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));

				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));
				this->m_faces.push_back(glm::ivec3(i41, i42, i43));
			}
			else if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &i11, &i12, &i13, &i21, &i22, &i23, &i31, &i32, &i33) == 9)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, i13));
				this->m_faces.push_back(glm::ivec3(i21, i22, i23));
				this->m_faces.push_back(glm::ivec3(i31, i32, i33));
			}
			else if (sscanf(line, "f %d/%d %d/%d %d/%d", &i11, &i12, &i21, &i22, &i31, &i32) == 6)
			{
				this->m_faces.push_back(glm::ivec3(i11, i12, -1));
				this->m_faces.push_back(glm::ivec3(i21, i22, -1));
				this->m_faces.push_back(glm::ivec3(i31, i32, -1));
			}
			else if (sscanf(line, "f %d %d %d", &i11, &i21, &i31) == 3)
			{
				this->m_faces.push_back(glm::ivec3(i11, -1, -1));
				this->m_faces.push_back(glm::ivec3(i21, -1, -1));
				this->m_faces.push_back(glm::ivec3(i31, -1, -1));
			}
			else if (sscanf(line, "f %d//%d %d//%d %d//%d", &i11, &i13, &i21, &i23, &i31, &i33) == 6)
			{
				this->m_faces.push_back(glm::ivec3(i11, -1, i13));
				this->m_faces.push_back(glm::ivec3(i21, -1, i23));
				this->m_faces.push_back(glm::ivec3(i31, -1, i33));
			}
		}
	}

	m_facenumber = m_faces.size() / 3;

	m_bounding.boundingsize = m_bounding.maxpoint - m_bounding.minpoint;
	m_center = (m_bounding.maxpoint + m_bounding.minpoint) / float(2.0);
}


void BaseMesh::InitVertices()
{
	for (int i = 0; i < m_facenumber; i++)
	{
		if (m_coordinates.size() != 0)
		{
			glm::vec2 vert_coord1 = m_coordinates[m_faces[i * 3].g - 1];
			glm::vec2 vert_coord2 = m_coordinates[m_faces[i * 3 + 1].g - 1];
			glm::vec2 vert_coord3 = m_coordinates[m_faces[i * 3 + 2].g - 1];

			uvs.push_back(vert_coord1);
			uvs.push_back(vert_coord2);
			uvs.push_back(vert_coord3);
		}


		glm::vec3 vert_pos1 = m_vertices[m_faces[i * 3].r - 1];
		glm::vec3 vert_pos2 = m_vertices[m_faces[i * 3 + 1].r - 1];
		glm::vec3 vert_pos3 = m_vertices[m_faces[i * 3 + 2].r - 1];

		vertices.push_back(vert_pos1);
		vertices.push_back(vert_pos2);
		vertices.push_back(vert_pos3);

		glm::vec3 normal = glm::normalize(glm::cross(vert_pos2 - vert_pos1, vert_pos3 - vert_pos1));

		if (m_normals.size() == 0)
		{
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
		}
		else
		{
			normals.push_back(m_normals[m_faces[i * 3].b - 1]);
			normals.push_back(m_normals[m_faces[i * 3 + 1].b - 1]);
			normals.push_back(m_normals[m_faces[i * 3 + 2].b - 1]);
		}
	}

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	if (m_coordinates.size() != 0)
	{
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void BaseMesh::init()
{

}

Mesh::Mesh()
{

}


Mesh::~Mesh()
{
	
}



void Mesh::init()
{
	m_shader.Init();

	InitVertices();
}

void Mesh::render(int state)
{
	if (state == 0)
	{
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                  // attribute
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(0);
	}
	else if (state == 1)
	{
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			1,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glUseProgram(0);
	}
}

DisMesh::DisMesh()
{
	bias = 0.0;
	scale = 0.0;
}

DisMesh::~DisMesh()
{
	
}

void DisMesh::init()
{
	if(!nortexname.empty())m_nortex = loadDDS(nortexname.c_str());
	if(!distexname.empty())m_distex = loadDDS(distexname.c_str());
	if(!paraname.empty())
	{
		fstream f(paraname.c_str());
		char line[256];
		float f1;
		f.getline(line, 255);
		if (sscanf(line, "DisplacementMap Scale: %f", &f1) == 1)
		{
			scale = f1;
		}
		f.getline(line, 255);
		if (sscanf(line, "DisplacementMap Bias : %f", &f1) == 1)
		{
			bias = f1;
		}
	}
	m_shader.Init();
	m_shader.Enable();
	m_shader.SetDisTex(0);
	m_shader.SetNormalTex(1);
	m_shader.SetBias(bias);
	m_shader.SetScale(scale);

	m_shader.Disable();
	InitVertices();
}

void DisMesh::SetNormalTextureName(std::string name)
{
	nortexname = name;
}

void DisMesh::SetDisTextureName(std::string name)
{
	distexname = name;
}

void DisMesh::SetParaName(std::string name)
{
	paraname = name;
}

void DisMesh::render()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_distex);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_nortex);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_PATCHES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glUseProgram(0);
}

