#pragma once

// Hard coded for now.
// This file will be replaced by a config class to load config file

const int gGridWidth=8;
const int gGridHeight=8;
const int gCellSize=64;
const int gMinimumVanishCount = 3;
const float gFallingAcceleration = 200.f;
const float gSwitchingSpeed = 200.f;

// Particle control
const int gParticlePoolSize = 100;
const float gParticleLifeTime = 1.0f;

// Particle animation
const float gParticleDepartSpeed = 100.f;
const float gParticleRotateSpeed = 10.f;


// Resource List
static const int gImageCount = 7;
static LPWSTR gImageList[gImageCount] =
{
	L"Resources\\Images\\Bluehound.png",
	L"Resources\\Images\\doggie.png",
	L"Resources\\Images\\Pointy.png",
	L"Resources\\Images\\PurpGuy.png",
	L"Resources\\Images\\RedDog.png",
	L"Resources\\Images\\Woof.png",
	L"Resources\\Images\\Cat.png",
};

static const int gSoundCount = 3;
static LPWSTR gSoundList[gSoundCount] = 
{
	L"Resources\\Sound\\MusicMono.wav",
	L"Resources\\Sound\\0085.WAV",
	L"Resources\\Sound\\0001.WAV",
};

static const int gShaderCount = 1;
static LPWSTR gShaderList[gShaderCount] =
{
	L"Shaders\\BasicHLSL.fx",
};