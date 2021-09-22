#include "Block.h"

#include "BindableMacro.h"
#include "Cube.h"
#include "Sphere.h"
#include "Vec4.h"

Block::Block(Graphics& graphics, std::mt19937& rand,
           std::uniform_real_distribution<float>& a,
           std::uniform_real_distribution<float>& b,
           std::uniform_real_distribution<float>& c,
           std::uniform_real_distribution<float>& d,
           std::uniform_real_distribution<float>& e,
           std::uniform_real_distribution<float>& f)
	:
	radius(d(rand)),
	position(a(rand), a(rand), a(rand)),
	velocity(c(rand), c(rand), c(rand)),
	rotation(b(rand), b(rand), b(rand)),
	scale(e(rand), e(rand), e(rand)),
	color(f(rand), f(rand), f(rand), 0.0f)
{
	// Since every cube needs to share a few binds, we will declare them here. Static constructor, if you will.
	if (!isStaticInitialized)
	{

		auto model = Cube::Make();
		model.Transform(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.2f));
		AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));

		auto vertexShader = std::make_unique<VertexShader>(graphics, L"VertexShader.cso");
		auto byteCode = vertexShader->GetCode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(graphics, L"PixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		AddStaticBind(std::make_unique<InputLayout>(graphics, inputDesc, byteCode));
		AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		isStaticInitialized = true;
	} else
	{
		SetIndexBuffer();
	}
	// End of static constructor

	// Only non static bindables are color and transform
	struct Vec4
	{
		Vec4f face_colors[6];
	};
	Vec4 colors = 
	{
		{
			{Vec4f(1.0f, 0.0f, 1.0f, 0.0f) *= color},
			{Vec4f(1.0f, 0.0f, 0.0f, 0.0f) *= color},
			{Vec4f(0.0f, 1.0f, 0.0f, 0.0f) *= color},
			{Vec4f(0.0f, 0.0f, 1.0f, 0.0f) *= color},
			{Vec4f(1.0f, 1.0f, 0.0f, 0.0f) *= color},
			{Vec4f(0.0f, 1.0f, 1.0f, 0.0f) *= color}
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<Vec4>>(graphics, colors));
	AddBind(std::make_unique<TransformCBuffer<Block>>(graphics, *this));
}

void Block::Update(float dt) noexcept
{
	position += (velocity * dt);
	angle += (rotation * dt);
}

DirectX::XMMATRIX Block::GetTransform() const noexcept
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) *
		DirectX::XMMatrixTranslation(radius, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(position.x, position.y, position.z) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

UINT Block::GetVertexCount() const noexcept
{
	return 0u; // This shape is indexed
}
