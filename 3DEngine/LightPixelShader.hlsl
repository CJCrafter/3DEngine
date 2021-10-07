cbuffer LightCBuf
{
	float3 lightPos;
}

// Light Color
static const float3 materialColor = { 0.7f, 0.7f, 0.9f };
static const float3 ambientColor = { 0.05f, 0.05f, 0.05f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float3 diffuseIntensity = 1.0f;

// Constants for determining how attenuation occurs
static const float3 attConst = 1.0f;   // Constant, m
static const float3 attLin = 0.045f;   // Linear, mx
static const float3 attQuad = 0.0075f; // Quadratic, mx^2

float4 main(float3 worldPos : POSITION, float3 normal : NORMAL) : SV_Target
{
	// Get a vector between the world position and light position
	const float3 between = lightPos - worldPos;
	const float distance = length(between);
	const float3 direction = between / distance; // Normalize

	// Attenuation controls how the light weakens as it approaches a maxiumum 
	// distance. Controlled by constants attConst, attLin, and attQuad.
	const float attenuation = 1.0f / (attConst + attLin * distance + attQuad * (distance * distance));
	const float3 diffuse = diffuseColor * diffuseIntensity * attenuation * max(0.0f, dot(direction, normal));

	// Saturate wraps all values to be [0, 1]
	return float4(saturate(diffuse + ambientColor), 1.0f);
}