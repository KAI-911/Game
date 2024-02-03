#include "Window.h"

namespace application
{
	namespace waindow
	{

		Window::Window()
			: windowHandle_()
		{
		}

		void Window::CreateSimpleWindow(HINSTANCE hInstance)
		{
			// ウィンドウクラスを初期化
			WNDCLASSEX	windowClass = {};
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = WindowProc;
			windowClass.hInstance = hInstance;
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.lpszClassName = WINDOW_TITLE;
			RegisterClassEx(&windowClass);

			// ウィンドウのサイズを求める
			RECT windowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
			AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

			// ウィンドウハンドルを作成
			windowHandle_ = CreateWindow(
				WINDOW_TITLE,
				WINDOW_TITLE,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				windowRect.right - windowRect.left,
				windowRect.bottom - windowRect.top,
				nullptr,
				nullptr,
				hInstance,
				nullptr);

			// ウィンドウの表示
			ShowWindow(windowHandle_, SW_SHOW);
		}

		LRESULT Window::WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			switch(nMsg){
				case WM_DESTROY:
				// 終了
				PostQuitMessage(0);
				return 0;
			}
			// switch文が処理しなかったメッセージを処理
			return DefWindowProc(hWnd, nMsg, wParam, lParam);
		}

	}
}