#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glad/glad.h"

namespace ruya
{

	class Shader
	{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		Shader();
		Shader(const char * vertexShaderPath, const char * fragmentShaderPath);
		
		// MANIPULATORS
		void setShaders(const char* vertexShaderPath, const char* fragmentShaderPath);
		void use();
		void setInt(const std::string& uniformName, int value);

		// GETTERS
		GLuint id() { return mID; }

	private:
		GLuint mID; // the shader id

		// HELPER FUNCTIONS
		GLuint createShader(GLenum shaderType, const std::string& shaderContent);
		GLuint createShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID);
	};

}



#endif // !SHADER_H
