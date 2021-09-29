#include "Block.h"

#include "BindableMacro.h"
#include "Cube.h"
#include "PointSphere.h"
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
		auto model = Cube<VertexBase>().Geometry();
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
		DrawableBase<Block>::Init();
	}
	// End of static constructor

	AddBind(std::make_unique<TransformCBuffer<Block>>(graphics, *this));
}

UINT Block::GetVertexCount() const noexcept
{
	return 0u; // This shape is indexed
}
