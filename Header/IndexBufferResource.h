#pragma once
#include <d3d9.h>
#include "GameResource.h"
#include "vector"

using namespace std;

class IndexBufferResource : public GameResource
{
public:
	// ctor & dtor
	IndexBufferResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void ReleaseResource() override;

	// Methods
	void LoadResource(LPDIRECT3DDEVICE9 d3dDevice, const wstring& source);
	inline LPDIRECT3DINDEXBUFFER9 GetIndexBuffer() { return mIB; }
	void SetIndexBufferData(UINT numIndices, D3DFORMAT indexFormat, void* indexData);
	UINT GetIndexSize() { return mIndexFormat == D3DFMT_INDEX16 ? 2 : 4; }
	UINT GetNumIndices() { return mNumIndices; }
	D3DFORMAT GetIndexFormat() { return mIndexFormat; }
private:
	LPDIRECT3DINDEXBUFFER9 mIB;
	UINT mNumIndices;
	D3DFORMAT mIndexFormat;
	vector<BYTE> mIndexData;
};