#include "MirahCube.h"

#include "BindableMacro.h"
#include "FacedCube.h"
#include "SkinnedCube.h"
#include "VertexBase.h"
#include "Surface.h"

MirahCube::MirahCube(Graphics& graphics)
{
	if (!isStaticInitialized)
	{
		/*
		struct Vertex : public VertexBase
		{
			struct
			{
				float u, v;
			} texture;

			Vertex()
				: Vertex(0.0f, 0.0f, 0.0f)
			{
			}

			Vertex(const float x, const float y, const float z)
				: VertexBase(x, y, z),
			      texture{0, 0}
			{
			}

			explicit Vertex(const Vec3f& vector)
				: VertexBase(vector),
			      texture{0, 0}
			{
			}

			~Vertex()
			{
			}
		};*/
		struct Vertex : public VertexBase
		{
			DirectX::XMFLOAT3 normal;

			Vertex()
				:
				Vertex(0.0f, 0.0f, 0.0f)
			{
			}

			Vertex(const float x, const float y, const float z)
				:
				VertexBase(x, y, z),
				normal{ 0, 0, 0 }
			{
			}

			explicit Vertex(const Vec3f& vector)
				:
				VertexBase(vector),
				normal{ 0, 0, 0 }
			{
			}

			~Vertex()
			{
			}
		};

		auto model = FacedCube<Vertex>().Geometry();
		model.SetNormals();

		AddStaticBind(std::make_unique<Texture>(graphics, Surface::FromFile("grass.png")));
		AddStaticBind(std::make_unique<VertexBuffer>(graphics, model.vertices));
		AddStaticBind(std::make_unique<Sampler>(graphics));
			
		auto vertexShader = std::make_unique<VertexShader>(graphics, L"LightVertexShader.cso");
		auto byteCode = vertexShader->GetCode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(graphics, L"LightPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics, model.indices));

		struct PSLightConstants
		{
			DirectX::XMVECTOR pos;
		};
		//AddStaticBind(std::make_unique<PixelConstantBuffer<PSLightConstants>>(graphics));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
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
