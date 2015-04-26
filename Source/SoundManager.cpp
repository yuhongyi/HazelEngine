#include "stdafx.h"


HANDLE gHandleAudioThreadShouldEnd;
HANDLE gHandleAudioThreadHasEnded;

// The wav playing code is COPIED from DXSDK,
// but the threading part is not.
SoundManager::SoundManager() :
	mXAudio2(NULL),
	mMasteringVoice(NULL),
	mExit(false)
{
}

bool SoundManager::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	// Initialize XAudio2
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	if(FAILED(XAudio2Create(&mXAudio2, flags)))
	{
		wprintf(L"Failed to init XAudio2 engine:\n");
		CoUninitialize();
		return false;
	}

	// Create a mastering voice
	IXAudio2MasteringVoice* mMasteringVoice = NULL;

	if(FAILED(mXAudio2->CreateMasteringVoice(&mMasteringVoice)))
	{
		wprintf(L"Failed creating mastering voice: \n");
		SAFE_RELEASE(mXAudio2);
		CoUninitialize();
		return false;
	}

	// Create events
	gHandleAudioThreadShouldEnd = CreateEvent( NULL, FALSE, FALSE, NULL );
	gHandleAudioThreadHasEnded = CreateEvent( NULL, FALSE, FALSE, NULL );

	// Create audio thread
	CreateThread(NULL, 0, PlaySoundEffectProc, &mSoundStreams, 0, NULL);

	// Loop background music
	PlaySoundEffect(7, true);
	return true;
}

void SoundManager::CleanUpResource()
{
	mExit = true;

	// Wait for sound thread to end
	SetEvent(gHandleAudioThreadShouldEnd);
	WaitForSingleObject(gHandleAudioThreadHasEnded, INFINITE);
	CloseHandle(gHandleAudioThreadShouldEnd);
	CloseHandle(gHandleAudioThreadHasEnded);

	for(auto soundIter = mSoundStreams.cbegin(); soundIter != mSoundStreams.cend(); soundIter++)
	{
		(*soundIter)->DestroyVoice();
	}
	mSoundStreams.clear();

	SAFE_RELEASE(mXAudio2);
	CoUninitialize();
}

//--------------------------------------------------------------------------------------
// Name: PlayPCM
// Desc: Plays a wave and blocks until the wave finishes playing
//--------------------------------------------------------------------------------------
HRESULT SoundManager::PlayPCM( IXAudio2* pXaudio2, SoundResource* pSoundResource, bool isLooping )
{
	HRESULT hr = S_OK;

	// Play the wave using a XAudio2SourceVoice
	// Create the source voice
	IXAudio2SourceVoice* pSourceVoice;
	if( FAILED( hr = pXaudio2->CreateSourceVoice( &pSourceVoice, pSoundResource->GetWaveFormat() ) ) )
	{
		wprintf( L"Error %#X creating source voice\n", hr );
		return hr;
	}

	XAUDIO2_BUFFER* audioBuffer = pSoundResource->GetAudioBuffer();
	audioBuffer->LoopCount = isLooping ? XAUDIO2_LOOP_INFINITE : 0;

	if( FAILED( hr = pSourceVoice->SubmitSourceBuffer( audioBuffer ) ) )
	{
		wprintf( L"Error %#X submitting source buffer\n", hr );
		pSourceVoice->DestroyVoice();
		return hr;
	}

	if(FAILED(hr = pSourceVoice->Start( 0 )))
	{
		pSourceVoice->DestroyVoice();
		return hr;
	}

	mSoundStreams.push_back(pSourceVoice);

	return hr;
}

void SoundManager::PlaySoundEffect(int soundId, bool isLooping)
{
	IXAudio2* xAudio = SoundManager::GetInstance()->GetXAudio2();
	SoundResource* soundResource = dynamic_cast<SoundResource*>(ResourceManager::GetInstance()->GetResourceById(soundId));

	if(xAudio && soundResource)
	{
		PlayPCM(xAudio, soundResource, isLooping);
	}
}

DWORD WINAPI SoundManager::PlaySoundEffectProc(LPVOID lpParameter)
{
	vector<IXAudio2SourceVoice*>* soundStreams = (vector<IXAudio2SourceVoice*>*)lpParameter;
	if(!soundStreams)
	{
		CoUninitialize();
		return 1;
	}
	
	while(WaitForSingleObject(gHandleAudioThreadShouldEnd, 10) != WAIT_OBJECT_0)
	{
		for(auto soundIter = soundStreams->cbegin(); soundIter != soundStreams->cend(); )
		{
			// Let the sound play
			XAUDIO2_VOICE_STATE state;
			(*soundIter)->GetState( &state );
			bool isRunning = ( state.BuffersQueued > 0 ) != 0;
	
			if(!isRunning)
			{
				(*soundIter)->DestroyVoice();
				soundIter = soundStreams->erase(soundIter);
			}
			else
			{
				soundIter++;
			}
		}
	}

	CoUninitialize();
	SetEvent(gHandleAudioThreadHasEnded);
	return 0;
}
