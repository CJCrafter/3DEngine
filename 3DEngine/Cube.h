#pragma once
#include <random>

#include "Drawable.h"
#include "Vec3.h"

class Cube : public Drawable
{
public:
	Cube(Graphics& graphics, std::mt19937& rand,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& b,
		std::uniform_real_distribution<float>& c,
		std::uniform_real_distribution<float>& d);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	float radius;
	Vec3f position;
	Vec3f velocity;
	Vec3f angle;
	Vec3f rotation;
};
