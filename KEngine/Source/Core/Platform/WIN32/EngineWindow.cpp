#include "KEngine.h"

namespace Win32
{
	EngineWindow::EngineWindow() :
		mHWnd{ nullptr },
		mHInstance{ HInstance() },
		mWidth{ 0 },
		mHeight{ 0 },
		mMinimized{ false },
		mMaximized{ false }
	{

	}

	EngineWindow::~EngineWindow()
	{
		if (mHWnd)
		{
			DestroyWindow(mHWnd);
		}
	}

	void EngineWindow::InitializeWindow(const std::wstring& title, const std::wstring& className, UINT width, UINT height)
	{
		mTitle = title;
		mClassName = className;
		mWidth = width;
		mHeight = height;

		RegisterWindowClass();

		RECT rect = { 0, 0, static_cast<LONG>(mWidth), static_cast<LONG>(mHeight) };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;

		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int posX = (screenWidth - windowWidth) / 2;
		int posY = (screenHeight - windowHeight) / 2;

		mHWnd = CreateWindowEx
		(
			0,
			mClassName.c_str(),
			mTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			posX,
			posY,
			windowWidth,
			windowHeight,
			nullptr,
			nullptr,
			HInstance(),
			this
		);

		if (!mHWnd)
		{
			DWORD error = GetLastError();
			return;
		}

		ShowWindow(mHWnd, SW_SHOW);
		::UpdateWindow(mHWnd);
	}

	void EngineWindow::RenderWindow()
	{

	}

	LRESULT CALLBACK EngineWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		EngineWindow* pEngineWindow = nullptr;
		if (msg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pEngineWindow = reinterpret_cast<EngineWindow*>(pCreate->lpCreateParams);

			::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pEngineWindow));
		}
		else
		{
			pEngineWindow = reinterpret_cast<EngineWindow*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		if (pEngineWindow)
		{
			pEngineWindow->MessageHandler(msg, wParam, lParam);
		}

		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	LRESULT EngineWindow::MessageHandler(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			OnResize(width, height);
			return 0;
		}
		case WM_KEYDOWN:
			OnKeydown();
			break;
		case WM_PAINT:
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONDOWN:
			break;
		default:
			break;
		}

		return DefWindowProcW(mHWnd, msg, wParam, lParam);
	}

	void EngineWindow::RegisterWindowClass() const
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = mHInstance;
		wc.lpszClassName = mClassName.c_str();
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;

		RegisterClassEx(&wc);
	}

	void EngineWindow::OnResize(UINT width, UINT height)
	{
		if (width == 0 || height == 0)
		{
			return;
		}
		UINT adjustedWidth = width;
		UINT adjustedHeight = height;
		//djustWindowRectEx();
	}

	void EngineWindow::OnKeydown()
	{

	}

}