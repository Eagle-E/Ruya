#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glad/glad.h"

namespace ruya
{

	class Shader
	{
	public:
		Shader();
		Shader(const char * vertexShaderPath, const char * fragmentShaderPath);
		void setShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
		void use();

		GLuint mID; // the shader id
	private:

		// HELPER FUNCTIONS
		GLuint createShader(GLenum shaderType, const std::string& shaderContent);
		GLuint createShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
	};

}



#endif // !SHADER_H
