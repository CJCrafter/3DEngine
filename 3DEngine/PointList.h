#pragma once
#include "Drawable.h"
#include "Shape.h"

class PointList : public Drawable<PointList>
{
public:
	PointList(Graphics& graphics, const Shape& shape);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
	UINT GetVertexCount() const noexcept override;
private:
	UINT points;
};