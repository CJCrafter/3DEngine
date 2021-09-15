#include "TransformCbuffer.h"

TransformCBuffer::TransformCBuffer(Graphics & graphics, const Drawable& parent)
	:
	buffer(graphics),
	parent(parent)
{}

void TransformCBuffer::Bind(Graphics& graphics) noexcept
{
	buffer.Update(graphics,
		DirectX::XMMatrixTranspose(
			parent.GetTransform() * graphics.GetProjection()
		)
	);
	buffer.Bind(graphics);
}