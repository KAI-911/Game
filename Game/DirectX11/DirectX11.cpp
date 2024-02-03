#include "DirectX11.h"
#include "../Window/Window.h"

namespace application
{
	namespace directX11
	{
		DirectX11* DirectX11::sInstance_ = nullptr;
		constexpr float BACK_COLOR[4] = {0.0f,0.0f,0.0f,1.0f};

		bool DirectX11::Initialize(HWND windowHandle)
		{
			if(!InitializeDeviceAndSwapChain(windowHandle)){
				return false;
			}
			if(!InitializeBackBuff()){
				return false;
			}
			if(!InitializeDepthStencilView()){
				return false;
			}
			if(!InitializeShader()){
				return false;
			}
			InitializeViewPort();
			return true;
		}

		bool DirectX11::InitializeDeviceAndSwapChain(HWND windowHandle)
		{
			// �f�o�C�X�ƃX���b�v�E�`�F�C���쐬
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;        // �o�b�N�o�b�t�@�̐�
			sd.BufferDesc.Width = application::waindow::WINDOW_WIDTH;    // �o�b�N�o�b�t�@�̕�
			sd.BufferDesc.Height = application::waindow::WINDOW_HEIGHT;    // �o�b�N�o�b�t�@�̍���
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // �t�H�[�}�b�g
			sd.BufferDesc.RefreshRate.Numerator = 60;    // ���t���b�V�����[�g�i����j
			sd.BufferDesc.RefreshRate.Denominator = 1;    // ���t���b�V�����[�g�i���q�j
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // �o�b�N�o�b�t�@�̎g�p�@
			sd.OutputWindow = windowHandle;        // �֘A�t����E�B���h�E
			sd.SampleDesc.Count = 1;            // �}���`�T���v���i�A���`�G�C���A�X�j�̐�
			sd.SampleDesc.Quality = 0;            // �}���`�T���v���i�A���`�G�C���A�X�j�̃N�I���e�B
			sd.Windowed = TRUE;        // �E�B���h�E���[�h�iTRUE���E�B���h�E���[�h�j
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // ���[�h�����؂�ւ�

#if _DEBUG
			UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#else
			UINT createDeviceFlags = 0;
#endif

			const D3D_DRIVER_TYPE DriverTypes[] = {
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			D3D_FEATURE_LEVEL pLevels[] = {D3D_FEATURE_LEVEL_11_0};
			D3D_FEATURE_LEVEL level;
			HRESULT hr = 0;
			for(auto type : DriverTypes){
				// �n�[�h�E�F�A�f�o�C�X���쐬
				hr = D3D11CreateDeviceAndSwapChain(
					nullptr, type, nullptr, createDeviceFlags,
					pLevels, 1, D3D11_SDK_VERSION, &sd,
					&swapChain_, &device_, &level, &context_);
				if(SUCCEEDED(hr)){
					break;
				}
			}
			if(FAILED(hr)){
				return false;
			}

			return true;
		}

		bool DirectX11::InitializeBackBuff()
		{
			ID3D11Texture2D* pBackBuffer;
			HRESULT hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			if(FAILED(hr)){
				return false;
			}
			hr = device_->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView_);
			pBackBuffer->Release();
			if(FAILED(hr)){
				return false;
			}
			return true;
		}

		bool DirectX11::InitializeDepthStencilView()
		{
			HRESULT hr;
			D3D11_TEXTURE2D_DESC txDesc;
			ZeroMemory(&txDesc, sizeof(txDesc));
			txDesc.Width = application::waindow::WINDOW_WIDTH;
			txDesc.Height = application::waindow::WINDOW_HEIGHT;
			txDesc.MipLevels = 1;
			txDesc.ArraySize = 1;
			txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			txDesc.SampleDesc.Count = 1;
			txDesc.SampleDesc.Quality = 0;
			txDesc.Usage = D3D11_USAGE_DEFAULT;
			txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			txDesc.CPUAccessFlags = 0;
			txDesc.MiscFlags = 0;
			hr = device_->CreateTexture2D(&txDesc, NULL, &depthStencilTexture_);
			if(FAILED(hr))
				return false;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
			ZeroMemory(&dsDesc, sizeof(dsDesc));
			dsDesc.Format = txDesc.Format;
			dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsDesc.Texture2D.MipSlice = 0;
			hr = device_->CreateDepthStencilView(depthStencilTexture_, &dsDesc, &depthStencilView_);
			if(FAILED(hr)){
				return false;
			}

			context_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

			return true;
		}

