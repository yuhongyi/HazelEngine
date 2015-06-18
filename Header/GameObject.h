#pragma once
#include <d3d9.h>
#include "GameMath.h"

class GameObject
{
public:
	// Ctor & dtor
	GameObject();
	GameObject(Vector2D size);

	// Methods
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice) = 0;
	virtual void Deinitialize() = 0;
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect) = 0;
	virtual void Tick(float deltaTime) = 0;
	int GetID();
	void SetID(int id);
	virtual void SetPosition(Vector2D newPosition);
	Vector2D GetPosition() const;
	void SetSize(Vector2D newSize);
	Vector2D GetSize() const;
protected:
	Vector2D mPosition;
	Vector2D mSize;
private:
	int mId;
};
