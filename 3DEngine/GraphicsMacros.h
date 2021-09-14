#pragma once

// graphics exception checking/throwing macros (some with dxgi infos)
#define GFX_EXCEPT_NOINFO(hr) HRException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (HRESULT hr; FAILED(hr = (hrcall))) throw HRException(__LINE__, __FILE__, hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) HRException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if( FAILED( hr = (hrcall) ) ) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),infoManager.GetMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); {auto v = infoManager.GetMessages(); if(!v.empty()) {throw InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(hr) HRException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

// Info manager
#ifdef NDEBUG
#define INFOMAN() HRESULT hr
#define INFOMAN(gfx) HRESULT hr
#else
#define GET_INFO_MANAGER() HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager()
#define GET_INFO_MANAGER(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif