#pragma once

class ShaderProgram
{
public:
	const char* m_VertexShaderString;
	const char* m_FragmentShaderString;

	GLuint m_VertexShaderHandle;
	GLuint m_FragmentShaderHandle;
	GLuint m_ProgramHandle;

public:
    ShaderProgram();
    ~ShaderProgram();

    void LoadFromDisk(const char* vertexFileName, const char* fragmentFileName);
	GLuint CreateShader(GLenum type, const char* source);
	void Cleanup();
	void UseProgram ();

	void SetMat4 (const std::string& name, const glm::mat4& value);
};