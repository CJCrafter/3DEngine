﻿#include "Surface.h"

#include <cassert>
#include <intsafe.h>
#include "ModifiedWindows.h"
#undef RGB(r,g,b)

Surface::SurfaceException::SurfaceException(int line, const char* file, std::string note) noexcept
	: EngineException(line, file, "SurfaceException"),
	  note(std::move(note))
{
}

const char* Surface::SurfaceException::what() const noexcept
{
	return EngineException::what();
}

const std::string& Surface::SurfaceException::GetNote() const noexcept
{
	return note;
}

Surface::Surface(const unsigned int width, const unsigned int height, const unsigned int pitch)
	: buffer(std::make_unique<Color[]>(pitch * height)),
	  width(width),
	  height(height)
{
}

Surface::Surface(const unsigned int width, const unsigned int height)
	: Surface(width, height, width)
{
}

Surface::Surface(Surface&& other) noexcept
	: buffer(std::move(other.buffer)),
      width(other.width),
      height(other.height)
{
}

Surface& Surface::operator=(Surface&& other) noexcept
{
	width = other.width;
	height = other.height;
	buffer = std::move(other.buffer);
	other.buffer = nullptr;
	return *this;
}

Surface::~Surface()
{
}

void Surface::Clear(const Color& fill) noexcept
{
	memset(buffer.get(), fill.RGB(), width * height * sizeof(Color));
}

void Surface::Put(const unsigned int x, const unsigned int y, const Color& color)
{
	assert(x < width);
	assert(y < height);
	buffer[y * width + x] = color;
}

const Color& Surface::Get(const unsigned int x, const unsigned int y) const
{
	assert(x < width);
	assert(y < height);
	return buffer[y * width + x];
}

unsigned int Surface::GetWidth() const noexcept
{
	return width;
}

unsigned int Surface::GetHeight() const noexcept
{
	return height;
}

Color* Surface::GetBuffer() noexcept
{
	return buffer.get();
}

const Color* Surface::GetBuffer() const noexcept
{
	return buffer.get();
}

void Surface::Save(const std::string& filename) const
{
	auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << filename << "]: failed to get encoder; size == 0.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		std::stringstream ss;
		ss << "Saving surface to [" << filename <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw Exception(__LINE__, __FILE__, ss.str());
	};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);


	// convert filenam to wide string (for Gdiplus)
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
	if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		std::stringstream ss;
		ss << "Saving surface to [" << filename << "]: failed to save.";
		throw SurfaceException(__LINE__, __FILE__, ss.str());
	}
}

void Surface::Copy(const Surface& src) 
{
	assert(width == src.width);
	assert(height == src.height);
	memcpy(buffer.get(), src.buffer.get(), width * height * sizeof(Color));
}

Surface::Surface(const unsigned int width, const unsigned int height, std::unique_ptr<Color[]> buffer)
	: buffer(std::move(buffer)),
	  width(width),
	  height(height)
{
}

Surface Surface::FromFile(const std::string& name)
{
	unsigned int width  = 0;
	unsigned int height = 0;
	unsigned int pitch  = 0;
	std::unique_ptr<Color[]> buffer = nullptr;

	{
		// convert filenam to wide string (for Gdiplus)
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << name << "]: failed to load.";
			throw SurfaceException(__LINE__, __FILE__, ss.str());
		}

		height = bitmap.GetHeight();
		buffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				buffer[y * pitch + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(buffer));
}
