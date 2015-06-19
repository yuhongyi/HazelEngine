#include "ParticleManager.h"
#include "Particle.h"
#include "vector"

using namespace std;


// class ParticleManager
bool ParticleManager::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	for(int i = 0; i < gParticlePoolSize; i++)
	{
		Particle* newParticle = new Particle();
		newParticle->Initialize(d3dDevice);
		mFreeParticles.push_back(newParticle);
	}

	return true;
}

void ParticleManager::Deinitialize()
{
	StopAllParticles();

	for(auto particleIter : mActiveParticles)
	{
		particleIter->Deinitialize();
		delete particleIter;
	}

	for(auto particleIter : mFreeParticles)
	{
		particleIter->Deinitialize();
		delete particleIter;
	}

	mActiveParticles.empty();
	mFreeParticles.empty();
}

void ParticleManager::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	HRESULT hr;
	UINT iPass, cPasses;

	D3DXHANDLE techniqueHandle = effect->GetTechniqueByName("RenderParticle");
	effect->SetTechnique(techniqueHandle);

	V(effect->Begin(&cPasses, 0));
	for(iPass = 0; iPass < cPasses; iPass++)
	{
		V(effect->BeginPass(iPass));

		for(auto particleIter : mActiveParticles)
		{
			particleIter->Render(d3dDevice, effect);
		}

		V(effect->EndPass());
	}
	V(effect->End());
}

void ParticleManager::Tick(float deltaTime)
{
	// Check life time of active particles
	vector<Particle*> particleLifeEnd;

	for(const auto particleIter : mActiveParticles)
	{
		if(particleIter->IsLifeTimeEnd())
		{
			particleLifeEnd.push_back(particleIter);
		}
	}

	for(auto particleIter : particleLifeEnd)
	{
		particleIter->Stop();
	}

	// Tick active particles
	for(auto particleIter : mActiveParticles)
	{
		particleIter->Tick(deltaTime);
	}
}

void ParticleManager::SpawnParticle(Cell* cellTemplate, float particleLifeTime)
{
	Particle* particleToPlay = mFreeParticles.front();
	if(particleToPlay->Play(cellTemplate, particleLifeTime))
	{
		mFreeParticles.remove(particleToPlay);
		mActiveParticles.push_back(particleToPlay);
	}
}

void ParticleManager::ReturnParticleToPool(Particle* particleToReturn)
{
	if(particleToReturn)
	{
		mActiveParticles.remove(particleToReturn);
		mFreeParticles.push_back(particleToReturn);
	}
}

void ParticleManager::StopAllParticles()
{
	// Check life time of active particles
	std::vector<Particle*> particlesToStop;

	for(const auto particleIter : mActiveParticles)
	{
		particlesToStop.push_back(particleIter);
	}

	for(auto particleIter : particlesToStop)
	{
		particleIter->Stop();
	}
}