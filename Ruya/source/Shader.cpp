#include "Shader.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <glm/gtc/type_ptr.hpp>

// hidden namespace for helper functions that are not really related to the class
namespace
{
	/*
	* Read the contents of the given filename/path.
	* @throws std::ifstream::failure if something went wrong while trying to read the file
	*/
	std::string readFileContents(const char* fileName)
	{
		// init file object
		std::ifstream vertexShaderFile;
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // throw exception when things fail
		vertexShaderFile.open(fileName);

		// copy file contents to string
		std::string vertexShaderText(
			(std::istreambuf_iterator<char>(vertexShaderFile)),
			(std::istreambuf_iterator<char>())
		);

		vertexShaderFile.close();
		return vertexShaderText;
	}
}

ruya::Shader::Shader() : mID(0)
{
	
}

ruya::Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	setShaders(vertexShaderPath, fragmentShaderPath);
	use();
}

/*
* Creates an opengl shader program with the given vertex and fragment shaders
* @throws std::exception when something went wrong, appropriate message is passed with the exception obj
*/
void ruya::Shader::setShaders(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	try
	{
		// get contents of the vertex and fragment shaders
		std::string vertexShaderText = readFileContents(vertexShaderPath);
		std::string fragmentShaderText = readFileContents(fragmentShaderPath);

		// create the shaders and the shader program
		GLuint vertexShaderID = createShader(GL_VERTEX_SHADER, vertexShaderText);
		GLuint fragmentShaderID = createShader(GL_FRAGMENT_SHADER, fragmentShaderText);
		mID = createShaderProgram(vertexShaderID, fragmentShaderID);
		
		// delete shaders, not needed afer successful linkage
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}
	catch (std::ifstream::failure e)
	{
		std::string errorMsg = "Exception: something went wrong while opening/reading/closing the vertex or fragment shader:\n=>";
		errorMsg += e.what();
		throw std::exception(errorMsg.c_str());
	}

}

/*
* Makes the shader program current -> OpenGL will render with the shaders of current program.
*/
void ruya::Shader::use()
{
	glUseProgram(mID);
}

void ruya::Shader::setInt(const std::string& uniformName, int value)
{
	glUniform1i(glGetUniformLocation(mID, uniformName.c_str()), value);
}

void ruya::Shader::setMatrix4D(const std::string& uniformName, const glm::mat4& matrix)
{
	unsigned int loc = glGetUniformLocation(mID, uniformName.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

/*####################################################################################################################################
*
*	Helper Functions
* 
####################################################################################################################################*/

/*
* Creates a shader of the given type in the OpenGL context.
* @throws std::exception if something went wrong.
* @returns shader id in opengl context if the shader was successfully created and compiled.
*/
GLuint ruya::Shader::createShader(GLenum shaderType, const std::string& shaderContent)
{
	GLuint shaderID;
	shaderID = glCreateShader(shaderType);
	const char* temp = shaderContent.c_str();
	glShaderSource(shaderID, 1, &temp, NULL);
	glCompileShader(shaderID);

	// error checking for shader compilation
	int success;
	char textBuffer[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, textBuffer);
		
		// compose error message and throw std::exception
		std::string errorMsg = "";
		errorMsg = "[ruya::Shader::createShader()] ";
		if (shaderType == GL_VERTEX_SHADER) errorMsg += "Vertex ";
		else if (shaderType == GL_FRAGMENT_SHADER) errorMsg += "Fragment ";
		errorMsg += "shader compilation failed.\n";
		errorMsg += textBuffer;
		throw std::exception(errorMsg.c_str());
	}

	return shaderID;
}

/*
* Creates a shader program, then attaches given shaders, then links program.
* @throws std::exception is linkage failed.
* @returns the program id of the created shader program.
*/
GLuint ruya::Shader::createShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID)
{
	// link shaders into one shader program to be used for the render calls
	GLuint shaderProgramID;
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	// errror checking for program linkage
	int success;
	char textBuffer[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, textBuffer);
		std::string errorMsg = "[ruya::Shader::createShaderProgram()] Shader program linkage failed.\n";
		errorMsg += textBuffer;
		errorMsg += "\n";
		throw std::exception(errorMsg.c_str());
	}

	return shaderProgramID;
}
