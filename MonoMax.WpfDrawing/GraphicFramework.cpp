#include "GraphicFramework.h"
#include <msclr/marshal_cppstd.h>
using namespace System::Runtime::InteropServices;

namespace MonoMaxGraphics
{
	void GraphicFramework::Init(GraphicsEngine* engine)
	{
		mEngine = engine;
	}

	void GraphicFramework::AddMesh(System::String^ filename)
	{
		if (mEngine == nullptr)
			std::exception("Graphic engine was not initialized!");

		std::string nativeStr = msclr::interop::marshal_as<std::string>(filename);

		mEngine->AddMesh(nativeStr.c_str());

	}
	void GraphicFramework::MouseUp(void)
	{
		if (mEngine != nullptr)
			mEngine->MouseUp();
	}

	void GraphicFramework::MouseDown(const int button, const int xPos, const int yPos)
	{
		if (mEngine != nullptr)
			mEngine->MouseDown(xPos, yPos, button);
	}

	void GraphicFramework::MouseMove(const int xPos, const int yPos)
	{
		if (mEngine != nullptr)
			mEngine->MouseMove(xPos, yPos);
	}

	void GraphicFramework::MouseScroll(const int xPos, const int yPos, const int delta)
	{
		if (mEngine != nullptr)
			mEngine->MouseScroll(delta);
	}
}
