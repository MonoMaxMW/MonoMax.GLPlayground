#include "ShaderPrg.h"
#include <cstring>

namespace MonoMaxGraphics
{
	ShaderPrg::ShaderPrg()
	{
		mId = -1;
		mShadername = "";
		mIsCompiled = false;
		mVariables = new std::map<const char*, GLint>();
	}

	ShaderPrg::~ShaderPrg()
	{
		delete mVariables;
	}


	void ShaderPrg::Create(const char* prgName)
	{
		mIsCompiled = false;
		mId = glCreateProgram();

		if (glGetError() != GL_NO_ERROR)
			throw std::exception();

		mShadername = prgName;
	}

	void ShaderPrg::AddShader(GLenum shadertype, const std::string code)
	{
		if (shadertype != GL_VERTEX_SHADER && shadertype != GL_FRAGMENT_SHADER)
			throw std::exception();

		GLuint id = glCreateShader(shadertype);

		GLenum error = (GLenum)glGetError();

		if (error != GL_NO_ERROR)
			throw std::exception("Shader creation failed!");

		const char* shaderCode = code.c_str();
		glShaderSource(id, 1, &shaderCode, NULL);
		glCompileShader(id);
		glAttachShader(mId, id);
		glDeleteShader(id);
	}

	void ShaderPrg::AddVar(const char* variable)
	{
		if (!IsCompiled())
			throw std::exception("Please compile program before adding variables");

		if (strlen(variable) == 0)
			throw std::exception("Argument was not correct!");

		GLint location = -1;

		if (variable[0] == 'u')
			location = glGetUniformLocation(mId, variable);
		else if(variable[0] == 'a')
			location = glGetAttribLocation(mId, variable);


		if (location < 0)
			throw std::exception("Location of variable was not found!. Please check the shader");

		mVariables->insert(std::pair<const char*, GLint>(variable, location));
	}

	GLint ShaderPrg::operator[](const char* variable) const
	{
		std::map<const char*, GLint>::iterator it;

		it = mVariables->find(variable);
		GLuint loc = -1;

		if (it == mVariables->end())
			throw std::exception("Variable was not found in map!");

		loc = (*it).second;

		return loc;
	}

	bool ShaderPrg::IsCompiled(void)
	{
		return mIsCompiled;
	}

	void ShaderPrg::Compile(void)
	{
		glLinkProgram(mId);
		mIsCompiled = true;
	}

	const char* ShaderPrg::GetName()
	{
		return mShadername;
	}

	GLuint& ShaderPrg::GetId(void)
	{
		return mId;
	}

	GLuint ShaderPrg::GetVar(const char* variable) const
	{
		return (*this)[variable];
	}
}