		bool DirectX11::InitializeShader()
		{
			if(!shader_.Initialize(*device_)){
				return false;
			}
			context_->VSSetShader(shader_.GetVertexShader(), 0, 0);
			context_->PSSetShader(shader_.GetPixelShader(), 0, 0);
			context_->IASetInputLayout(shader_.GetInputLayout());
			return true;
		}

		void DirectX11::InitializeViewPort()
		{
			viewPort_.TopLeftX = 0;
			viewPort_.TopLeftY = 0;
			viewPort_.Width = static_cast<float>(application::waindow::WINDOW_WIDTH);
			viewPort_.Height = static_cast<float>(application::waindow::WINDOW_HEIGHT);
			viewPort_.MinDepth = 0.0f;
			viewPort_.MaxDepth = 1.0f;
			context_->RSSetViewports(1, &viewPort_);
		}

		void DirectX11::Draw()
		{
			if(!device_ || !context_ || !renderTargetView_ || !swapChain_)return;
			context_->ClearRenderTargetView(renderTargetView_, BACK_COLOR);
			context_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			
			// @note �O�p�`�̕`��e�X�g
			{
				// ���_���
				shader::SimpleVertex vertices[] =
				{
					{ {0.0f, 0.5f, 0.5f},   {1.0f, 0.0f, 0.0f, 1.0f} },
					{ {0.5f, -0.5f, 0.5f},  {0.0f, 1.0f, 0.0f, 1.0f} },
					{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
				};

				// ���_�o�b�t�@�d�l�쐬
				D3D11_BUFFER_DESC vbDesc = {};
				vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
				vbDesc.ByteWidth = sizeof(vertices);			// �쐬����o�b�t�@�̃o�C�g�T�C�Y
				vbDesc.MiscFlags = 0;							// ���̑��̃t���O
				vbDesc.StructureByteStride = 0;					// �\�����o�b�t�@�̏ꍇ�A���̍\���̂̃T�C�Y
				vbDesc.Usage = D3D11_USAGE_DEFAULT;				// �쐬����o�b�t�@�̎g�p�@
				vbDesc.CPUAccessFlags = 0;

				// �����ݒ���̍쐬
				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = vertices;
				initData.SysMemPitch = 0;
				initData.SysMemSlicePitch = 0;

				// ���_�o�b�t�@�̍쐬
				ID3D11Buffer* vb;
				device_->CreateBuffer(&vbDesc, &initData, &vb);

				// ���_�o�b�t�@��`��Ŏg����悤�ɃZ�b�g����
				UINT stride = sizeof(shader::SimpleVertex);
				UINT offset = 0;
				context_->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

				// �v���~�e�B�u�E�g�|���W�[���Z�b�g
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

				//�`����ݒ�
				context_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

				// �ݒ���e�ŕ`��
				context_->Draw(3, 0);
			}

			swapChain_->Present(0, 0);
		}

		DirectX11::DirectX11()
			: device_(nullptr)
			, context_(nullptr)
			, swapChain_(nullptr)
			, renderTargetView_(nullptr)
			, depthStencilTexture_(nullptr)
			, depthStencilView_(nullptr)
			, viewPort_()
			, shader_()
		{
		}
		DirectX11::~DirectX11()
		{
			if(device_){
				device_->Release();
				device_ = nullptr;
			}
			if(context_){
				context_->Release();
				context_ = nullptr;
			}
			if(swapChain_){
				swapChain_->Release();
				swapChain_ = nullptr;
			}
			if(renderTargetView_){
				renderTargetView_->Release();
				renderTargetView_ = nullptr;
			}
			if(depthStencilTexture_){
				depthStencilTexture_->Release();
				depthStencilTexture_ = nullptr;
			}
			if(depthStencilView_){
				depthStencilView_->Release();
				depthStencilView_ = nullptr;
			}
		}

	}
}