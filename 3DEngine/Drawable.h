#pragma once
#include "Graphics.h"
#include "Bindable.h"
#include "Vec3.h"

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransform() const noexcept;

	void Draw(Graphics& graphics) const;
	virtual void Tick(float delta) noexcept;

protected:
	virtual UINT GetVertexCount() const noexcept = 0;
	virtual bool IsNotIndexed() const noexcept = 0;
	virtual const std::vector<std::unique_ptr<Bindable>>& GetBinds() const noexcept = 0;
	virtual const std::vector<std::unique_ptr<Bindable>>& GetGlobalBinds() const noexcept = 0;

public:
	Vec3f scale = {1.0f, 1.0f, 1.0f};
	Vec3f angle;
	Vec3f rotation;
	Vec3f position;
	Vec3f velocity;
};