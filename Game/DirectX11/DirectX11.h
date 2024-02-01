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
			// Direct3D�f�o�C�X
			ID3D11Device* device_;
			// Direct3D�f�o�C�X�R���e�L�X�g
			ID3D11DeviceContext* context_;
			// �X���b�v�`�F�C��
			IDXGISwapChain* swapChain_;
			// �o�b�N�o�b�t�@�[��RenderTargetView
			ID3D11RenderTargetView* renderTargetView_;
			//�[�x�X�e���V���p�e�N�X�`�����\�[�X
			ID3D11Texture2D* depthStencilTexture_;
			//�[�x�X�e���V���r���[
			ID3D11DepthStencilView* depthStencilView_;



			//�V���O���g��
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