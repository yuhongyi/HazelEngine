#include "ParticleManager.h"
#include "ResourceManager.h"
#include "GameResource.h"
#include "ImageResource.h"

#include <vector>

// class Particle
Particle::Particle() :
	mIsActive(false),
	mLifeTime(0),
	mStartTime(0),
	mResourceId(-1)
{
}

bool Particle::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	if( FAILED( d3dDevice->CreateVertexBuffer( 12 * sizeof( CUSTOMVERTEXFORPARTICLE ),
		0, D3DFVF_CUSTOMVERTEXFORPARTICLE,
		D3DPOOL_DEFAULT, &mVB, NULL ) ) )
	{
		return false;
	}

	return true;
}

void Particle::Deinitialize()
{
	if(mVB)
	{
		mVB->Release();
	}
}

void Particle::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	if(!mIsActive)
	{
		return;
	}

	// Set fade factor
	D3DXHANDLE fadeFactorHandle = effect->GetParameterByName(NULL, "gParticleFadeFactor");
	effect->SetFloat(fadeFactorHandle, mParticleFadeFactor);

	// Set particle time
	D3DXHANDLE particleTimeHandle = effect->GetParameterByName(NULL, "gParticleTime");
	effect->SetFloat(particleTimeHandle, mParticleTime);
	D3DXHANDLE particleSpeedHandle = effect->GetParameterByName(NULL, "gParticleSpeed");
	effect->SetFloat(particleSpeedHandle, gParticleDepartSpeed);

	// Set Texture
	GameResource* d3dResource = ResourceManager::GetInstance()->GetResourceById(mResourceId);
	ImageResource* image = dynamic_cast<ImageResource*>(d3dResource);
	if(image)
	{
		D3DXHANDLE cellTextureHandle = effect->GetParameterByName(NULL, "gCellTexture");
		effect->SetTexture(cellTextureHandle, image->GetTexture());
	}

	// Set VB
	d3dDevice->SetStreamSource(0, mVB, 0, sizeof(CUSTOMVERTEXFORPARTICLE));
	d3dDevice->SetFVF(D3DFVF_CUSTOMVERTEXFORPARTICLE);

	// Draw
	effect->CommitChanges();
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
}

void Particle::Tick(float deltaTime)
{
	mParticleTime += deltaTime;
	mParticleFadeFactor = max<float>(min<float>(float(1.0 - (mParticleTime / mLifeTime)), 1.f), 0.f);
}

