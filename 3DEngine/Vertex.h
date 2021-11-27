#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Color.h"
#include <dxgiformat.h>
#include <d3d11.h>

/**
 * Describes the structure of a VertexBuffer. Specifically defines the type and
 * order of vertex data in a buffer of bytes. We can use a VertexLayout to
 * access/mutate a buffer of bytes.
 */
class VertexLayout 
{
public:
	enum ElementType {
		POS_2D, POS_3D, TEXTURE_2D, NORMAL, COLOR_3, COLOR_4, COLOR_ARGB
	};

	template<ElementType> struct Map;
	template<> struct Map<POS_2D>
	{
		using DATA_TYPE = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* SEMANTIC = "Position";
	};
	template<> struct Map<POS_3D>
	{
		using DATA_TYPE = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SEMANTIC = "Position";
	};
	template<> struct Map<TEXTURE_2D>
	{
		using DATA_TYPE = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* SEMANTIC = "Texcoord";
	};
	template<> struct Map<NORMAL>
	{
		using DATA_TYPE = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SEMANTIC = "Normal";
	};
	template<> struct Map<COLOR_3>
	{
		using DATA_TYPE = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* SEMANTIC = "Color";
	};
	template<> struct Map<COLOR_4>
	{
		using DATA_TYPE = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* SEMANTIC = "Color";
	};
	template<> struct Map<COLOR_ARGB>
	{
		using DATA_TYPE = Color;
		static constexpr DXGI_FORMAT FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;
		static constexpr const char* SEMANTIC = "Color";
	};

	class Element {
	public:
		Element(ElementType type, size_t offset)
			:
			type(type),
			offset(offset) 
		{
		}

		[[nodiscard]] ElementType GetType() const noexcept
		{
			return type;
		}

		[[nodiscard]] size_t GetOffset() const noexcept
		{
			return offset;
		}

		[[nodiscard]] size_t GetTotalOffset() const noexcept
		{
			return offset + Size();
		}

		size_t Size() const
		{
			return SizeOf(type);
		}

		static constexpr size_t SizeOf(ElementType type)
		{
			switch (type)
			{
			case POS_2D: 
				return sizeof(Map<POS_2D>::DATA_TYPE);
			case POS_3D:
				return sizeof(Map<POS_3D>::DATA_TYPE);
			case TEXTURE_2D:
				return sizeof(Map<TEXTURE_2D>::DATA_TYPE);
			case NORMAL: 
				return sizeof(Map<NORMAL>::DATA_TYPE);
			case COLOR_3:
				return sizeof(Map<COLOR_3>::DATA_TYPE);
			case COLOR_4:
				return sizeof(Map<COLOR_4>::DATA_TYPE);
			case COLOR_ARGB:
				return sizeof(Map<COLOR_ARGB>::DATA_TYPE);
			default:
				assert("Invalid element type" && false);
				return 0u;
			}
		}

		[[nodiscard]] D3D11_INPUT_ELEMENT_DESC GetDescription() const
		{
			switch (type)
			{
			case POS_2D:
				return GenerateDescription<POS_2D>(GetOffset());
			case POS_3D:
				return GenerateDescription<POS_3D>(GetOffset());
			case TEXTURE_2D:
				return GenerateDescription<TEXTURE_2D>(GetOffset());
			case NORMAL:
				return GenerateDescription<NORMAL>(GetOffset());
			case COLOR_3:
				return GenerateDescription<COLOR_3>(GetOffset());
			case COLOR_4:
				return GenerateDescription<COLOR_4>(GetOffset());
			case COLOR_ARGB:
				return GenerateDescription<COLOR_ARGB>(GetOffset());
			default:
				assert("Invalid element type" && false);
				return { "INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}
		}

	private:
		template<ElementType T>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDescription(const size_t offset)
		{
			return { Map<T>::SEMANTIC, 0, Map<T>::FORMAT, 0, static_cast<UINT>(offset), D3D11_INPUT_PER_VERTEX_DATA, 0};
		}

	private:
		ElementType type;
		size_t offset;
	};

	template<ElementType T>
	[[nodiscard]] const Element& Get() const
	{
		for (auto& data : vertexData)
		{
			if (T == data.GetType())
			{
				return data;
			}
		}
		assert("Could not resolve element type" && false);
		return vertexData.front();
	}

	[[nodiscard]] const Element& Get(size_t index) const
	{
		return vertexData[index];
	}

	VertexLayout& Append(ElementType type)
	{
		vertexData.emplace_back(type, Size());
		return *this;
	}

	size_t Size() const
	{
		return vertexData.empty() ? 0u : vertexData.back().GetTotalOffset();
	}

	size_t GetElementCount() const noexcept
	{
		return vertexData.size();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DVertexLayout() const
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> description;
		description.reserve(GetElementCount());
		for (const auto& element : vertexData)
		{
			description.push_back(element.GetDescription());
		}
		return description;
	}

private:
	std::vector<Element> vertexData;
};

/**
 * Points to a specific sub-buffer of bytes, and is capable of decoding the
 * Vertex data into a more usable layout.
 */
class Vertex
{
	friend class VertexHolder;

public:
	template<VertexLayout::ElementType T>
	auto& Attribute()
	{
		auto attribute = data + layout.Get<T>().GetOffset();
		return *reinterpret_cast<typename VertexLayout::Map<T>::SysType*>(attribute);
	}

