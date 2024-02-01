#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace application
{
	namespace directX11
	{

		class DirectX11
		{
		public:
			bool Initialize(HWND windowHandle);

		private:
			bool InitializeDeviceAndSwapChain(HWND windowHandle);
			bool InitializeBackBuff();
			bool InitializeDepthStencilView();

		public:
			void Draw();

		private:
			// Direct3Dデバイス
			ID3D11Device* device_;
			// Direct3Dデバイスコンテキスト
			ID3D11DeviceContext* context_;
			// スワップチェイン
			IDXGISwapChain* swapChain_;
			// バックバッファーのRenderTargetView
			ID3D11RenderTargetView* renderTargetView_;
			//深度ステンシル用テクスチャリソース
			ID3D11Texture2D* depthStencilTexture_;
			//深度ステンシルビュー
			ID3D11DepthStencilView* depthStencilView_;



			//シングルトン
		public:
			static void CreateInstance() { if(!sInstance_){ sInstance_ = new DirectX11(); } }
			static DirectX11& GetInstance() { return *sInstance_; }
			static void DeleteInstance() { if(sInstance_){ delete sInstance_; sInstance_ = nullptr; } }
		private:
			DirectX11();
			~DirectX11();
		private:
			static DirectX11* sInstance_;
		};

	}
}