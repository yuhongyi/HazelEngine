#include "Globals.h"
#include "VertexBufferResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)

VertexBufferResource::VertexBufferResource() :
mVB(NULL)
{
	mFileSource[0] = '/0';
}

bool VertexBufferResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating)
{
	if (FAILED(d3dDevice->CreateVertexBuffer(mVertexSize * mNumVertices,
		0, mVertexFormat,
		D3DPOOL_DEFAULT, &mVB, NULL)))
	{
		wprintf(L"Failed to create vertex buffer");
		return false;
	}

	return GameResource::InitResource(d3dDevice, isRecreating);
}

void VertexBufferResource::ReleaseResource(bool isDeviceLost)
{
	SAFE_RELEASE(mVB);
}

void VertexBufferResource::LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source)
{
}

void VertexBufferResource::SetVertexBufferFormat(UINT vertexSize, UINT numVertices, DWORD vertexFormat)
{
	mVertexSize = vertexSize;
	mNumVertices = numVertices;
	mVertexFormat = vertexFormat;
}