#pragma once
#include <windows.h>
#include <string>
#include "Globals.h"

using namespace std;

// Hard coded for now.
// This file will be replaced by a config class to load config file

const int gGridWidth=8;
const int gGridHeight=8;
const int gCellSize=64;
const int gMinimumVanishCount = 3;
const float gFallingAcceleration = 600.f;
const float gSwitchingSpeed = 300.f;

// Particle control
const int gParticlePoolSize = 100;
const float gParticleLifeTime = 1.0f;

// Particle animation
const float gParticleDepartSpeed = 100.f;
const float gParticleRotateSpeed = 10.f;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#endif

// Resource List
static wstring gImageList[] =
{
	L"Resources\\Images\\Bluehound.png",
	L"Resources\\Images\\doggie.png",
	L"Resources\\Images\\Pointy.png",
	L"Resources\\Images\\PurpGuy.png",
	L"Resources\\Images\\RedDog.png",
	L"Resources\\Images\\Woof.png",
	L"Resources\\Images\\Cat.png",
};
static const int gImageCount = ARRAY_SIZE(gImageList);

// Shader list
static wstring gShaderList[] =
{
	L"Shaders\\BasicHLSL.fx",
};
static const int gShaderCount = ARRAY_SIZE(gShaderList);
