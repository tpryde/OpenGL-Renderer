#include "stdafx.h"

GameCore::GameCore() :
	m_VBO(0),
	m_IBO(0),
	m_pShader(nullptr),	
	uniform(0.1),
	rotation(0.1)
{
}

GameCore::~GameCore()
{
    // delete the allocated VBO space.
    glDeleteBuffers( 1, &m_VBO );
    CheckForGLErrors();
}

void GameCore::OneTimeInit()
{
    GenerateBuffers();
	QuadGenerateBuffers();

	m_pShader = std::make_unique<ShaderProgram> ();
	m_pShader->LoadFromDisk("shaders/simple.vert", "shaders/simple.frag");

	glEnable (GL_DEPTH_TEST);
}

void GameCore::Tick(double TimePassed)
{
	uniform += TimePassed;
	rotation += TimePassed * 0.001;

}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    // draw to the entire window
    glViewport( 0, 0, width, height );
    CheckForGLErrors();
}

void GameCore::GenerateBuffers()
{
    // ATM this can only be called once, so we assert if it's called again with a VBO already allocated
    assert( m_VBO == 0 );
	assert( m_IBO == 0 );

    // generate a VBO
    glGenBuffers( 1, &m_VBO );

    // vertex position info for a triangle
    GLfloat VertexPositions[] = { 0.5f,  0.0f, 
								0.0f, 0.5f,
								-0.5f, 0.0f,
								0.5f, 0.0f,
								0.0f, -0.5f,
								-0.5, 0.0f };

	GLubyte VertexIndex[] = { 0, 1, 2, 3, 4, 5, 6 };

    // fill it with our triangle positions
	glGenBuffers( 1, &m_VBO ); // Generates a handle.
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );//Binds handle to target
    glBufferData( GL_ARRAY_BUFFER, sizeof(VertexPositions), VertexPositions, GL_STATIC_DRAW ); // Allocates an amount of memory equal to the size in bits as the number of Vertex points.(GL_STATIC_DRAW tells how to use the info).
    glBindBuffer( GL_ARRAY_BUFFER, 0 ); // Turns handle back to default

	glGenBuffers( 1, &m_IBO ); 
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(VertexIndex), VertexIndex, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // check for errors
    CheckForGLErrors();
}

void GameCore::OnDrawFrame()
{
    // check for errors before we start drawing
    CheckForGLErrors();

    // clear the screen to darkish blue
    glClearColor( 0.0f, 0.0f, 0.6f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QuadDraw();
	//DrawTriangleWithUniform();
	glUseProgram(m_pShader->m_ProgramHandle);

    {
	//draw vertices
		/*glPointSize( 5 );
		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
		glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IBO  );
		glDrawElements( GL_POINTS, 5, GL_UNSIGNED_BYTE, 0 );
		glDisableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );*/
    }

    // check for errors again when we're done drawing
    CheckForGLErrors();
}

void GameCore::QuadGenerateBuffers()
{
	Vertices = new VertexDef[4];
	Indices = new GLushort[6];

	Vertices[0].pos.x = -0.5f, Vertices[0].pos.y = 0.5f;
	Vertices[1].pos.x = 0.5f, Vertices[1].pos.y = 0.5f;
	Vertices[2].pos.x = -0.5f, Vertices[2].pos.y = -0.5f;
	Vertices[3].pos.x = 0.5f, Vertices[3].pos.y = -0.5f;

	for ( int i = 0; i < 4; i++)
	{
		Vertices[i].color.x = rand()%1000/1000.0f;
		Vertices[i].color.y = rand()%1000/1000.0f;
		Vertices[i].color.z = rand()%1000/1000.0f;
	}

	GLuint buffers[10];
	glGenBuffers( 1, &m_QuadVBO ); // always make check here to see if m_QuadVBO is not 0. If true make assert or error.
	glBindBuffer( GL_ARRAY_BUFFER, m_QuadVBO );
	int size = sizeof(VertexDef) * 4;
	glBufferData( GL_ARRAY_BUFFER, size, Vertices, GL_STATIC_DRAW );

	Indices[0] = 0;
	Indices[1] = 2;
	Indices[2] = 1;
	Indices[3] = 1;
	Indices[4] = 2;
	Indices[5] = 3;

	glGenBuffers( 1, &m_QuadIBO ); // always make check here to see if m_QuadIBO is not 0. if true make assert or error.
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_QuadIBO );
	size = sizeof(GLushort) * 6;
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, Indices, GL_STATIC_DRAW );

	delete[] Vertices;
	delete[] Indices;
}

void GameCore::QuadDraw()
{
	glUseProgram( m_pShader->m_ProgramHandle );

	glBindBuffer( GL_ARRAY_BUFFER, m_QuadVBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_QuadIBO );

	GLuint aPos = glGetAttribLocation(m_pShader->m_ProgramHandle, "a_Position" );
	glVertexAttribPointer( aPos, 2, GL_FLOAT, GL_FALSE, 20, (void*)offsetof(VertexDef, pos) );
	glEnableVertexAttribArray( aPos );

	GLuint aCol = glGetAttribLocation(m_pShader->m_ProgramHandle, "a_Color" );
	glVertexAttribPointer( aCol, 3, GL_FLOAT, GL_FALSE, 20, (void*)offsetof(VertexDef, color) );
	glEnableVertexAttribArray( aCol );

	GLint uOffset2 = glGetUniformLocation( m_pShader->m_ProgramHandle, "u_Uniform" );
	glUniform1f( uOffset2, uniform);
	//glUniform1f( uOffset, rotation);

	GLint uOffset = glGetUniformLocation( m_pShader->m_ProgramHandle, "u_Rotation" );
	//glUniform1f( uOffset, uniform);
	glUniform1f( uOffset, rotation);
	
	glPointSize( 5 );
	//glDrawArrays( GL_POINTS, 0, 4 );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void GameCore::DrawTriangleWithUniform()
{
	//glEnable( GL_CULL_FACE ); // by default culls back
	//glCullFace( GL_FRONT_AND_BACK ); // used to cull a desired face (Takes GL_FRONT, GL_BACK and GL_FRONT_AND_BACK)
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}