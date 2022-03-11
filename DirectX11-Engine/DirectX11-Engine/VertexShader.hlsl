struct VSOut
{
	float4 color : Color;
	float4 position : SV_Position;
};

VSOut main(float4 pos : POSITION, float4 color : Color)
{
	VSOut vso;
	vso.position = pos;
	vso.color = color;
	return vso;
}