


enum SOUND_OPERATION
{
	PLAY,
	STOP,
	LOOP
};




HRESULT DSound_Init(HWND hWnd, IDirectSound8 **pDS8);

int Sound_Play(HWND hWnd, char* filename, IDirectSound8** pDS8, IDirectSoundBuffer8** pDSBuffer);

void Sound_Operation(IDirectSoundBuffer8* pDSBuffer, SOUND_OPERATION operation);

void Release_Sound(IDirectSound8* pDS8, IDirectSoundBuffer8* pDSBuffer);

bool openWave(TCHAR *filepath, WAVEFORMATEX &waveFormatEx, char** ppData, DWORD &dataSize);

