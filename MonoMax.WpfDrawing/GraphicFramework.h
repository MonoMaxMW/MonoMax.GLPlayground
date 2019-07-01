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
		void MouseUp(void);
		void MouseDown(const int button, const int xPos, const int yPos);
		void MouseMove(const int xPos, const int yPos);
		void MouseScroll(const int xPos, const int yPos, const int delta);

	};
}

