#pragma once

#include "common.h"
#include "math.h"
#include "opengl.h"

namespace MonoMaxGraphics
{
	class ShaderPrg
	{
	private:
		std::map<const char*, GLint>* mVariables;
		GLuint mId;
		bool mIsCompiled;
		const char* mShadername;

	public:
		ShaderPrg();
		~ShaderPrg();
		void Create(const char*);
		void AddShader(GLenum shadertype, const std::string code);
		void AddVariable(const char* variable);
		GLint operator[] (const char* variable);
		bool IsCompiled(void);
		void Compile(void);
		const char* GetName();
		GLuint& GetId(void);
	};
}

