#include "stdafx.h"
#include "ShaderProgram.h"
#include <glm\gtc\type_ptr.hpp>

ShaderProgram::ShaderProgram()
{
	m_VertexShaderString = 0;
	m_FragmentShaderString = 0;

	m_VertexShaderHandle = 0;
	m_FragmentShaderHandle = 0;
	m_ProgramHandle = 0;
}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::LoadFromDisk(const char* vertexFileName, const char* fragmentFileName)
{
	m_VertexShaderString = LoadCompleteFile( vertexFileName );
	m_FragmentShaderString = LoadCompleteFile( fragmentFileName );

	m_VertexShaderHandle = CreateShader( GL_VERTEX_SHADER, m_VertexShaderString );
	m_FragmentShaderHandle = CreateShader( GL_FRAGMENT_SHADER, m_FragmentShaderString );

	m_ProgramHandle = glCreateProgram();
	glAttachShader(m_ProgramHandle, m_VertexShaderHandle);
	glAttachShader(m_ProgramHandle, m_FragmentShaderHandle);
	glLinkProgram(m_ProgramHandle);
}

GLuint ShaderProgram::CreateShader(GLenum type, const char* source)
{
	GLuint handle = glCreateShader(type);

	const char* strings[] = {source};
	glShaderSource(handle, 1, strings, 0);

	glCompileShader( handle );

	int compiled;
	glGetShaderiv( handle, GL_COMPILE_STATUS, &compiled );

	if(compiled == 0)
	{
		int length;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length);

		std::vector<char> buffer (length, 0);

		glGetShaderInfoLog( handle, length, 0, buffer.data ());

		LogError(buffer.data ());
		assert(false);

	}
	return handle;
}

void ShaderProgram::Cleanup()
{

}

void ShaderProgram::UseProgram ()
{
	glUseProgram (m_ProgramHandle);
}

void ShaderProgram::SetMat4 (const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv (glGetUniformLocation (m_ProgramHandle, name.c_str ()), 1, GL_FALSE, glm::value_ptr (value));
}