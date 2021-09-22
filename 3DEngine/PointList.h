#pragma once
#include "DrawableBase.h"
#include "Shape.h"

class PointList : public DrawableBase<PointList>
{
public:
	PointList(Graphics& graphics, const Shape& shape);
	UINT GetVertexCount() const noexcept override;
private:
	UINT points;
};