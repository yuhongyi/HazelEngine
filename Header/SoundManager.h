#pragma once

#define _WIN32_DCOM
#define _CRT_SECURE_NO_DEPRECATE
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
#include "SDKwavefile.h"
#include <stdlib.h>


class SoundManager : public Singleton<SoundManager>, public GameResource
{
public:
	// ctor & dtor
	SoundManager();

	// GameResource
	virtual bool InitResource(LPDIRECT3DDEVICE9 d3dDevice);
	virtual void CleanUpResource();
	
	// Methods
	inline IXAudio2* GetXAudio2() { return mXAudio2; }
	inline bool ShouldExit() { return mExit; }

	void PlaySoundEffect(int soundId, bool isLoop);
	static DWORD WINAPI PlaySoundEffectProc(LPVOID lpParameter);
	
private:
	IXAudio2* mXAudio2;
	IXAudio2MasteringVoice* mMasteringVoice;
	vector<IXAudio2SourceVoice*> mSoundStreams;
	bool mExit;

	HRESULT PlayPCM( IXAudio2* pXaudio2, SoundResource* pSoundResource, bool isLooping );
};