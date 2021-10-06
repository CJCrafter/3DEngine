cbuffer CBuf
{
	matrix model;
	matrix modelView;
};

struct Out
{
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float4 pos : SV_POSITION;
};

Out main(float3 pos : POSITION, float3 normal : NORMAL)
{
	Out temp;
	temp.worldPos = (float3) mul(float4(pos, 1.0f), model);
	temp.normal = mul(normal, (float3x3) model);
	temp.pos = mul(float4(pos, 1.0f), modelView);
	return temp;
}