#pragma once
#include "Globals.h"
#include "ArchivedGameResource.h"

class ShaderResource : public ArchivedGameResource
{
public:
	// ctor & dtor
	ShaderResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void ReleaseResource() override;

	// Methods
	ID3DXEffect* GetEffect();
private:
	ID3DXEffect* mEffect;
};