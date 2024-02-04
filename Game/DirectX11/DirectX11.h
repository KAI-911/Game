#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "../Shader/Shader.h"
#include "../Math/Matrix4x4.h"

namespace application
{
	namespace directX11
	{

		struct ConstantBuffer
		{
			math::Matrix4x4 worldMatrix;
			math::Matrix4x4 viewMatrix;
			math::Matrix4x4 projectionMatrix;
		};

		constexpr float VIEWING_ANGLE = 45.0f;
		constexpr float NEAR_CLIP = 0.1f;
		constexpr float FAR_CLIP = 1000.0f;

		class DirectX11
		{
		public:
			bool Initialize(HWND windowHandle);

		private:
			bool InitializeDeviceAndSwapChain(HWND windowHandle);
			bool InitializeBackBuff();
			bool InitializeDepthStencilView();
			bool InitializeShader();
			void InitializeViewPort();
			bool InitializeConstabtBuffer();
			void InitializeProjectionMatrix();
			bool InitializeRasterizer();

			void SetWorldMatrix(const math::Matrix4x4& worldMatrix);
			void SetViewMatrix(const math::Matrix4x4& viewMatrix);
			void SetProjectionMatrix(const math::Matrix4x4& projectionMatrix);

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
			//ビューポート
			D3D11_VIEWPORT viewPort_;
			//シェーダー
			shader::Shader shader_;
			//定数バッファ
			ID3D11Buffer* constantBuffer_;
			//定数バッファの設定情報
			ConstantBuffer constantBufferData_;

		public://シングルトン
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