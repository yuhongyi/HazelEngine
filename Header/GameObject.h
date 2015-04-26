#pragma once


class GameObject
{
public:
	// Ctor & dtor
	GameObject();
	GameObject(Vector2D size);

	// Methods
	virtual bool Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void Deinitialize();
	virtual void Render(LPDIRECT3DDEVICE9 d3dDevice, ID3DXEffect* effect);
	virtual void Tick(float deltaTime);
	int GetID();
	void SetID(int id);
	void SetPosition(Vector2D newPosition);
	Vector2D GetPosition() const;
	void SetSize(Vector2D newSize);
	Vector2D GetSize() const;
protected:
	Vector2D mPosition;
	Vector2D mSize;
private:
	int mId;
};
