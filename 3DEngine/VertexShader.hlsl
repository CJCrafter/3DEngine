struct VSOut
{
	float3 color : COLOR;
	float4 pos : SV_POSITION;
};

cbuffer Cbuf
{
	matrix transform;
}

VSOut main(float2 pos : POSITION, float3 color : COLOR)
{
	VSOut temp;
	temp.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	temp.color = color;
	return temp;
}