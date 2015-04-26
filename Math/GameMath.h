#pragma once
#include "math.h"

struct Vector2D
{
	float X;
	float Y;

	Vector2D():
	X(0.f), Y(0.f)
	{
	}

	Vector2D(float size_x, float size_y):
	X(size_x), Y(size_y)
	{
	}

	inline bool operator == (const Vector2D &rhs) const
	{
		if(this->X == rhs.X && this->Y == rhs.Y)
		{
			return true;
		}

		return false;
	}

	inline bool operator != (const Vector2D &rhs) const
	{
		if(this->X == rhs.X && this->Y == rhs.Y)
		{
			return false;
		}

		return true;
	}

	inline Vector2D operator + (const Vector2D &rhs) const
	{
		return Vector2D(this->X + rhs.X, this->Y + rhs.Y);
	}

	inline Vector2D operator - (const Vector2D &rhs) const
	{
		return Vector2D(this->X - rhs.X, this->Y - rhs.Y);
	}
};

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex
	D3DCOLOR color;
	FLOAT texcoordU, texcoordV;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// A structure for our custom vertex type
struct CUSTOMVERTEXFORPARTICLE
{
	FLOAT x, y, z; // The transformed position for the vertex
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	FLOAT texcoordU, texcoordV;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEXFORPARTICLE (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


