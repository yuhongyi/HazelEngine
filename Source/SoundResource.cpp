#include "stdafx.h"
#include "SoundResource.h"
#pragma warning(disable:4995)
#pragma warning(disable:4996)


//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
	bool bFound = false;

	if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
		return E_INVALIDARG;

	// Get the exe name, and exe path
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileName( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH - 1] = 0;
	strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
	if( strLastSlash )
	{
		wcscpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

		// Chop the exe name from the exe path
		*strLastSlash = 0;

		// Chop the .exe from the exe name
		strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
		if( strLastSlash )
			*strLastSlash = 0;
	}

	wcscpy_s( strDestPath, cchDest, strFilename );
	if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
		return S_OK;

	// Search all parent directories starting at .\ and using strFilename as the leaf name
	WCHAR strLeafName[MAX_PATH] = {0};
	wcscpy_s( strLeafName, MAX_PATH, strFilename );

	WCHAR strFullPath[MAX_PATH] = {0};
	WCHAR strFullFileName[MAX_PATH] = {0};
	WCHAR strSearch[MAX_PATH] = {0};
	WCHAR* strFilePart = NULL;

	GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
	if( strFilePart == NULL )
		return E_FAIL;

	while( strFilePart != NULL && *strFilePart != '\0' )
	{
		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strFullFileName, MAX_PATH, L"%s\\%s\\%s", strFullPath, strExeName, strLeafName );
		if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
		{
			wcscpy_s( strDestPath, cchDest, strFullFileName );
			bFound = true;
			break;
		}

		swprintf_s( strSearch, MAX_PATH, L"%s\\..", strFullPath );
		GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
	}
	if( bFound )
		return S_OK;

	// On failure, return the file as the path but also return an error code
	wcscpy_s( strDestPath, cchDest, strFilename );

	return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}

SoundResource::SoundResource() :
	mIsLoop(false),
	mWaveData(NULL)
{
	mFilePath[0] = '/0';
	ZeroMemory(&mAudioBuffer, sizeof(mAudioBuffer));
	mWaveData = NULL;
}

bool SoundResource::InitResource(LPDIRECT3DDEVICE9 d3dDevice)
{
	HRESULT hr = S_OK;

	// Locate the wave file
	WCHAR strFilePath[MAX_PATH];
	if( FAILED( hr = FindMediaFileCch( strFilePath, MAX_PATH, mFilePath ) ) )
	{
		wprintf( L"Failed to find media file: %s\n", mFilePath );
		return false;
	}

	// Read in the wave file
	CWaveFile wav;
	if( FAILED( hr = wav.Open( strFilePath, NULL, WAVEFILE_READ ) ) )
	{
		wprintf( L"Failed reading WAV file: %#X (%s)\n", hr, strFilePath );
		return false;
	}

	// Get format of wave file
	mWaveFormat = *wav.GetFormat();

	// Calculate how many bytes and samples are in the wave
	DWORD cbWaveSize = wav.GetSize();

	mWaveData = new BYTE[cbWaveSize];

	// Read the sample data into memory
	if( FAILED( hr = wav.Read( mWaveData, cbWaveSize, &cbWaveSize ) ) )
	{
		wprintf( L"Failed to read WAV data: %#X\n", hr );
		SAFE_DELETE_ARRAY( mWaveData );
		return false;
	}

	// Submit the wave sample data using an XAUDIO2_BUFFER structure
	mAudioBuffer.pAudioData = mWaveData;
	mAudioBuffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
	mAudioBuffer.AudioBytes = cbWaveSize;

	return true;
}

void SoundResource::CleanUpResource()
{
	SAFE_DELETE_ARRAY( mWaveData );
}

void SoundResource::SetPath(LPWSTR filePath)
{
	if(wcslen(filePath) < MAX_PATH)
	{
		wcscpy(&mFilePath[0], filePath);
	}
}

LPWSTR SoundResource::GetPath()
{
	return mFilePath;
}

void SoundResource::SetIsLoop(bool loop)
{
	mIsLoop = loop;
}

bool SoundResource::GetIsLoop()
{
	return mIsLoop;
}