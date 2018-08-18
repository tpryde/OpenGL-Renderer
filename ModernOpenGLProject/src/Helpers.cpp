#include "stdafx.h"
#include <iostream>

void LogError(const char* string)
{
    std::clog << string << std::endl;
}

bool CheckForGLErrors()
{
	for (GLenum error = glGetError (); error != GL_NO_ERROR; error = glGetError ())
	{
		switch (error)
		{
			case GL_OUT_OF_MEMORY:
			{
				LogError("Out of Memory!");
				break;
			}
			case GL_INVALID_ENUM:
			{
				LogError("Invalid Enum!");
				break;
			}
			case GL_INVALID_OPERATION:
			{
				LogError("Invalid Operation!");
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				LogError("Invalid Frame buffer Operation!");
				break;
			}
			default:
				break;
		}
	}

	return false;
}

char* LoadCompleteFile(const char* filename)
{
    FILE* file;
    errno_t err = fopen_s( &file, filename, "rb" );

    char* buffer;

    if( file )
    {
        fseek( file, 0, SEEK_END );
        long size = ftell( file );
        rewind( file );

        buffer = new char[size+1];
        fread( buffer, size, 1, file );
        buffer[size] = 0;

        fclose( file );

        return buffer;
    }

    return nullptr;
}

double MyGetSystemTime()
{
	return std::chrono::high_resolution_clock::now ().time_since_epoch ().count ();
}
