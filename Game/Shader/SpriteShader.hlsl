// 頂点シェーダーから出力するデータ
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

cbuffer ConstantBuffer
{
    float4x4 world; //ワールド変換行列
    float4x4 view; //ビュー変換行列
    float4x4 projection; //透視射影変換行列
}

//========================================
// 頂点シェーダー
//========================================
VS_OUTPUT VertexMain(float4 pos : POSITION, float4 color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.pos = mul(pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
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