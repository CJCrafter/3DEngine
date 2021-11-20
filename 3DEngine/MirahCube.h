#pragma once

#include "DrawableBase.h"

class MirahCube : public DrawableBase<MirahCube>
{
public:
	MirahCube(Graphics& graphics, float color[3]);
};
