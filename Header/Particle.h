#pragma once
#include "GameObject.h"
#include "Cell.h"

class Particle : public GameObject
{
public:
	// ctor & dtor
	Particle();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void Deinitialize() override;
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect) override;
	virtual void Tick(float deltaTime) override;
	virtual void SetPosition(Vector2D newPosition) override;

	// Methods
	virtual bool Play(Cell* cellTemplate, float lifetime);
	virtual void Stop();
	virtual bool IsLifeTimeEnd() const;
private:
	// Particle control
	bool mIsActive;
	float mLifeTime;
	float mStartTime;

	// Particle animation
	float mParticleTime;
	float mParticleFadeFactor;

	// Render related
	wstring mResourceName;
	VertexBufferResource* mVBResource;
	D3DXMATRIX mWVPMatrix;
	virtual void OnPositionUpdated();
	virtual void SetCellTemplate(Cell* cellTemplate);
};