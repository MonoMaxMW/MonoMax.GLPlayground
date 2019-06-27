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


	}
}
