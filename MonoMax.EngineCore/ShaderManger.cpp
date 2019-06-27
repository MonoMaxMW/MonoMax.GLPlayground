#include "ShaderPrg.h"
#include "ShaderManger.h"

namespace MonoMaxGraphics
{
	ShaderManger::ShaderManger()
	{
		mShaderRepository = new std::vector<ShaderPrg*>();
	}

	ShaderManger::~ShaderManger()
	{
		delete mShaderRepository;
	}

	void ShaderManger::AddShaderPrg(ShaderPrg* prg)
	{
		mShaderRepository->push_back(prg);
	}

	ShaderPrg* ShaderManger::GetShaderPrg(const char* shaderPrg)
	{

		if (mShaderRepository->size() == 0)
			return nullptr;

		for (unsigned i = 0; i < mShaderRepository->size(); i++)
		{
			if (mShaderRepository->at(i)->GetName() == shaderPrg)
			{
				return mShaderRepository->at(i);
			};
		}

		return nullptr;
	}

	ShaderPrg* ShaderManger::operator[](const char* shaderPrg)
	{
		return GetShaderPrg(shaderPrg);
	}
}
