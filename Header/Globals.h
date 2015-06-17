#pragma once
#pragma warning(disable:4100)
#include <d3d9.h>
#include <dxerr.h>
#include <D3DX9Effect.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory>
#include <algorithm>

#include "GameConfig.h"
#include "GameMath.h"

#define COLORDEBUGGING 0

//--------------------------------------------------------------------------------------
// Helper macros
//--------------------------------------------------------------------------------------

#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif