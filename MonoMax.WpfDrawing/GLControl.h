#pragma once
#include "GraphicFramework.h"

#pragma managed(push, off)
#include "../MonoMax.EngineCore/Core.h"
#pragma managed(pop)

namespace MonoMaxGraphics
{
	public ref class GLControl : public System::Windows::Controls::UserControl
	{
	private:
		bool mIsInitialized;
		System::Threading::Thread^ mRenderThread;
		System::Threading::CancellationTokenSource^ mCancelToken;
		System::Threading::ManualResetEvent^ mManualReset;

		GraphicFramework^ mGraphicsFramework;
		MonoMaxEngine::GraphicsEngine* mGraphicsEngine;
		System::Windows::Controls::TextBlock^ mFpsCounter;
		System::DateTime mLastUpdate;

		char* mBufferPtr;

		System::Windows::Controls::Image^ mImageControl;
		System::Windows::Media::Imaging::WriteableBitmap^ mWriteableImg;

		void OnTick(System::Object^ sender, System::EventArgs^ e);
		void UpdateImageData();
		void RenderThreadLoop(void);
		void UpdateFps(void);

	protected:

	public:
		GraphicFramework^ GetGfxFramework(void);

		void Terminate(void);

		// overrides
		void OnRenderSizeChanged(System::Windows::SizeChangedInfo^ info) override;
		void OnMouseDown(System::Windows::Input::MouseButtonEventArgs^ e) override;
		void OnMouseUp(System::Windows::Input::MouseButtonEventArgs^ e) override;
		void OnMouseMove(System::Windows::Input::MouseEventArgs^ e) override;
		void OnMouseWheel(System::Windows::Input::MouseWheelEventArgs^ e) override;
	};
}

