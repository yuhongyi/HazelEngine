#pragma once
#include <d3d9.h>
#include "GameResource.h"
#include "vector"

using namespace std;

class VertexBufferResource : public GameResource
{
public:
	// ctor & dtor
	VertexBufferResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void ReleaseResource() override;

	// Methods
	void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source);
	inline LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer() { return mVB; }
	void SetVertexBufferData(UINT vertexSize, UINT numVertices, DWORD vertexFormat, void* vertexData);
	UINT GetVertexStride() { return mVertexSize; }
	UINT GetNumVertices() { return mNumVertices; }
	DWORD GetVertexFormat() { return mVertexFormat; }

private:
	LPDIRECT3DVERTEXBUFFER9 mVB;
	UINT mVertexSize;
	UINT mNumVertices;
	DWORD mVertexFormat;
	vector<BYTE> mVertexData;
};