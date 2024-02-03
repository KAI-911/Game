#pragma once
#include <windows.h>
#include <stdint.h>

namespace application
{
	namespace waindow
	{
		constexpr wchar_t WINDOW_TITLE[] = L"window";
		constexpr uint32_t WINDOW_WIDTH = 1280;
		constexpr uint32_t WINDOW_HEIGHT = 720;

		class Window
		{
		public:
			Window();

			/// <summary>
			/// ウィンドウの作成
			/// </summary>
			/// <param name="hInstance"></param>
			void CreateSimpleWindow(HINSTANCE hInstance);

			/// <summary>
			/// ウィンドウプロシージャ
			/// </summary>
			/// <param name="hWnd"></param>
			/// <param name="nMsg"></param>
			/// <param name="wParam"></param>
			/// <param name="lParam"></param>
			/// <returns></returns>
			static LRESULT CALLBACK WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

			HWND GetWindowHandle() const { return windowHandle_; }

		private:
			HWND windowHandle_;
		};

	}
}