#include "GLControl.h"

using namespace System::Threading;
using namespace System::Windows;
using namespace System::Windows::Media;
using namespace System::Windows::Media::Imaging;
using namespace System::Windows::Controls;

namespace MonoMaxGraphics
{
	int _w, _h, _fps;
	int oldW, oldH;

	void GLControl::OnRenderSizeChanged(System::Windows::SizeChangedInfo^ info)
	{
		_w = (int)info->NewSize.Width;
		_h = (int)info->NewSize.Height;

		if (!m_isInitialized)
		{
			m_graphicsEngine = new MonoMaxGraphics::GraphicsEngine();
			m_canRender = true;

			Grid^ mainGrid = gcnew Grid();
			m_fpsCounter = gcnew TextBlock();
			m_fpsCounter->Margin = Thickness(3);
			m_fpsCounter->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
			

			m_lastUpdate = System::DateTime::Now;

			m_ImageControl = gcnew Image();
			m_ImageControl->RenderTransformOrigin = Point(0.5, 0.5);
			m_ImageControl->RenderTransform = gcnew ScaleTransform(1.0, -1.0);
			
			mainGrid->Children->Add(m_ImageControl);
			mainGrid->Children->Add(m_fpsCounter);

			AddChild(mainGrid);

			System::Windows::Controls::Panel::SetZIndex(m_ImageControl, -1);

			m_cancelToken = gcnew CancellationTokenSource();
			m_renderThread = gcnew Thread(gcnew ParameterizedThreadStart(this, &GLControl::RenderThreadLoop));
			m_renderThread->IsBackground = true;
			m_renderThread->Start(m_cancelToken);

		}

		m_writeableImg = gcnew WriteableBitmap(_w, _h, 96, 96, PixelFormats::Pbgra32, nullptr);
		m_bufferPtr = (char*)m_writeableImg->BackBuffer.ToPointer();
		m_ImageControl->Source = m_writeableImg;
	}

	void GLControl::Terminate(void)
	{
		m_cancelToken->Cancel();
		m_renderThread->Abort();
		m_renderThread->Join();

		delete m_graphicsEngine;
	}

	void GLControl::ChangeState(bool status)
	{
		m_canRender = status;
	}

	void GLControl::UpdateImageData(void)
	{
		m_writeableImg->Lock();
		m_writeableImg->AddDirtyRect(Int32Rect(0, 0, _w, _h));
		m_writeableImg->Unlock();
	}

	void GLControl::RenderThreadLoop(System::Object^ token)
	{
		m_graphicsEngine->Init(true);
		m_isInitialized = true;
		m_lastUpdate = System::DateTime::Now;

		while (!m_cancelToken->IsCancellationRequested)
		{
			if (m_canRender)
			{
				if (_w != oldW || _h != oldH)
				{
					m_graphicsEngine->Resize(_w, _h);
					oldW = _w;
					oldH = _h;
				}

				m_graphicsEngine->Render(m_bufferPtr);

				//if ((System::DateTime::Now - m_lastUpdate).TotalMilliseconds >= 1000)
				//{
				//	m_ImageControl->Dispatcher->Invoke(gcnew System::Action(this, &GLControl::UpdateFps));

				//	m_lastUpdate = System::DateTime::Now;
				//	_fps = 0;
				//}

				m_ImageControl->Dispatcher->BeginInvoke(gcnew System::Action(this, &GLControl::UpdateImageData));

				//_fps++;
			}
		}
	}

	void GLControl::UpdateFps(void)
	{
		m_fpsCounter->Text = _fps.ToString();
	}

	void GLControl::OnTick(System::Object^ sender, System::EventArgs^ e)
	{
		System::TimeSpan elapsed = (System::DateTime::Now - m_lastUpdate);
		if (elapsed.TotalMilliseconds >= 1000)
		{
			m_fpsCounter->Text = "FPS= " + _fps.ToString();
			_fps = 0;
			m_lastUpdate = System::DateTime::Now;
		}

		m_graphicsEngine->Render(m_bufferPtr);
		m_ImageControl->Dispatcher->Invoke(gcnew System::Action(this, &GLControl::UpdateImageData));

		_fps++;
	}
}



