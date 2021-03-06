cbuffer Cbuf
{
	matrix model;
	matrix modelView;
}

struct Out
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

Out main(float3 data : POSITION)
{
	Out temp;
	temp.pos = mul(float4(data, 1.0f), modelView);
	temp.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return temp;
}