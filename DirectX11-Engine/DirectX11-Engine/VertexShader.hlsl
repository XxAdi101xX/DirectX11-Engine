struct VSOut
{
	float4 color : Color;
	float4 position : SV_Position;
};

cbuffer TransformationBuf
{
	matrix transform;
};

VSOut main(float4 pos : POSITION, float4 color : Color)
{
	VSOut vso;
	vso.position = mul(pos, transform);
	vso.color = color;
	return vso;
}