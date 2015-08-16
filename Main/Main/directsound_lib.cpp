#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "winmm.lib" )

#include <windows.h>
//#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

#include "directsound_lib.h"

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }











HRESULT DSound_Init(HWND hWnd, IDirectSound8 **pDS8)
{
	// サウンドデバイス作成
	DirectSoundCreate8(NULL, pDS8, NULL);
	(*pDS8)->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	return S_OK;
}




int Sound_Play(HWND hWnd, char* filename, IDirectSound8** pDS8, IDirectSoundBuffer8** pDSBuffer)
{


	// Waveファイルオープン
	WAVEFORMATEX wFmt;
	char *pWaveData = 0;
	DWORD waveSize = 0;

	if (!openWave((filename), wFmt, &pWaveData, waveSize))
	{
		return 0;
	}



	DSBUFFERDESC DSBufferDesc;
	DSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
	DSBufferDesc.dwFlags = 0;
	DSBufferDesc.dwBufferBytes = waveSize;
	DSBufferDesc.dwReserved = 0;
	DSBufferDesc.lpwfxFormat = &wFmt;
	DSBufferDesc.guid3DAlgorithm = GUID_NULL;







	IDirectSoundBuffer *ptmpBuf = 0;

	(*pDS8)->CreateSoundBuffer(&DSBufferDesc, &ptmpBuf, NULL);
	ptmpBuf->QueryInterface(IID_IDirectSoundBuffer8, (void**)pDSBuffer);

	ptmpBuf->Release();
	if (pDSBuffer == 0)
	{
		(*pDS8)->Release();
		return 0;
	}



	// セカンダリバッファにWaveデータ書き込み
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;

	if (DS_OK == (*pDSBuffer)->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		(*pDSBuffer)->Unlock(lpvWrite, dwLength, NULL, 0);
	}

	delete[] pWaveData; // 元音はもういらない


	return 0;
}





void Sound_Operation(IDirectSoundBuffer8* pDSBuffer,SOUND_OPERATION operation)
{
	if (operation == PLAY)
	{
		pDSBuffer->Play(0, 0, 0);
	}

	if (operation ==  LOOP)
	{
		pDSBuffer->Play(0, 0, DSBPLAY_LOOPING);
	}

	if (operation == STOP)
	{
		pDSBuffer->Stop();
	}

}




void Release_Sound(IDirectSound8* pDS8, IDirectSoundBuffer8* pDSBuffer)
{
	SAFE_RELEASE(pDSBuffer);
	SAFE_RELEASE(pDS8);
}





// Waveファイルオープン関数
bool openWave(TCHAR *filename, WAVEFORMATEX &wFmt, char** pWaveData, DWORD &waveSize)
{

	//filepathの位置になにもなければ失敗なのでfalseを返す
	if (filename == 0)
	{
		return false;
	}


	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;
	

	// Waveファイルオープン
	memset(&mmioInfo, 0, sizeof(MMIOINFO));				//memsetに0をセット
	hMmio = mmioOpen(filename, &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return false; // ファイルオープン失敗
	}



	// RIFFチャンク検索
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);

	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	// フォーマットチャンク検索
	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmRes = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);

	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	DWORD fmsize = formatChunk.cksize;
	DWORD size = mmioRead(hMmio, (HPSTR)&wFmt, fmsize);

	if (size != fmsize)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	mmioAscend(hMmio, &formatChunk, 0);

	
	// データチャンク検索
	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmRes = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);

	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	*pWaveData = new char[dataChunk.cksize];
	size = mmioRead(hMmio, (HPSTR)*pWaveData, dataChunk.cksize);

	if (size != dataChunk.cksize)
	{
		delete[] * pWaveData;
		return false;
	}

	waveSize = size;

	// ハンドルクローズ
	mmioClose(hMmio, 0);

	return true;
}
