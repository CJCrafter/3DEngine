cbuffer LightCBuf
{
	float3 lightPos;
}

static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambient = { 0.15f, 0.15f, 0.15f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float3 diffuseIntensity = 1.0f;
static const float3 attConst = 1.0f;
static const float3 attLin = 1.0f;
static const float3 attQuad = 1.0f;

float4 main(float3 worldPos : POSITION, float3 normal : NORMAL) : SV_Target
{
	// Get a vector between the world position and light position
	const float3 between = lightPos - worldPos;
	const float distance = length(between);
	const float3 direction = between / distance; // Normalize

	const float att = attConst + attLin * distance + attQuad * (distance * distance);
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(direction, normal));

	return float4(saturate(diffuse + ambient), 1.0f);
}