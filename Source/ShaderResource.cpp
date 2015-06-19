#include "Globals.h"
#include "ShaderResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ShaderResource::ShaderResource() :
	mEffect(nullptr)
{
}

bool ShaderResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	if (FAILED(D3DXCreateEffectFromFile(
		d3dDevice,
		mFileSource.c_str(),
		nullptr,
		nullptr,
		D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER | D3DXFX_LARGEADDRESSAWARE,
		nullptr,
		&mEffect,
		nullptr)))
	{
		return false;
	}

	return GameResource::InitResource(d3dDevice);
}

void ShaderResource::ReleaseResource()
{
	SAFE_RELEASE(mEffect);
}

ID3DXEffect* ShaderResource::GetEffect()
{
	return mEffect;
}