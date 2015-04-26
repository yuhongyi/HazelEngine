#pragma once
#include "DXUT.h"
#include "GameConfig.h"
#include "Interfaces.h"
#include "GameMath.h"
#include "Singleton.h"
#include <Windows.h>

#include "ResourceManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "ParticleManager.h"

#define COLORDEBUGGING 0

//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif