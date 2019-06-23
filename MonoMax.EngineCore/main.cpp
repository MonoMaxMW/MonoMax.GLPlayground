#include <iostream>
#include <string>
#include <fstream>
#include "common.h"
#include "EngineCore.h"

void main()
{
	MonoMaxGraphics::GraphicsEngine* graphicsEngine = new MonoMaxGraphics::GraphicsEngine();
	graphicsEngine->Init();
	while (true)
	{
		graphicsEngine->Render(nullptr);
	}

}