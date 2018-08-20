#include "stdafx.h"
#include "AutomataGrid.h"
#include <glm/gtc/epsilon.hpp>
#include <random>

Quad::Quad (int GridLength) :
	Scale ((DEFAULT_BOUND - DEFAULT_OFFSET - DEFAULT_PADDING) / static_cast<float> (GridLength)),
	Vertices{ glm::vec3{  0.5f,  0.5f, 0.0f}  * DEFAULT_BOUND / static_cast<float> (GridLength),
			   glm::vec3{  0.5f, -0.5f, 0.0f} * DEFAULT_BOUND / static_cast<float> (GridLength),
			   glm::vec3{ -0.5f, -0.5f, 0.0f} * DEFAULT_BOUND / static_cast<float> (GridLength),
			   glm::vec3{ -0.5f,  0.5f, 0.0f} * DEFAULT_BOUND / static_cast<float> (GridLength) },
	Indices{ 0, 1, 2, 0, 2, 3 }
{
}

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
	m_LocalQuad (GridLength),
	m_ModelMat (1.0f),
	m_IsModelMatStale (false),
	m_TimePassed (0.0f)
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

void AutomataGrid::UpdateInstanceData ()
{
	glBindVertexArray (m_vao);
	glBindBuffer (GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (AutomataCell) * m_Grid.size (), m_Grid.data (), GL_DYNAMIC_DRAW);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
}

void AutomataGrid::CreateInstanceData ()
{
	std::uniform_int_distribution<int> ub (0, 1);
	std::random_device rd;

	for (int i = 0; i < m_GridLength; ++i)
	{
		for (int j = 0; j < m_GridLength; ++j)
		{ 
			const float x = m_LocalQuad.Scale * (static_cast<float> (j) - static_cast<float> (m_GridLength - 1) / 2.0f + static_cast<float> (j) * DEFAULT_OFFSET / m_GridLength);
			const float y = m_LocalQuad.Scale * (static_cast<float> (i) - static_cast<float> (m_GridLength - 1) / 2.0f + static_cast<float> (i) * DEFAULT_OFFSET / m_GridLength);

			m_Grid.push_back ({ glm::vec3{ x, y, 0.0f }, (ub(rd) ? ALIVE : DEAD) });
		}
	}

	UpdateInstanceData ();
}

void AutomataGrid::StepLife (double delta)
{
	m_TimePassed += delta;

	if (m_TimePassed < SECONDS_PER_CYCLE) return;

	m_TimePassed -= SECONDS_PER_CYCLE;

	//perform GameOfLifeCycle

	auto OldGrid = m_Grid;

	for (int i = 0; i < m_GridLength; ++i)
	{
		for (int j = 0; j < m_GridLength; ++j)
		{
			const auto GridLength = m_GridLength;
			const auto Idx = [&GridLength](auto Row, auto Col) {return Row * GridLength + Col; };

			auto& Cell = m_Grid[Idx(i,j)];

			int LiveNeighbourCount = 0;

			if (i-1 > 0          && j-1>0          && OldGrid[Idx(i-1,j-1)].Color == ALIVE) ++LiveNeighbourCount;
			if (i-1 > 0		         			   && OldGrid[Idx(i-1,j+0)].Color == ALIVE) ++LiveNeighbourCount;
			if (i-1 > 0          && j+1<GridLength && OldGrid[Idx(i-1,j+1)].Color == ALIVE) ++LiveNeighbourCount;
			if (                    j-1>0          && OldGrid[Idx(i+0,j-1)].Color == ALIVE) ++LiveNeighbourCount;
			if (                    j+1<GridLength && OldGrid[Idx(i+0,j+1)].Color == ALIVE) ++LiveNeighbourCount;
			if (i+1 < GridLength && j-1>0          && OldGrid[Idx(i+1,j-1)].Color == ALIVE) ++LiveNeighbourCount;
			if (i+1 < GridLength     			   && OldGrid[Idx(i+1,j+0)].Color == ALIVE) ++LiveNeighbourCount;
			if (i+1 < GridLength && j+1<GridLength && OldGrid[Idx(i+1,j+1)].Color == ALIVE) ++LiveNeighbourCount;


			if (Cell.Color == ALIVE)
			{
				if (LiveNeighbourCount != 2 && LiveNeighbourCount != 3) Cell.Color = DEAD;
			}
			else
			{
				if (LiveNeighbourCount == 3) Cell.Color = ALIVE;
			}
		}
	}

	UpdateInstanceData ();
}