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
	// �T�E���h�f�o�C�X�쐬
	DirectSoundCreate8(NULL, pDS8, NULL);
	(*pDS8)->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

	return S_OK;
}




int Sound_Play(HWND hWnd, char* filename, IDirectSound8** pDS8, IDirectSoundBuffer8** pDSBuffer)
{


	// Wave�t�@�C���I�[�v��
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



	// �Z�J���_���o�b�t�@��Wave�f�[�^��������
	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;

	if (DS_OK == (*pDSBuffer)->Lock(0, 0, &lpvWrite, &dwLength, NULL, NULL, DSBLOCK_ENTIREBUFFER))
	{
		memcpy(lpvWrite, pWaveData, dwLength);
		(*pDSBuffer)->Unlock(lpvWrite, dwLength, NULL, 0);
	}

	delete[] pWaveData; // �����͂�������Ȃ�


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





// Wave�t�@�C���I�[�v���֐�
bool openWave(TCHAR *filename, WAVEFORMATEX &wFmt, char** pWaveData, DWORD &waveSize)
{

	//filepath�̈ʒu�ɂȂɂ��Ȃ���Ύ��s�Ȃ̂�false��Ԃ�
	if (filename == 0)
	{
		return false;
	}


	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo;
	

	// Wave�t�@�C���I�[�v��
	memset(&mmioInfo, 0, sizeof(MMIOINFO));				//memset��0���Z�b�g
	hMmio = mmioOpen(filename, &mmioInfo, MMIO_READ);
	if (!hMmio)
	{
		return false; // �t�@�C���I�[�v�����s
	}



	// RIFF�`�����N����
	MMRESULT mmRes;
	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmRes = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);

	if (mmRes != MMSYSERR_NOERROR)
	{
		mmioClose(hMmio, 0);
		return false;
	}


	// �t�H�[�}�b�g�`�����N����
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

	
	// �f�[�^�`�����N����
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

	// �n���h���N���[�Y
	mmioClose(hMmio, 0);

	return true;
}
