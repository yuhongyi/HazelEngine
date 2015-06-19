#pragma once
#include <list>
using namespace std; 
#include "GameObject.h"
#include "Singleton.h"
#include "Particle.h"

class ParticleManager : public Singleton<ParticleManager>, public GameObject
{
public:
	// ctor & dtor
	ParticleManager() {}

	// GameObject
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice) override;
	virtual void Deinitialize() override;
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect) override;
	virtual void Tick(float deltaTime) override;

	// Methods
	void SpawnParticle(Cell* cellTemplate, float particleLifeTime);
	void ReturnParticleToPool(Particle* particleToReturn);
	void StopAllParticles();
private:
	list<Particle*> mActiveParticles;
	list<Particle*> mFreeParticles;
};