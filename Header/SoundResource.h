#pragma once
#include <xaudio2.h>
#include "GameResource.h"

class SoundResource : public GameResource
{
public:
	// ctor & dtor
	SoundResource();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();

	// Methods
	void SetPath(LPWSTR filePath);
	LPWSTR GetPath();
	void SetIsLoop(bool loop);
	bool GetIsLoop();
	inline WAVEFORMATEX* GetWaveFormat() { return &mWaveFormat; }
	inline XAUDIO2_BUFFER* GetAudioBuffer() { return &mAudioBuffer; }
private:
	TCHAR mFilePath[MAX_PATH + 1];
	bool mIsLoop;
	XAUDIO2_BUFFER mAudioBuffer;
	WAVEFORMATEX mWaveFormat;
	BYTE* mWaveData;
};