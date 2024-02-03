// ���_�V�F�[�_�[����o�͂���f�[�^
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

//========================================
// ���_�V�F�[�_�[
//========================================
VS_OUTPUT VertexMain(float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = pos;
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