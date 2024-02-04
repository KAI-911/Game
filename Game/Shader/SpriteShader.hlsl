// ���_�V�F�[�_�[����o�͂���f�[�^
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

cbuffer ConstantBuffer
{
    float4x4 world; //���[���h�ϊ��s��
    float4x4 view; //�r���[�ϊ��s��
    float4x4 projection; //�����ˉe�ϊ��s��
}

//========================================
// ���_�V�F�[�_�[
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
// �s�N�Z���V�F�[�_�[
//========================================
float4 PixelMain(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}