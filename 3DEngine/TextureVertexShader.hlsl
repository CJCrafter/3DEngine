cbuffer CBuf
{
	matrix transform;
}

struct Out
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};

Out main(float3 pos : POSITION, float2 tex : TEXCOORD)
{
	Out temp;
	temp.pos = mul(float4(pos, 1.0f), transform);
	temp.tex = tex;
	return temp;
}