#pragma once
#include <d3d9.h>
#include "GameResource.h"

class IndexBufferResource : public GameResource
{
public:
	// ctor & dtor
	IndexBufferResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	virtual void ReleaseResource(bool isDeviceLost = false);

	// Methods
	void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	inline LPDIRECT3DINDEXBUFFER9 GetVertexBuffer() { return mIB; }
	UINT GetIndexSize() { return mBufferFormat == D3DFMT_INDEX16 ? 2 : 4; }
	UINT GetNumIndices() { return mBufferSize; }
	D3DFORMAT GetVertexFormat() { return mBufferFormat; }
private:
	LPDIRECT3DINDEXBUFFER9 mIB;
	UINT mBufferSize;
	D3DFORMAT mBufferFormat;
};