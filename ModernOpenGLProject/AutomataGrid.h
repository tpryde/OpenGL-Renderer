#pragma once

#include <array>

struct Quad
{
	const std::vector<glm::vec3> Vertices;

	const std::vector<unsigned int> Indices;

	static void BindAttributeDescription (GLuint vao, GLuint vbo);

	Quad () :
		Vertices { glm::vec3{  0.5f,  0.5f, 0.0f},
				   glm::vec3{  0.5f, -0.5f, 0.0f},
				   glm::vec3{ -0.5f, -0.5f, 0.0f},
				   glm::vec3{ -0.5f,  0.5f, 0.0f} },
		Indices { 0, 1, 2, 0, 2, 3 }
	{
	}
};


struct AutomataCell
{
	glm::vec3 Origin;
	glm::vec3 Color;

	static void BindAttributeDescription (GLuint vao, GLuint vbo);

};


class AutomataGrid
{
	GLuint m_vao;
	GLuint m_vertex_vbo;
	GLuint m_vertex_ebo;
	GLuint m_instance_vbo;

	std::unique_ptr<ShaderProgram> m_pShader;
	glm::mat4 m_ModelMat;
	bool m_IsModelMatStale;

	const int m_GridLength;
	std::vector<AutomataCell> m_Grid;
	const Quad m_LocalQuad;

	void CreateVertexData ();
	void CreateInstanceData ();

public:
	explicit AutomataGrid (int GridLength, const glm::mat4& ProjMat, const glm::mat4& ViewMat);

	void InitializeAutomataGridProgram (const glm::mat4& ProjMat, const glm::mat4& ViewMat) const;
	void GenerateBuffers ();

	~AutomataGrid ();

	void Resize (const glm::mat4& ProjMat) const;
	void Render (const glm::mat4* ViewMat);
};

