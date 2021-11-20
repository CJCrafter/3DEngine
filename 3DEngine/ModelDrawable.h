#pragma once

#include "DrawableBase.h"
#include "Graphics.h"

class ModelDrawable : public DrawableBase<ModelDrawable>
{
public:
	ModelDrawable(Graphics& graphics, const char* file);
};