void Particle::UpdateVertexBuffer()
{
	D3DCOLOR cellDiffuse = 0xffffffff;
	CUSTOMVERTEXFORPARTICLE Vertices[] =
	{
		{ mPosition.X + mSize.X * 0.5f,  mPosition.Y + mSize.Y * 0.5f, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 0.5f, 0.5f}, // x, y, z, tex2
		{ mPosition.X,  mPosition.Y, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 0.f, 0.f}, 
		{ mPosition.X + mSize.X,  mPosition.Y, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 1.f, 0.f}, 
		{ mPosition.X + mSize.X * 0.5f,  mPosition.Y + mSize.Y * 0.5f, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 0.5f, 0.5f},
		{ mPosition.X + mSize.X,  mPosition.Y, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 1.f, 0.f}, 
		{ mPosition.X + mSize.X,  mPosition.Y + mSize.Y, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 1.f, 1.f}, 
		{ mPosition.X + mSize.X * 0.5f,  mPosition.Y + mSize.Y * 0.5f, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 0.5f, 0.5f},
		{ mPosition.X + mSize.X,  mPosition.Y + mSize.Y, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 1.f, 1.f}, 
		{ mPosition.X,  mPosition.Y + mSize.Y, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 0.f, 1.f}, 
		{ mPosition.X + mSize.X * 0.5f,  mPosition.Y + mSize.Y * 0.5f, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.5f, 0.5f},
		{ mPosition.X,  mPosition.Y + mSize.Y, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.f, 1.f}, 
		{ mPosition.X,  mPosition.Y, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.f, 0.f}, 
	};

	VOID* pVertices;
	if(!mVB || FAILED(mVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
	{
		return;
	}

	memcpy( pVertices, Vertices, sizeof( Vertices ) );
	mVB->Unlock();
}

void Particle::SetCellTemplate(Cell* cellTemplate)
{
	if(cellTemplate)
	{
		mResourceId = cellTemplate->GetResourceID();
		SetSize(cellTemplate->GetSize());
		SetPosition(cellTemplate->GetPosition());
		UpdateVertexBuffer();
	}
}

bool Particle::Play(Cell* cellTemplate, float lifetime)
{
	if(mIsActive || lifetime <= 0.f || !cellTemplate)
	{
		return false;
	}

	SetCellTemplate(cellTemplate);
	mLifeTime = lifetime;
	mStartTime = 0.f;
	mIsActive = true;

	return true;
}

void Particle::Stop()
{
	if(mIsActive)
	{
		mIsActive = false;
		mLifeTime = 0.f;
		ParticleManager::GetInstance()->ReturnParticleToPool(this);
	}
}

bool Particle::IsLifeTimeEnd() const
{
	if(mParticleTime >= mLifeTime)
	{
		return true;
	}

	return false;
}

// class ParticleManager
ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

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

	for(auto particleIter = mActiveParticles.begin();
		particleIter != mActiveParticles.end();
		++particleIter)
	{
		(*particleIter)->Deinitialize();
		delete *particleIter;
	}

	for(auto particleIter = mFreeParticles.begin();
		particleIter != mFreeParticles.end();
		++particleIter)
	{
		(*particleIter)->Deinitialize();
		delete *particleIter;
	}

	mActiveParticles.empty();
	mFreeParticles.empty();
}

void ParticleManager::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	HRESULT hr;
	UINT iPass, cPasses;

	// Set WVP matrix
	D3DXMATRIX matWorld, matScale, matTrans, matProj, matWVP;

	D3DXMatrixScaling(&matScale, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&matTrans, -0.5f, gGridHeight * gCellSize + 0.5f, 0.0f);
	D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

	D3DXMatrixOrthoOffCenterLH(
		&matProj,
		0.0f, (float)gGridWidth * gCellSize,
		0.0f, (float)gGridHeight * gCellSize,
		0.0f, 1.0f);

	D3DXMatrixMultiply(&matWVP, &matWorld, &matProj);

	D3DXHANDLE wvpMatrixHandle = effect->GetParameterByName(NULL, "gWorldViewProjectionMatrix");
	effect->SetMatrix(wvpMatrixHandle, &matWVP);

	D3DXHANDLE techniqueHandle = effect->GetTechniqueByName("RenderParticle");
	effect->SetTechnique(techniqueHandle);

	V(effect->Begin(&cPasses, 0));
	for(iPass = 0; iPass < cPasses; iPass++)
	{
		V(effect->BeginPass(iPass));

		for(auto particleIter = mActiveParticles.begin();
			particleIter != mActiveParticles.end();
			++particleIter)
		{
			(*particleIter)->Render(d3dDevice, effect);
		}

		V(effect->EndPass());
	}
	V(effect->End());
}

void ParticleManager::Tick(float deltaTime)
{
	// Check life time of active particles
	vector<Particle*> particleLifeEnd;

	for(auto particleIter = mActiveParticles.cbegin();
		particleIter != mActiveParticles.cend();
		++particleIter)
	{
		if((*particleIter)->IsLifeTimeEnd())
		{
			particleLifeEnd.push_back((*particleIter));
		}
	}

	for(auto particleIter = particleLifeEnd.cbegin();
		particleIter != particleLifeEnd.cend();
		++particleIter)
	{
		(*particleIter)->Stop();
	}

	// Tick active particles
	for(auto particleIter = mActiveParticles.begin();
		particleIter != mActiveParticles.end();
		++particleIter)
	{
		(*particleIter)->Tick(deltaTime);
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

	for(auto particleIter = mActiveParticles.cbegin();
		particleIter != mActiveParticles.cend();
		++particleIter)
	{
		particlesToStop.push_back((*particleIter));
	}

	for(auto particleIter = particlesToStop.cbegin();
		particleIter != particlesToStop.cend();
		++particleIter)
	{
		(*particleIter)->Stop();
	}
}