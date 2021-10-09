cbuffer LightCBuf
{
    float3 lightPos;

	// Light Color
    float3 ambientColor;
    float3 diffuseColor;
    float diffuseIntensity;

	// Constants for determining how attenuation occurs
    float attConst; // Constant, m
    float attLin; // Linear, mx
    float attQuad; // Quadratic, mx^2
}

cbuffer ObjectCBuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
}

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

	// Specular lighting causes a "shiny spot" to appear on shapes. It helps the
	// brain to figure out where an object is relative to a light source.
    const float3 w = normal * dot(between, normal);
    const float3 r = w * 2.0 - between;
    const float3 specular = attenuation * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
	
	// Saturate wraps all values to be [0, 1]
	return float4(saturate((diffuse + ambientColor + specular) * materialColor), 1.0f);
}