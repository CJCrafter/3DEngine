#define NOMINMAX
#include "ModelDrawable.h"

#include "BindableMacro.h"
#include "VertexBase.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "Vertex.h"

ModelDrawable::ModelDrawable(Graphics& graphics, const char* file) 
{
	if (!isStaticInitialized) 
	{
		VertexHolder vertices(std::move(
			VertexLayout{}
				.Append(VertexLayout::POS_3D)
				.Append(VertexLayout::NORMAL)
		));
		
		Assimp::Importer assimp;
		const auto model = assimp.ReadFile(file,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
			);
		const auto mesh = model->mMeshes[0];

		// Get the vertices/normals from the mesh, and load them into our list
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
		{
			vertices.EmplaceBack(
				DirectX::XMFLOAT3{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
				*reinterpret_cast<DirectX::XMFLOAT3*>(&mesh->mNormals[i]) 
			);
		}

		// Get the indices, validate there are 3 for each face, and load them into our list
		std::vector<unsigned int> indices;
		indices.reserve(mesh->mNumFaces * 3);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) 
		{
			const auto& face = mesh->mFaces[i];
			assert( face.mNumIndices == 3 ); // peace of mind
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<VertexBuffer>(graphics, vertices));

		auto vertexShader = std::make_unique<VertexShader>(graphics, L"LightVertexShader.cso");
		auto byteCode = vertexShader->GetCode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(graphics, L"LightPixelShader.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(graphics, indices));

		AddStaticBind(std::make_unique<InputLayout>(graphics, vertices.GetLayout().GetD3DVertexLayout(), byteCode));
		AddStaticBind(std::make_unique<Topology>(graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	
		isStaticInitialized = true;
	}
	else 
	{
		DrawableBase<ModelDrawable>::Init();
	}

	struct PSMaterialCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
		float specularIntensity = 2.0f;
		float specularPower = 30.0f;
		float padding[2];
	} colorBuf{};

	colorBuf.color = { 1.0f, 0.0f, 0.0f };
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialCBuf>>(graphics, colorBuf, 1u));

	AddBind(std::make_unique<TransformCBuffer<ModelDrawable>>(graphics, *this));
}
