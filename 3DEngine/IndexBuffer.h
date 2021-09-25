#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned __int32>& indices);
	void Bind(Graphics& graphics) noexcept override;
	UINT GetCount() const noexcept;
protected:
	UINT count;
	ComPtr<ID3D11Buffer> indexBuffer;
};