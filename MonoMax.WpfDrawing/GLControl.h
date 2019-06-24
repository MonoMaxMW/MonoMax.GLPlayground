#pragma once

#pragma managed(push, off)
#include "../MonoMax.EngineCore/EngineCore.h"
#pragma managed(pop)

namespace MonoMaxGraphics
{
	public ref class GLControl : public System::Windows::Controls::UserControl
	{
	private:
		bool m_isInitialized;
		System::Threading::Thread^ m_renderThread;
		System::Threading::CancellationTokenSource^ m_cancelToken;
		System::Threading::ManualResetEvent^ m_manualReset;

		MonoMaxGraphics::GraphicsEngine* m_graphicsEngine;
		System::Windows::Controls::TextBlock^ m_fpsCounter;
		System::DateTime m_lastUpdate;

		char* m_bufferPtr;
		bool m_canRender;
		
		System::Windows::Controls::Image^ m_ImageControl;
		System::Windows::Media::Imaging::WriteableBitmap^ m_writeableImg;

		void OnTick(System::Object^ sender, System::EventArgs^ e);
		void UpdateImageData();
		void RenderThreadLoop(System::Object^ token);
		void UpdateFps(void);

	protected:

	public:
		void Terminate(void);
		void ChangeState(bool status);

		// overrides
		void OnRenderSizeChanged(System::Windows::SizeChangedInfo^ info) override;
	};
}

