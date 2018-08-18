#pragma once

struct vector2
{
	float x, y;
};

struct vector3
{
	float x, y, z;
};

struct VertexDef
{
	vector2 pos;
	vector3 color;
};

class GameCore
{
public:
    GLuint m_VBO;
	GLuint m_IBO;
	std::unique_ptr<ShaderProgram> m_pShader;

	GLuint m_QuadVBO;
	GLuint m_QuadIBO;

public:
    GameCore();
    virtual ~GameCore();

    virtual void OneTimeInit();
    virtual void Tick(double TimePassed);

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    void GenerateBuffers();

    virtual void OnDrawFrame();

	void QuadGenerateBuffers();
	void QuadDraw();
	void DrawTriangleWithUniform();

private:
	VertexDef* Vertices;
	GLushort* Indices;

	float uniform;
	float rotation;
	unsigned int input;
};
