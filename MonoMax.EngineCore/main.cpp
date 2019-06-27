#include "Core.h"

void main()
{
	MonoMaxGraphics::GraphicsEngine* graphicsEngine = new MonoMaxGraphics::GraphicsEngine();
	graphicsEngine->Init();
	while (true)
	{
		graphicsEngine->Render(nullptr);
	}

}