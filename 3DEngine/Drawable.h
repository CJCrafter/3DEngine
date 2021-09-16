#pragma once
#include <DirectXMath.h>

#include "Graphics.h"
#include "Bindable.h"
#include "IndexBuffer.h"

template<class T>
class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;

	void Draw(Graphics& graphics) const
	{
		for (auto& bind : binds)
		{
			bind->Bind(graphics);
		}
		for (auto& bind : staticBinds)
		{
			bind->Bind(graphics);
		}
		graphics.DrawIndexed(indexBuffer->GetCount());
	}
	virtual void Update(float dt) noexcept = 0;


	void AddBind(std::unique_ptr<Bindable> bind)
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		binds.push_back(std::move(bind));
	}
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
	{
		assert("Attempting to add index buffer a second time" && this->indexBuffer == nullptr);
		this->indexBuffer = indexBuffer.get();
		binds.push_back(std::move(indexBuffer));
	}

	void AddStaticBind(std::unique_ptr<Bindable> bind)
	{
		assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer)
	{
		assert("Attempting to add index buffer a second time" && this->indexBuffer == nullptr);
		this->indexBuffer = indexBuffer.get();
		staticBinds.push_back(std::move(indexBuffer));
	}

protected:
	void SetIndexBuffer()
	{
		assert("We already have an index buffer set, dumbass" && indexBuffer == nullptr);
		for (auto& bind : staticBinds)
		{
			if (const auto a = dynamic_cast<IndexBuffer*>(bind.get()))
			{
				indexBuffer = a;
				return;
			}
		}
		assert("No static index buffer??" && indexBuffer != nullptr);
	}

private:
	const IndexBuffer* indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
	static std::vector<std::unique_ptr<Bindable>> staticBinds;

protected:
	static bool isStaticInitialized;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> Drawable<T>::staticBinds;

template<class T>
bool Drawable<T>::isStaticInitialized;