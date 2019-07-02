#pragma once
#include "common.h"
#include "opengl.h"

namespace MonoMaxEngine
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

