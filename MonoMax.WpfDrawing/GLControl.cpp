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

	volatile bool cancelThread;

	void GLControl::OnRenderSizeChanged(System::Windows::SizeChangedInfo^ info)
	{
		_w = (int)info->NewSize.Width;
		_h = (int)info->NewSize.Height;

		if (!mIsInitialized)
		{
			mGraphicsEngine = new MonoMaxEngine::GraphicsEngine();
			mGraphicsFramework = gcnew MonoMaxGraphics::GraphicFramework();
			mGraphicsFramework->Init(mGraphicsEngine);
			

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
			mRenderThread = gcnew Thread(gcnew ThreadStart(this, &GLControl::RenderThreadLoop));
			mRenderThread->IsBackground = true;
			mRenderThread->Start();
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
		mCancelToken->Cancel(false);
		mRenderThread->Abort();
		mRenderThread->Join();

		delete mGraphicsEngine;
		delete mGraphicsFramework;

	}

	void GLControl::UpdateImageData(void)
	{
		mWriteableImg->Lock();
		mWriteableImg->AddDirtyRect(Int32Rect(0, 0, _w, _h));
		mWriteableImg->Unlock();
	}

	void GLControl::RenderThreadLoop(void)
	{
		mGraphicsEngine->Init(true);
		mIsInitialized = true;
		mLastUpdate = System::DateTime::Now;

		while (!cancelThread)
		{
			if (_w != oldW || _h != oldH)
			{
				mGraphicsEngine->Resize(_w, _h);
				oldW = _w;
				oldH = _h;
			}

			mGraphicsEngine->Mainloop(mBufferPtr);
			mImageControl->Dispatcher->Invoke(Threading::DispatcherPriority::Send, gcnew System::Action(this, &GLControl::UpdateImageData));
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

		mGraphicsEngine->Mainloop(mBufferPtr);
		mImageControl->Dispatcher->Invoke(gcnew System::Action(this, &GLControl::UpdateImageData));

		_fps++;
	}


	void GLControl::OnMouseDown(System::Windows::Input::MouseButtonEventArgs^ e)
	{
		if (!mIsInitialized)
			return;

		int btnId = -1;
		int x, y;

		x = (int)e->GetPosition(this).X;
		y = (int)e->GetPosition(this).Y;


		if (e->ChangedButton == System::Windows::Input::MouseButton::Left)
		{
			btnId = 1;
		}
		else if (e->ChangedButton == System::Windows::Input::MouseButton::Right)
		{
			btnId = 2;
		}

		mGraphicsFramework->MouseDown(btnId, x, y);
	}

	void GLControl::OnMouseUp(System::Windows::Input::MouseButtonEventArgs^ e)
	{
		if (!mIsInitialized)
			return;

		mGraphicsFramework->MouseUp();
	}

	void GLControl::OnMouseMove(System::Windows::Input::MouseEventArgs^ e)
	{
		if (!mIsInitialized)
			return;

		int x, y;
		x = (int)e->GetPosition(this).X;
		y = (int)e->GetPosition(this).Y;

		mGraphicsFramework->MouseMove(x, y);
	}

	void GLControl::OnMouseWheel(System::Windows::Input::MouseWheelEventArgs^ e)
	{
		if (!mIsInitialized)
			return;

		int x, y;
		x = e->GetPosition(this).X;
		y = e->GetPosition(this).Y;

		int delta = 0;

		if (e->Delta < 0)
			delta = -1;
		else
			delta = 1;

		mGraphicsFramework->MouseScroll(x, y, delta);
	}
}
