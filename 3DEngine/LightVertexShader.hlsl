cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct Out
{
	float3 relativePos : POSITION;
	float3 normal : NORMAL;
	float4 pos : SV_POSITION;
};

Out main(float3 pos : POSITION, float3 normal : NORMAL)
{
	Out temp;
	temp.relativePos = (float3) mul(float4(pos, 1.0f), modelView);
	temp.normal = mul(normal, (float3x3) modelView);
	temp.pos = mul(float4(pos, 1.0f), modelViewProj);
	return temp;
}