#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& graphics) noexcept
{
	GetContext(graphics)->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, nullptr);
}