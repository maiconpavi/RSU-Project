#pragma once
#ifndef SHADER_HPP
#	define SHADER_HPP
#	include <glad/glad.h>
#	include <glm/glm.hpp>
#	include <glm/gtc/matrix_transform.hpp>
#	include <string>

#	include <stdio.h>
#	include <iostream>
#	include <vector>

using namespace std;

class Shader
{
public:
	Shader(std::string ShaderFolder);
	~Shader();

	void Use();
	void UnUse();

	GLuint GetId();

	void CreateShader(const char* FileName, int type);

	void AddShader(const char* FilePath, const char* Type);

	void SetBool(const std::string& name, bool value) const;

	void SetInt(const std::string& name, int value) const;

	void SetUint(const std::string& name, unsigned int value) const;

	void SetFloat(const std::string& name, float value) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;

	void SetMat4(const std::string& name, const glm::mat4& mat) const;

private:
	void GetApplyShader();
	std::string LoadShader(GLenum shaderType, const char* fileName);
	void CreateProgram();

	void CheckProgramErrors(GLuint shader);
	void CheckShaderErrors(GLuint shader);

	GLuint m_ShaderId;
	GLuint m_ApplyVert;
	GLuint m_ApplyFrag;
	std::vector<GLuint> m_Shaders;
	bool m_Compiled = false;

	std::string m_ShaderFolder;
};

#endif
