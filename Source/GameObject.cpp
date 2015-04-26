#include "Globals.h"
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