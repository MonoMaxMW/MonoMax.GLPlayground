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

		if (!mIsInitialized)
		{
			mGraphicsEngine = new MonoMaxGraphics::GraphicsEngine();
			mGraphicsFramework = gcnew MonoMaxGraphics::GraphicFramework();
			mGraphicsFramework->Init(mGraphicsEngine);
			
			mCanRender = true;

			Grid^ mainGrid = gcnew Grid();
			mFpsCounter = gcnew TextBlock();
			mFpsCounter->Margin = Thickness(3);
			mFpsCounter->VerticalAlignment = System::Windows::VerticalAlignment::Bottom;
			

			mLastUpdate = System::DateTime::Now;

			mImageControl = gcnew Image();
			mImageControl->RenderTransformOrigin = Point(0.5, 0.5);
			mImageControl->RenderTransform = gcnew ScaleTransform(1.0, -1.0);
			
			mainGrid->Children->Add(mImageControl);
			mainGrid->Children->Add(mFpsCounter);

			AddChild(mainGrid);

			System::Windows::Controls::Panel::SetZIndex(mImageControl, -1);

			mCancelToken = gcnew CancellationTokenSource();
			mRenderThread = gcnew Thread(gcnew ParameterizedThreadStart(this, &GLControl::RenderThreadLoop));
			mRenderThread->IsBackground = true;
			mRenderThread->Start(mCancelToken);

		}

		mWriteableImg = gcnew WriteableBitmap(_w, _h, 96, 96, PixelFormats::Pbgra32, nullptr);
		mBufferPtr = (char*)mWriteableImg->BackBuffer.ToPointer();
		mImageControl->Source = mWriteableImg;
	}

	GraphicFramework^ GLControl::GetGfxFramework(void)
	{
		return mGraphicsFramework;
	}

	void GLControl::Terminate(void)
	{
		mCancelToken->Cancel();
		mRenderThread->Abort();
		mRenderThread->Join();

		delete mGraphicsEngine;
		delete mGraphicsFramework;
	}

	void GLControl::ChangeState(bool status)
	{
		mCanRender = status;
	}

	void GLControl::UpdateImageData(void)
	{
		mWriteableImg->Lock();
		mWriteableImg->AddDirtyRect(Int32Rect(0, 0, _w, _h));
		mWriteableImg->Unlock();
	}

	void GLControl::RenderThreadLoop(System::Object^ token)
	{
		mGraphicsEngine->Init(true);
		mIsInitialized = true;
		mLastUpdate = System::DateTime::Now;

		while (!mCancelToken->IsCancellationRequested)
		{
			if (mCanRender)
			{
				if (_w != oldW || _h != oldH)
				{
					mGraphicsEngine->Resize(_w, _h);
					oldW = _w;
					oldH = _h;
				}

				mGraphicsEngine->Render(mBufferPtr);

				//if ((System::DateTime::Now - m_lastUpdate).TotalMilliseconds >= 1000)
				//{
				//	m_ImageControl->Dispatcher->Invoke(gcnew System::Action(this, &GLControl::UpdateFps));

				//	m_lastUpdate = System::DateTime::Now;
				//	_fps = 0;
				//}

				mImageControl->Dispatcher->BeginInvoke(gcnew System::Action(this, &GLControl::UpdateImageData));

				//_fps++;
			}
		}
	}

	void GLControl::UpdateFps(void)
	{
		mFpsCounter->Text = _fps.ToString();
	}

	void GLControl::OnTick(System::Object^ sender, System::EventArgs^ e)
	{
		System::TimeSpan elapsed = (System::DateTime::Now - mLastUpdate);
		if (elapsed.TotalMilliseconds >= 1000)
		{
			mFpsCounter->Text = "FPS= " + _fps.ToString();
			_fps = 0;
			mLastUpdate = System::DateTime::Now;
		}

		mGraphicsEngine->Render(mBufferPtr);
		mImageControl->Dispatcher->Invoke(gcnew System::Action(this, &GLControl::UpdateImageData));

		_fps++;
	}
}



