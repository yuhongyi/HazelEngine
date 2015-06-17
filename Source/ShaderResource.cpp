#include "Globals.h"
#include "ShaderResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ShaderResource::ShaderResource() :
	mEffect(nullptr)
{
	mFileSource[0] = '/0';
}

bool ShaderResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	if (FAILED(D3DXCreateEffectFromFile(
		d3dDevice,
		mFileSource,
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER | D3DXFX_LARGEADDRESSAWARE,
		nullptr,
		&mEffect,
		nullptr)))
	{
		return false;
	}

	return GameResource::InitResource(d3dDevice, isRecreating);
}

void ShaderResource::ReleaseResource(bool isDeviceLost)
{
	SAFE_RELEASE(mEffect);
}

void ShaderResource::SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
{
	if(wcslen(source) < MAX_PATH)
	{
		wcscpy(&mFileSource[0], source);
	}
}

ID3DXEffect* ShaderResource::GetEffect()
{
	return mEffect;
}