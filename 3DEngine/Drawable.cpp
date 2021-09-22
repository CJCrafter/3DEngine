#include "Drawable.h"

void Drawable::Draw(Graphics& graphics) const
{
	// Binds are unique to each drawable. There are usually a small number
	// of binds here (compared to global binds).
	for (auto& bind : GetBinds())
	{
		bind->Bind(graphics);
	}

	// These binds are unique to the class type. For every class that inherits
	// from BindableBase, each member of THAT class will have these binds. 
	for (const auto& bind : GetGlobalBinds())
	{
		bind->Bind(graphics);
	}

	// Manually set by the user. If this is true, then an indexBuffer should
	// never have been set, since it will be ignored. Drawing without an index
	// buffer is probably a mistake, and should only be done for debugging
	if (IsNotIndexed())
	{
		graphics.Draw(GetVertexCount());
	}
	else
	{
		graphics.DrawIndexed(GetVertexCount());
	}
}

void Drawable::Tick(const float delta) noexcept
{
	angle += (rotation * delta);
	position += (velocity * delta);
}

DirectX::XMMATRIX Drawable::GetTransform() const noexcept
{
	return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(position.x, position.y, position.z) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f);
}