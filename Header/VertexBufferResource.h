#pragma once
#include <d3d9.h>
#include "GameResource.h"

class VertexBufferResource : public GameResource
{
public:
	// ctor & dtor
	VertexBufferResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice, bool isRecreating = false);
	virtual void ReleaseResource(bool isDeviceLost = false);

	// Methods
	void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, LPWSTR source);
	inline LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return mVB; }
	void SetVertexBufferFormat(UINT vertexSize, UINT numVertices, DWORD vertexFormat);
	UINT GetVertexStride() { return mVertexSize; }
	UINT GetNumVertices() { return mNumVertices; }
	DWORD GetVertexFormat() { return mVertexFormat; }

private:
	LPDIRECT3DVERTEXBUFFER9 mVB;
	UINT mVertexSize;
	UINT mNumVertices;
	DWORD mVertexFormat;
};