#pragma once
#include "DrawableBase.h"
#include "Shape.h"

class ShapeDrawable : public DrawableBase<ShapeDrawable>
{
public:
	ShapeDrawable(Graphics& graphics, const Shape& shape);
};
