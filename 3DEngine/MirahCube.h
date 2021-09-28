#pragma once
#include <random>

#include "DrawableBase.h"

class MirahCube : public DrawableBase<MirahCube>
{
public:
	MirahCube(Graphics& graphics);
	UINT GetVertexCount() const noexcept override;
};
