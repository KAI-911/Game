#include <windows.h>
#include "Window/Window.h"
#include "DirectX11/DirectX11.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	application::waindow::Window window;
	window.CreateSimpleWindow(hInstance);
	application::directX11::DirectX11::CreateInstance();

	application::directX11::DirectX11::GetInstance().Initialize(window.GetWindowHandle());

	// メインループ
	MSG	msg = {};
	while(true){
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
			//============================================
			// ウィンドウメッセージ処理
			//============================================
			// 終了メッセージ
			if(msg.message == WM_QUIT){
				break;
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		application::directX11::DirectX11::GetInstance().Draw();
	}

	return 0;
}