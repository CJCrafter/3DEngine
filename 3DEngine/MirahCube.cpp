#include "MirahCube.h"

#include "Cube.h"
#include "BindableMacro.h"
#include "VertexBase.h"

MirahCube::MirahCube(Graphics& graphics)
{
	if (!isStaticInitialized)
	{
		struct Vertex : public VertexBase
		{
			Vertex(const float x, const float y, const float z)
				: VertexBase(x, y, z)
			{
			}

			explicit Vertex(const Vec3f& vector)
				: VertexBase(vector)
			{
			}

			~Vertex()
			{
			}
		};

		auto model = Cube<Vertex>().Geometry();
		AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));

		auto vertexShader = std::make_unique<VertexShader>(graphics, L"TexturedVertexShader.cso");
		auto byteCode = vertexShader->GetCode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(graphics, L"TexturePixelShader.cso"));

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
		DrawableBase<MirahCube>::Init();
	}

	AddBind(std::make_unique<TransformCBuffer<MirahCube>>(graphics, *this));
}

UINT MirahCube::GetVertexCount() const noexcept
{
	return 0u;
}
