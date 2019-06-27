#pragma once

#pragma managed(push, off)
#include "../MonoMax.EngineCore/Core.h"
#pragma managed(pop)

namespace MonoMaxGraphics
{


	public ref class GraphicFramework
	{
	private:
		GraphicsEngine* mEngine;

	internal:
		void Init(GraphicsEngine* engine);

	public:
		void AddMesh(System::String^ filename);

	};
}

