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
			// �E�B���h�E�N���X��������
			WNDCLASSEX	windowClass = {};
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW;
			windowClass.lpfnWndProc = WindowProc;
			windowClass.hInstance = hInstance;
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.lpszClassName = WINDOW_TITLE;
			RegisterClassEx(&windowClass);

			// �E�B���h�E�̃T�C�Y�����߂�
			RECT windowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
			AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

			// �E�B���h�E�n���h�����쐬
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

			// �E�B���h�E�̕\��
			ShowWindow(windowHandle_, SW_SHOW);
		}

		LRESULT Window::WindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
		{
			switch(nMsg){
				case WM_DESTROY:
				// �I��
				PostQuitMessage(0);
				return 0;
			}
			// switch�����������Ȃ��������b�Z�[�W������
			return DefWindowProc(hWnd, nMsg, wParam, lParam);
		}

	}
}