// 頂点シェーダーから出力するデータ
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

//========================================
// 頂点シェーダー
//========================================
VS_OUTPUT VertexMain(float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = pos;
	output.color = color;
	return output;
}

//========================================
// ピクセルシェーダー
//========================================
float4 PixelMain(VS_OUTPUT input) : SV_TARGET
{
	return input.color;
}