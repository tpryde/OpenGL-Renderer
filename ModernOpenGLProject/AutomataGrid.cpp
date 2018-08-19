#include "stdafx.h"
#include "AutomataGrid.h"
#include <glm/gtc/epsilon.hpp>
#include <random>

void Quad::BindAttributeDescription (GLuint vao, GLuint vbo)
{
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray (0);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, sizeof(glm ::vec3)
		, reinterpret_cast<const void*> (0));

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}

void AutomataCell::BindAttributeDescription (GLuint vao, GLuint vbo)
{
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray (1);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, sizeof (AutomataCell)
		, reinterpret_cast<const void*> (offsetof (AutomataCell, Origin)));
	glVertexAttribDivisor (1, 1);

	glEnableVertexAttribArray (2);
	glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, sizeof (AutomataCell)
		, reinterpret_cast<const void*> (offsetof (AutomataCell, Color)));
	glVertexAttribDivisor (2, 1);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}

AutomataGrid::AutomataGrid (int GridLength, const glm::mat4& ProjMat, const glm::mat4& ViewMat) :
	m_pShader (std::make_unique<ShaderProgram> ()),
	m_GridLength (GridLength),
	m_LocalQuad {},
	m_ModelMat (1.0f),
	m_IsModelMatStale (false)
{
	InitializeAutomataGridProgram (ProjMat, ViewMat);

	GenerateBuffers ();

	CreateVertexData ();
	CreateInstanceData ();
}

void AutomataGrid::InitializeAutomataGridProgram (const glm::mat4& ProjMat, const glm::mat4& ViewMat) const
{
	m_pShader->LoadFromDisk ("shaders/simple.vert", "shaders/simple.frag");
	m_pShader->UseProgram ();
	m_pShader->SetMat4 ("proj", ProjMat);
	m_pShader->SetMat4 ("view", ViewMat);
	m_pShader->SetMat4 ("model", m_ModelMat);
}

void AutomataGrid::GenerateBuffers ()
{
	glGenVertexArrays (1, &m_vao);

	glGenBuffers (1, &m_vertex_vbo);
	glGenBuffers (1, &m_vertex_ebo);
	glGenBuffers (1, &m_instance_vbo);

	Quad::BindAttributeDescription (m_vao, m_vertex_vbo);
	AutomataCell::BindAttributeDescription (m_vao, m_instance_vbo);
}

void AutomataGrid::Resize (const glm::mat4& ProjMat) const
{
	m_pShader->UseProgram ();

	m_pShader->SetMat4 ("proj", ProjMat);

	glUseProgram (0);
}


AutomataGrid::~AutomataGrid ()
{
	glDeleteBuffers (1, &m_vertex_vbo);
	glDeleteBuffers (1, &m_vertex_ebo);
	glDeleteBuffers (1, &m_instance_vbo);

	glDeleteVertexArrays (1, &m_vao);
}

void AutomataGrid::Render (const glm::mat4* ViewMat)
{
	m_pShader->UseProgram ();

	if (ViewMat) m_pShader->SetMat4 ("view", *ViewMat);

	if (m_IsModelMatStale) m_pShader->SetMat4 ("model", m_ModelMat);

	glBindVertexArray (m_vao);
	
	glDrawElementsInstanced (GL_TRIANGLES, m_LocalQuad.Indices.size (), GL_UNSIGNED_INT, nullptr, m_Grid.size ());

	glBindVertexArray (0);

	glUseProgram (0);
}

void AutomataGrid::CreateVertexData ()
{
	glBindVertexArray (m_vao);
	glBindBuffer (GL_ARRAY_BUFFER, m_vertex_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (glm::vec3) * m_LocalQuad.Vertices.size (), m_LocalQuad.Vertices.data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_vertex_ebo);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (unsigned int) * m_LocalQuad.Indices.size (), m_LocalQuad.Indices.data (), GL_STATIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}


void AutomataGrid::CreateInstanceData ()
{
	for (int i = 0; i < m_GridLength; ++i)
	{
		for (int j = 0; j < m_GridLength; ++j)
		{ 
			const float x = static_cast<float> (i) - static_cast<float> (m_GridLength - 1) / 2.0f + i * 0.1f;
			const float y = static_cast<float> (j) - static_cast<float> (m_GridLength - 1) / 2.0f + j * 0.1f;

			m_Grid.push_back ({ glm::vec3{ x, y, 0.0f }, glm::vec3{ 1.0f, 1.0f, 1.0f } });
		}
	}

	glBindVertexArray (m_vao);
	glBindBuffer (GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (AutomataCell) * m_Grid.size (), m_Grid.data (), GL_DYNAMIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}