#include "Particle.h"
#include "ResourceManager.h"
#include "ParticleManager.h"
#include "GameResource.h"
#include "ImageResource.h"

#include <vector>

// class Particle
Particle::Particle() :
	mIsActive(false),
	mLifeTime(0),
	mStartTime(0),
	mVBResource(nullptr)
{
	mVBResource = new VertexBufferResource();
	mSize.X = (float)gCellSize;
	mSize.Y = (float)gCellSize;
}

bool Particle::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	D3DCOLOR cellDiffuse = 0xffffffff;
	CUSTOMVERTEXFORPARTICLE Vertices[] =
	{
		{ mSize.X * 0.5f, mSize.Y * 0.5f, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 0.5f, 0.5f }, // x, y, z, tex2
		{ 0.f, 0.f, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 0.f, 0.f },
		{ mSize.X, 0.f, 0.f, D3DXVECTOR3(0.f, -1.f, 0.f), cellDiffuse, 1.f, 0.f },
		{ mSize.X * 0.5f, mSize.Y * 0.5f, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 0.5f, 0.5f },
		{ mSize.X, 0.f, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 1.f, 0.f },
		{ mSize.X, mSize.Y, 0.f, D3DXVECTOR3(1.f, 0.f, 0.f), cellDiffuse, 1.f, 1.f },
		{ mSize.X * 0.5f, mSize.Y * 0.5f, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 0.5f, 0.5f },
		{ mSize.X, mSize.Y, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 1.f, 1.f },
		{ 0.f, mSize.Y, 0.f, D3DXVECTOR3(0.f, 1.f, 0.f), cellDiffuse, 0.f, 1.f },
		{ mSize.X * 0.5f, mSize.Y * 0.5f, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.5f, 0.5f },
		{ 0.f, mSize.Y, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.f, 1.f },
		{ 0.f, 0.f, 0.f, D3DXVECTOR3(-1.f, 0.f, 0.f), cellDiffuse, 0.f, 0.f },
	};

	mVBResource->SetVertexBufferData(sizeof(CUSTOMVERTEXFORPARTICLE), 12, D3DFVF_CUSTOMVERTEXFORPARTICLE, Vertices);
	mVBResource->InitResource(d3dDevice);

	return true;
}

void Particle::Deinitialize()
{
}

void Particle::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
	if(!mIsActive)
	{
		return;
	}

	// Set transform matrix
	D3DXHANDLE wvpMatrixHandle = effect->GetParameterByName(0, "gWorldViewProjectionMatrix");
	effect->SetMatrix(wvpMatrixHandle, &mWVPMatrix);

	// Set fade factor
	D3DXHANDLE fadeFactorHandle = effect->GetParameterByName(0, "gParticleFadeFactor");
	effect->SetFloat(fadeFactorHandle, mParticleFadeFactor);

	// Set particle time
	D3DXHANDLE particleTimeHandle = effect->GetParameterByName(0, "gParticleTime");
	effect->SetFloat(particleTimeHandle, mParticleTime);
	D3DXHANDLE particleSpeedHandle = effect->GetParameterByName(0, "gParticleSpeed");
	effect->SetFloat(particleSpeedHandle, gParticleDepartSpeed);

	// Set Texture
	GameResource* d3dResource = ResourceManager::GetInstance()->GetArchivedGameResource(mResourceName);
	ImageResource* image = dynamic_cast<ImageResource*>(d3dResource);
	if(image)
	{
		D3DXHANDLE cellTextureHandle = effect->GetParameterByName(0, "gCellTexture");
		effect->SetTexture(cellTextureHandle, image->GetTexture());
	}

	// Set VB
	d3dDevice->SetStreamSource(0, mVBResource->GetVertexBuffer(), 0, mVBResource->GetVertexStride());
	d3dDevice->SetFVF(mVBResource->GetVertexFormat());

	// Draw
	effect->CommitChanges();
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);
}

void Particle::Tick(float deltaTime)
{
	mParticleTime += deltaTime;
	mParticleFadeFactor = max<float>(min<float>(float(1.0 - (mParticleTime / mLifeTime)), 1.f), 0.f);
}

void Particle::SetPosition(Vector2D newPosition)
{
	if (mPosition != newPosition)
	{
		GameObject::SetPosition(newPosition);
		OnPositionUpdated();
	}
}

void Particle::OnPositionUpdated()
{
	// Set WVP matrix
	D3DXMATRIX matWorld, matScale, matTrans, matProj;

	D3DXMatrixScaling(&matScale, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&matTrans, 0.5f, gGridHeight * gCellSize + 0.5f, 0.0f);
	D3DXMatrixMultiply(&matWorld, &matScale, &matTrans);

	D3DXMATRIX matParticleTrans;
	D3DXMATRIX matParticleWorldTrans;
	D3DXMatrixTranslation(&matParticleTrans, mPosition.X, -mPosition.Y, 0.0f);
	D3DXMatrixMultiply(&matParticleWorldTrans, &matWorld, &matParticleTrans);

	D3DXMatrixOrthoOffCenterLH(
		&matProj,
		0.0f, (float)gGridWidth * gCellSize,
		0.0f, (float)gGridHeight * gCellSize,
		0.0f, 1.0f);

	D3DXMatrixMultiply(&mWVPMatrix, &matParticleWorldTrans, &matProj);
}

void Particle::SetCellTemplate(Cell* cellTemplate)
{
	if(cellTemplate)
	{
		mResourceName = cellTemplate->GetResourceName();
		SetPosition(cellTemplate->GetPosition());
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
	mParticleTime = 0.f;

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