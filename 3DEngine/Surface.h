#pragma once
#include "Color.h"
#include "EngineException.h"

class Surface
{
public:
	class SurfaceException : public EngineException
	{
	public:
		SurfaceException(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const std::string& GetNote() const noexcept;
	private:
		std::string note;
	};

public:
	Surface(unsigned int width, unsigned int height, unsigned int pitch);
	Surface(unsigned int width, unsigned int height);
	Surface(const Surface& other) = delete;
	Surface(Surface&& other) noexcept;
	Surface& operator=(const Surface& other) = delete;
	Surface& operator=(Surface&& other) noexcept;
	~Surface();

	void Clear(const Color& fill) noexcept;
	void Put(unsigned int x, unsigned int y, const Color& color);
	const Color& Get(unsigned int x, unsigned int y) const;
	unsigned GetWidth() const noexcept;
	unsigned GetHeight() const noexcept;

	Color* GetBuffer() noexcept;
	const Color* GetBuffer() const noexcept;
	static Surface FromFile(const std::string& name);

	void Save(const std::string& file) const;
	void Copy(const Surface& other);
private:
	Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> buffer);
private:
	std::unique_ptr<Color[]> buffer;
	unsigned int width;
	unsigned int height;
};
