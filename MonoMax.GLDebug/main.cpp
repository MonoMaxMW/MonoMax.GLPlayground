// MonoMax.GLDebug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../MonoMax.EngineCore/Core.h"

int main()
{
	MonoMaxGraphics::GraphicsEngine* engine = new MonoMaxGraphics::GraphicsEngine();
	engine->Init(false);
	engine->Resize(640, 480);
	//engine->AddMesh("C:/tmp/stl/test_1.stl");

	while (true)
	{
		engine->Render(nullptr);
	}
}
