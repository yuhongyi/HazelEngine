#include "stdafx.h"
#include "ShaderResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

ShaderResource::ShaderResource() :
	mEffect(NULL)
{
	mFileSource[0] = '/0';
}

bool ShaderResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	return true;
}

void ShaderResource::CleanUpResource()
{
	SAFE_RELEASE(mEffect);
}

void ShaderResource::SetSource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
{
	if(wcslen(source) < MAX_PATH)
	{
		wcscpy(&mFileSource[0], source);
	}

	D3DXCreateEffectFromFile(
		d3dDevice,
		mFileSource,
		NULL,
		NULL,
		D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER | D3DXFX_LARGEADDRESSAWARE,
		NULL,
		&mEffect,
		NULL);
}

ID3DXEffect* ShaderResource::GetEffect()
{
	return mEffect;
}