#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(std::string ShaderFolder)
	: m_ShaderFolder(ShaderFolder)
{
	GetApplyShader();
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderId);
}

void Shader::GetApplyShader()
{
	CreateShader((const char*)(m_ShaderFolder + "/apply.vert").c_str(), GL_VERTEX_SHADER);
	CreateShader((const char*)(m_ShaderFolder + "/apply.frag").c_str(), GL_FRAGMENT_SHADER);
}

void Shader::Use()
{
	if(!m_Compiled)
	{

		CreateProgram();
		m_Compiled = true;
	}
	glUseProgram(m_ShaderId);
}

void Shader::UnUse()
{
	glUseProgram(0);
}

GLuint Shader::GetId()
{
	return m_ShaderId;
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderId, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ShaderId, name.c_str()), value);
}

void Shader::SetUint(const std::string& name, unsigned int value) const
{
	glUniform1ui(glGetUniformLocation(m_ShaderId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ShaderId, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ShaderId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ShaderId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ShaderId, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::LoadShader(GLenum shaderType, const char* fileName)
{
	std::string currentLine = "";
	std::string sourceCode("");
	std::ifstream shaderFile;

	shaderFile.open(fileName);

	if(shaderFile.is_open())
	{
		while(std::getline(shaderFile, currentLine))
			sourceCode += currentLine + "\n";
	}
	else
	{
		std::cout << "Falha ao abrir o arquivo: " << fileName
				  << "\n ************************************************** \n";
	}

	shaderFile.close();

	return sourceCode;
}

void Shader::CreateShader(const char* FileName, int type)
{
	std::cout << "Criando o shader" << FileName << '\n';
	std::string TempCode = LoadShader(type, FileName);

	const GLchar* ShaderCode = TempCode.c_str();

	GLuint temp_shader = glCreateShader(type);

	glShaderSource(temp_shader, 1, &ShaderCode, NULL);
	glCompileShader(temp_shader);
	CheckShaderErrors(temp_shader);

	m_Shaders.push_back(temp_shader);
}

void Shader::CreateProgram()
{
	m_ShaderId = glCreateProgram();
	for(GLuint shader : m_Shaders)
	{
		std::cout << shader << endl;
		glAttachShader(m_ShaderId, shader);
	}

	glLinkProgram(m_ShaderId);
	CheckProgramErrors(m_ShaderId);

	for(GLuint shader : m_Shaders)
	{
		glDeleteShader(shader);
	}
}

void Shader::CheckShaderErrors(GLuint shader)
{
	int compiled;
	char log[1024];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(compiled != GL_TRUE)
	{
		glGetShaderInfoLog(shader, 1024, NULL, log);
		std::cout << "Erro ao compilar o shader"
				  << "\n"
				  << log << "\n ************************************************** \n";
	}
}

void Shader::CheckProgramErrors(GLuint shader)
{
	int compiled;
	char log[1024];

	glGetProgramiv(shader, GL_LINK_STATUS, &compiled);
	if(!compiled)
	{
		glGetProgramInfoLog(shader, 1024, NULL, log);
		std::cout << "Erro ao vincular o shader"
				  << "\n"
				  << log << "\n ************************************************** \n";
	}
}
