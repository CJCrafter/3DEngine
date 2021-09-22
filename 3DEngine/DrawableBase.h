#pragma once
#include <DirectXMath.h>

#include "Drawable.h"
#include "IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	UINT GetVertexCount() const noexcept override
	{
		return indexBuffer->GetCount();
	}
	bool IsNotIndexed() const noexcept override
	{
		return notIndexed;
	}

	const std::vector<std::unique_ptr<Bindable>>& GetBinds() const noexcept override
	{
		return binds;
	}
	const std::vector<std::unique_ptr<Bindable>>& GetGlobalBinds() const noexcept override
	{
		return staticBinds;
	}


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

	virtual void Init()
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
	static bool notIndexed;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

template<class T>
bool DrawableBase<T>::isStaticInitialized;

template<class T>
bool DrawableBase<T>::notIndexed;