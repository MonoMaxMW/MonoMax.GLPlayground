#pragma once

#include "common.h"
#include "math.h"
#include "opengl.h"

namespace MonoMaxEngine
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
		void AddVar(const char* variable);
		bool IsCompiled(void);
		void Compile(void);
		const char* GetName();
		GLuint& GetId(void);
		GLuint GetVar(const char* variable) const;
		GLint operator[] (const char* variable) const;
	};
}

