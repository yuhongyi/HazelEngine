#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject() :
	mId(-1),
	mPosition(-1.f, -1.f),
	mSize(0.f, 0.f)
{
}

GameObject::GameObject(Vector2D size):
	mId(-1),
	mPosition(-1.f, -1.f),
	mSize(size)
{

}
bool GameObject::Initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	return true;
}

void GameObject::Deinitialize()
{
}

void GameObject::Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect)
{
}

void GameObject::Tick(float deltaTime)
{
}

int GameObject::GetID()
{
	return mId;
}

void GameObject::SetID(int id)
{
	mId = id;
}

Vector2D GameObject::GetPosition() const
{
	return mPosition;
}

void GameObject::SetPosition(Vector2D newPosition)
{
	mPosition = newPosition;
}

void GameObject::SetSize(Vector2D newSize)
{
	mSize = newSize;
}

Vector2D GameObject::GetSize() const
{
	return mSize;
}