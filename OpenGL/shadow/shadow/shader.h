#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<glm/glm.hpp>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

class Shader {
public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) {
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			
			vShaderFile.close();
			fShaderFile.close();
			
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			if (geometryPath != nullptr) {
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e) {
			std::cout << "ERROR:Failed to Read the File." << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
			

		unsigned int vertexShader, fragmentShader, geometryShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		compileCheck(vertexShader, "VERTEXSHADER");

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		compileCheck(fragmentShader, "FRAGMENTSHADER");

		if (geometryPath != nullptr) {
			const char* gShaderCode = geometryCode.c_str();
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &gShaderCode, NULL);
			glCompileShader(geometryShader);
			compileCheck(geometryShader, "GEOMETRYSHADER");
		}

		id = glCreateProgram();
		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		if (geometryPath != nullptr)
			glAttachShader(id, geometryShader);
		glLinkProgram(id);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryPath != nullptr)
			glDeleteShader(geometryShader);

	}

	void use() {
		glUseProgram(id);
	}

private:
	void compileCheck(GLuint shader, std::string type) {
		GLint success;
		GLchar inFolog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, inFolog);
				std::cout << "ERROR:SHADER_COMPILATION_ERROR of Type: " << type << "\n" << inFolog << std::endl;
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, inFolog);
				std::cout << "ERROR:PROGRAM_LINKING_ERROR of Type:" << type << "\n" << inFolog << std::endl;
			}
		}
	}
};

#endif // !SHADER_H
