#pragma once
#include <list>
using namespace std; 
#include "GameObject.h"
#include "Singleton.h"
#include "Cell.h"

class Particle : public GameObject
{
public:
	// ctor & dtor
	Particle();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);
	virtual void SetPosition(Vector2D newPosition);

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
	int mResourceId;
	VertexBufferResource mVBResource;
	D3DXMATRIX mWVPMatrix;
	virtual void OnPositionUpdated();
	virtual void SetCellTemplate(Cell* cellTemplate);
};

class ParticleManager : public Singleton<ParticleManager>, public GameObject
{
public:
	// ctor & dtor
	ParticleManager();
	~ParticleManager();

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);

	// Methods
	void SpawnParticle(Cell* cellTemplate, float particleLifeTime);
	void ReturnParticleToPool(Particle* particleToReturn);
	void StopAllParticles();
private:
	list<Particle*> mActiveParticles;
	list<Particle*> mFreeParticles;
};