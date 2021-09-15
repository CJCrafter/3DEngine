#include "Drawable.h"
#include "IndexBuffer.h"

void Drawable::Draw(Graphics& graphics) const
{
	for (auto& bind : binds)
	{
		bind->Bind(graphics);
	}
	graphics.DrawIndexed(indexBuffer->GetCount());
}


void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuffer) noexcept
{
	assert("Attempting to add index buffer a second time" && this->indexBuffer == nullptr);
	this->indexBuffer = indexBuffer.get();
	binds.push_back(std::move(indexBuffer));
}