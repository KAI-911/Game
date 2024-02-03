#pragma once
#include <d3d11.h>
#include <stdint.h>

namespace application
{
	namespace shader
	{

		struct SimpleVertex
		{
			float pos[3];
			float color[4];
		};

		class Shader
		{
		public:
			Shader();
			~Shader();
			bool Initialize(ID3D11Device& dsevice);

			ID3D11VertexShader* GetVertexShader()const { return vertexShader; }
			ID3D11PixelShader* GetPixelShader()const { return pixelShader; }
			ID3D11InputLayout* GetInputLayout()const { return inputLayout; }

		private:
			// ���_�V�F�[�_�[
			ID3D11VertexShader* vertexShader;
			// �s�N�Z���V�F�[�_�[
			ID3D11PixelShader* pixelShader;
			// ���̓��C�A�E�g
			ID3D11InputLayout* inputLayout;
		};

	}
}