cbuffer Cbuf
{
	matrix transform;
}

float4 main(float3 data : POSITION) : SV_POSITION
{
	return mul(float4(data, 1.0f), transform);
}