#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glad/glad.h"
#include <glm/glm.hpp>

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

		// UNIFORMS
		void setInt(const std::string& uniformName, int value);
		void setMatrix4D(const std::string& uniformName, const glm::mat4& matrix);
		void setVec3(const std::string& uniformName, const glm::vec3& vec);

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
