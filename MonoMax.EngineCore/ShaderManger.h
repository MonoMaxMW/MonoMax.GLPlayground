#pragma once
#include "common.h"
#include "opengl.h"

namespace MonoMaxGraphics
{
	class ShaderManger
	{
	private:
		std::vector<ShaderPrg*>* mShaderRepository;

	public:
		ShaderManger();
		~ShaderManger();
		void AddShaderPrg(ShaderPrg* prg);
		ShaderPrg* GetShaderPrg(const char* shaderPrg);
		ShaderPrg* operator[] (const char* shaderPrg);
	};
}

