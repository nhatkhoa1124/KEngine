#pragma once

#include <string>

namespace Win32
{
	class KENGINE_API EngineWindow
	{
	public:
		EngineWindow();
		~EngineWindow();
		void InitializeWindow(const std::wstring& title, const std::wstring& className, UINT width, UINT height);
		void UpdateWindow(HWND hWnd);
		inline HWND GetHandle() { return mHWnd; }
	private:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MessageHandler(UINT msg, WPARAM wParam, LPARAM lParam);
		void RegisterWindowClass() const;
		void OnResize(UINT width, UINT height);
		void OnKeydown();
	private:
		HWND mHWnd;
		HINSTANCE mHInstance;
		std::wstring mTitle;
		std::wstring mClassName;
		UINT mWidth;
		UINT mHeight;
		bool mMinimized;
		bool mMaximized;
	};
}