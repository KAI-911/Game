﻿#include "DirectX11.h"
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
			if(!InitializeConstabtBuffer()){
				return false;
			}
			if(!InitializeRasterizer()){
				return false;
			}

			InitializeProjectionMatrix();
			SetViewMatrix(math::Matrix4x4::CreateRotationXMatrix(45)*math::Matrix4x4::CreateTransformMatrix(0, 5, -5));
			SetWorldMatrix(math::Matrix4x4::CreateRotationYMatrix(0));

			return true;
		}

		bool DirectX11::InitializeDeviceAndSwapChain(HWND windowHandle)
		{
			// デバイスとスワップ・チェイン作成
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 1;        // バックバッファの数
			sd.BufferDesc.Width = application::waindow::WINDOW_WIDTH;    // バックバッファの幅
			sd.BufferDesc.Height = application::waindow::WINDOW_HEIGHT;    // バックバッファの高さ
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // フォーマット
			sd.BufferDesc.RefreshRate.Numerator = 60;    // リフレッシュレート（分母）
			sd.BufferDesc.RefreshRate.Denominator = 1;    // リフレッシュレート（分子）
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // バックバッファの使用法
			sd.OutputWindow = windowHandle;        // 関連付けるウィンドウ
			sd.SampleDesc.Count = 1;            // マルチサンプル（アンチエイリアス）の数
			sd.SampleDesc.Quality = 0;            // マルチサンプル（アンチエイリアス）のクオリティ
			sd.Windowed = TRUE;        // ウィンドウモード（TRUEがウィンドウモード）
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;        // モード自動切り替え

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
				// ハードウェアデバイスを作成
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

		bool DirectX11::InitializeConstabtBuffer()
		{
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(ConstantBuffer);
			cbDesc.Usage = D3D11_USAGE_DEFAULT;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = 0;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;
			if(FAILED(device_->CreateBuffer(&cbDesc, NULL, &constantBuffer_))){
				return false;
			}
			return true;
		}

		void DirectX11::InitializeProjectionMatrix()
		{
			math::Matrix4x4 mat = {};
			float aspect = static_cast<float>(application::waindow::WINDOW_HEIGHT) / static_cast<float>(application::waindow::WINDOW_WIDTH);
			float angle = tanf(math::AngleToRadian(VIEWING_ANGLE / 2));
			float scaleX = 1 / angle;
			float scaleY = 1 / angle / aspect;
			float scaleZ = 1 / (FAR_CLIP - NEAR_CLIP) * FAR_CLIP;
			float transZ = -NEAR_CLIP / (FAR_CLIP - NEAR_CLIP) * FAR_CLIP;
			mat[0][0] = scaleX;
			mat[1][1] = scaleY;
			mat[2][2] = scaleZ;
			mat[2][3] = 1;
			mat[3][3] = 0;
			mat[3][2] = transZ;
			SetProjectionMatrix(mat);
			context_->VSSetConstantBuffers(0, 1, &constantBuffer_);
		}

		bool DirectX11::InitializeRasterizer()
		{
			ID3D11RasterizerState* rasterizerState = nullptr;
			D3D11_RASTERIZER_DESC rasterizerDesc =
			{
				D3D11_FILL_SOLID, // ワイヤーフレーム
				D3D11_CULL_BACK,      // 裏面ポリゴンをカリングします
				false,
				0,
				0.0f,
				false,
				false,
				false,
				false,
				false
			};

			if(FAILED(device_->CreateRasterizerState(&rasterizerDesc, &rasterizerState))){
				return false;
			}

			// ラスタライザーステート設定
			context_->RSSetState(rasterizerState);
			rasterizerState->Release();

			return true;
		}

		void DirectX11::SetWorldMatrix(const math::Matrix4x4& worldMatrix)
		{
			constantBufferData_.worldMatrix = worldMatrix.GetTransposeMatrix();
			context_->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBufferData_, 0, 0);
		}

		void DirectX11::SetViewMatrix(const math::Matrix4x4& viewMatrix)
		{
			constantBufferData_.viewMatrix = viewMatrix.GetInverseMatrix().GetTransposeMatrix();
			context_->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBufferData_, 0, 0);
		}

		void DirectX11::SetProjectionMatrix(const math::Matrix4x4& projectionMatrix)
		{
			constantBufferData_.projectionMatrix = projectionMatrix.GetTransposeMatrix();
			context_->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBufferData_, 0, 0);
		}

		void DirectX11::Draw()
		{
			if(!device_ || !context_ || !renderTargetView_ || !swapChain_)return;
			context_->ClearRenderTargetView(renderTargetView_, BACK_COLOR);
			context_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			
			SetWorldMatrix(math::Matrix4x4::CreateRotationYMatrix(0.01f) * constantBufferData_.worldMatrix.GetTransposeMatrix());
			// @note 三角形の描画テスト
			{
				// 頂点情報
				shader::SimpleVertex vertices[] =
				{
					{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
					{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
					{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
					{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f, 1.0f } },

					{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
					{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
					{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
					{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f, 1.0f } },

					{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
					{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
					{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
					{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f, 1.0f } },

					{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
					{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
					{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
					{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

					{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
					{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
					{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
					{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f } },

					{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
					{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
					{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
					{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
				};

				uint16_t g_IndexList[] = {
				 0,  1,  2,     3,  2,  1,
				 4,  5,  6,     7,  6,  5,
				 8,  9, 10,    11, 10,  9,
				12, 13, 14,    15, 14, 13,
				16, 17, 18,    19, 18, 17,
				20, 21, 22,    23, 22, 21,
				};

				// 頂点バッファ仕様作成
				D3D11_BUFFER_DESC vbDesc = {};
				vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// デバイスにバインドするときの種類(頂点バッファ、インデックスバッファ、定数バッファなど)
				vbDesc.ByteWidth = sizeof(vertices);			// 作成するバッファのバイトサイズ
				vbDesc.MiscFlags = 0;							// その他のフラグ
				vbDesc.StructureByteStride = 0;					// 構造化バッファの場合、その構造体のサイズ
				vbDesc.Usage = D3D11_USAGE_DEFAULT;				// 作成するバッファの使用法
				vbDesc.CPUAccessFlags = 0;

				// 初期設定情報の作成
				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = vertices;
				initData.SysMemPitch = 0;
				initData.SysMemSlicePitch = 0;

				// 頂点バッファの作成
				ID3D11Buffer* vb;
				device_->CreateBuffer(&vbDesc, &initData, &vb);

				// 頂点バッファを描画で使えるようにセットする
				UINT stride = sizeof(shader::SimpleVertex);
				UINT offset = 0;
				context_->IASetVertexBuffers(0, 1, &vb, &stride, &offset);

				uint32_t count =36;
				D3D11_BUFFER_DESC ibDesc;
				ibDesc.ByteWidth = sizeof(uint16_t) * count;
				ibDesc.Usage = D3D11_USAGE_DEFAULT;
				ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				ibDesc.CPUAccessFlags = 0;
				ibDesc.MiscFlags = 0;
				ibDesc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA irData;
				irData.pSysMem = g_IndexList;
				irData.SysMemPitch = 0;
				irData.SysMemSlicePitch = 0;

				// 頂点バッファの作成
				ID3D11Buffer* ib;
				device_->CreateBuffer(&ibDesc, &irData, &ib);

				context_->IASetIndexBuffer(ib, DXGI_FORMAT_R16_UINT, 0);

				// プロミティブ・トポロジーをセット
				context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//描画先を設定
				context_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

				// 設定内容で描画
				context_->DrawIndexed(count, 0, 0);
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
			, constantBuffer_(nullptr)
			, constantBufferData_()
		{}

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
			if(constantBuffer_){
				constantBuffer_->Release();
				constantBuffer_ = nullptr;
			}
		}

	}
}