	template<typename T>
	void SetAttribute(size_t index, T&& value)
	{
		using namespace DirectX;
		const auto& element = layout.Get(index);
		auto attribute = data + element.GetOffset();

		switch (element.GetType())
		{
		case VertexLayout::POS_2D:
			SetAttribute<VertexLayout::POS_2D>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::POS_3D:
			SetAttribute<VertexLayout::POS_3D>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::TEXTURE_2D:
			SetAttribute<VertexLayout::TEXTURE_2D>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::NORMAL:
			SetAttribute<VertexLayout::NORMAL>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::COLOR_3:
			SetAttribute<VertexLayout::COLOR_3>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::COLOR_4:
			SetAttribute<VertexLayout::COLOR_4>(attribute, std::forward<T>(value));
			break;
		case VertexLayout::COLOR_ARGB:
			SetAttribute<VertexLayout::COLOR_ARGB>(attribute, std::forward<T>(value));
			break;
		default:
			assert("Invalid element type" && false);
		}
	}

protected:
	Vertex(char* data, const VertexLayout& layout)
		:
		data(data),
		layout(layout)
	{
		assert(data != nullptr);
	}

private:
	template<typename First, typename ...Rest>
	void SetAttribute(size_t index, First&& first, Rest&&... rest)
	{
		SetAttribute(index, std::forward<First>(first));
		SetAttribute(index + 1, std::forward<Rest>(rest)...);
	}

	template<VertexLayout::ElementType T, typename SourceType>
	void SetAttribute(char* attribute, SourceType&& value)
	{
		using Destination = typename VertexLayout::Map<T>::DATA_TYPE;
		if constexpr (std::is_assignable_v<Destination, SourceType>)
		{
			*reinterpret_cast<Destination*>(attribute) = value;
		}
		else
		{
			assert("Parameter attribute type mismatch" && false);
		}
	}

private:
	char* data = nullptr;
	const VertexLayout& layout;
};

class ConstVertex
{
public:
	ConstVertex(const Vertex& vertex)
		:
		vertex(vertex)
	{
	}

	template<VertexLayout::ElementType T>
	const auto& Attribute() const
	{
		return const_cast<Vertex&>(vertex).Attribute<T>();
	}

private:
	Vertex vertex;
};

/**
 * Stores a buffer of bytes, and a VertexLayout to define how that buffer is
 * structured.
 */
class VertexHolder
{
public:
	explicit VertexHolder(VertexLayout layout)
		:
		layout(std::move(layout))
	{
	}

	[[nodiscard]] const char* GetData() const
	{
		return buffer.data();
	}

	[[nodiscard]] const VertexLayout& GetLayout() const noexcept
	{
		return layout;
	}

	[[nodiscard]] size_t SizeBytes() const
	{
		return buffer.size();
	}

	[[nodiscard]] size_t Size() const
	{
		return buffer.size() / layout.Size();
	}

	template<typename ...Params>
	void EmplaceBack(Params&&... params)
	{
		assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match vertex count");
		buffer.resize(buffer.size() + layout.Size());
		Back().SetAttribute(0u, std::forward<Params>(params)...);
	}

	Vertex Front()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(), layout };
	}

	Vertex Back()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
	}

	Vertex operator[](size_t index)
	{
		assert(index < Size());
		return Vertex{ buffer.data() + layout.Size() * index, layout };
	}

	// These are constant versions of the Front(), Back(), and [] operations
	// which will not mutate this vertex buffer. Since these functions effectively
	// act as getters, the result should not be ignored.
	[[nodiscard]] ConstVertex Front() const
	{
		return const_cast<VertexHolder*>(this)->Front();
	}

	[[nodiscard]] ConstVertex Back() const
	{
		return const_cast<VertexHolder*>(this)->Back();
	}

	[[nodiscard]] ConstVertex operator[](size_t index) const
	{
		return const_cast<VertexHolder&>(*this)[index];
	}

private:
	std::vector<char> buffer{};
	VertexLayout layout;
};