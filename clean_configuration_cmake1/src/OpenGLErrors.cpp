#include "OpenGLErrors.h"

void printOpenGLError(GLenum err)
{
	bool stopProgram = false;

	switch (err)
	{
	case GL_INVALID_ENUM:
		std::cout << "Error: GL_INVALID_ENUM" << std::endl;
		if (stopProgram)
		{
			system("pause");
		}
		break;
	case GL_INVALID_VALUE:
		std::cout << "Error: GL_INVALID_VALUE" << std::endl;
		if (stopProgram)
		{
			system("pause");
		}
		break;
	case GL_INVALID_OPERATION:
		std::cout << "Error: GL_INVALID_OPERATION" << std::endl;
		if (stopProgram)
		{
			system("pause");
		}
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "Error: GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
		if (stopProgram)
		{
			system("pause");
		}
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "Error: GL_OUT_OF_MEMORY" << std::endl;
		if (stopProgram)
		{
			system("pause");
		}
		break;
	case GL_NO_ERROR:
		std::cout << "No error reported" << std::endl;
		break;
	default:
		std::cout << "Unknown error" << std::endl;
		break;
	}
}