#pragma once
#include <DirectXMath.h>

#include "Graphics.h"
#include "Bindable.h"

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
	void Draw(Graphics& graphics) const;
	virtual void Update(float dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> indexBuffer) noexcept;
private:
	const IndexBuffer* indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};


