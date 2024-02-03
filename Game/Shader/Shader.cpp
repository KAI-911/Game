#include "Shader.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

namespace application
{
	namespace shader
	{
		constexpr wchar_t SHADER_FILE[] = L"Shader/SpriteShader.hlsl";
		constexpr char VERTEX_SHADER_ENTRY_POINT_[] = "VertexMain";
		constexpr char PIXEL_SHADER_ENTRY_POINT_[] = "PixelMain";

		Shader::Shader()
			: vertexShader(nullptr)
			, pixelShader(nullptr)
			, inputLayout(nullptr)
		{
		}

		Shader::~Shader()
		{
			if(vertexShader){
				vertexShader->Release();
			}
			if(pixelShader){
				pixelShader->Release();
			}
			if(inputLayout){
				inputLayout->Release();
			}
		}

		bool Shader::Initialize(ID3D11Device& dsevice)
		{
			ID3DBlob* compiledVS;
			if(FAILED(D3DCompileFromFile(SHADER_FILE, nullptr, nullptr, VERTEX_SHADER_ENTRY_POINT_, "vs_5_0", 0, 0, &compiledVS, nullptr))){
				return false;
			}
			// ピクセルシェーダーを読み込み＆コンパイル
			ID3DBlob* compiledPS;
			if(FAILED(D3DCompileFromFile(SHADER_FILE, nullptr, nullptr, PIXEL_SHADER_ENTRY_POINT_, "ps_5_0", 0, 0, &compiledPS, nullptr))){
				return false;
			}

			// 頂点シェーダー作成
			if(FAILED(dsevice.CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &vertexShader))){
				return false;
			}
			// ピクセルシェーダー作成
			if(FAILED(dsevice.CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &pixelShader))){
				return false;
			}

			const uint8_t layoutSize = 2;
			D3D11_INPUT_ELEMENT_DESC layout[layoutSize] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			// 頂点インプットレイアウト作成
			if(FAILED(dsevice.CreateInputLayout(&layout[0], layoutSize, compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), &inputLayout))){
				return false;
			}
			return true;
		}
	}
}