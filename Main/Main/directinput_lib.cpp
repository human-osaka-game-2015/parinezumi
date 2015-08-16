#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <dinput.h>
#include"directinput_lib.h"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }

#define BUFFER_SIZE  64


LPDIRECTINPUT8 pDinput = NULL;

//キー
LPDIRECTINPUTDEVICE8 pKeyDevice = NULL;

//マウス
LPDIRECTINPUTDEVICE8 pMouseDevice = NULL;	//　マウスデバイス 










/**
 * ダイレクトインプットの初期化関数
 */
HRESULT Init_Dinput()
{
	HRESULT hr;

	//「DirectInput」オブジェクトの作成
	if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return hr;
	}

	return S_OK;
}


/**
 * ダイレクトインプットのキーボード初期化
 */
HRESULT Init_Dinput_Key(HWND hWnd)
{
	HRESULT hr;

	//キーボードの初期化
	if(FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pKeyDevice, NULL)))
	{
		return hr;
	}

	//データフォーマット
	if(FAILED(hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//協調レベル
	if(FAILED(hr = pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	pKeyDevice->Acquire();
	return S_OK;
}



/**
 * ダイレクトインプットのマウス初期化
 */
HRESULT Init_Dinput_Mouse(HWND hWnd)
{
	HRESULT hr;


	//マウスの初期化
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysMouse,
		&pMouseDevice, NULL)))
	{
		return hr;
	}

	//データフォーマット
	if (FAILED(hr = pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	//協調レベル
	if (FAILED(hr = pMouseDevice->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	DIPROPDWORD dipdw;
	// the header
	dipdw.diph.dwSize = sizeof(dipdw);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = DIPROPAXISMODE_REL;


	if (FAILED(hr = pMouseDevice->SetProperty(DIPROP_AXISMODE, &dipdw.diph)))
	{
		return hr;
	}


	// バッファ
	dipdw.dwData = BUFFER_SIZE;

	if (FAILED(hr = pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		return hr;
	}

	pMouseDevice->Acquire();
	return S_OK;
}






/**
 * KEYSTATE* Keyはキーの状態を保存する場所(KEYSTATE型はtest_lib.hで宣言してある)
 * int DIKはDirectinputの文字識別コード、状態をチェックしたいキーの状態を保存する
 * LPDIRECTINPUTDEVICE8　はデバイスのオブジェクト
 */
void Key_Check_Dinput(KEYSTATE* Key, int DIK)
{
	BYTE diks[256];
	static int old_diks[256] = {OFF};

	HRESULT hr = pKeyDevice->Acquire();
	if((hr == DI_OK) || (hr == S_FALSE))
	{
		pKeyDevice->GetDeviceState(sizeof(diks), &diks);

		if(diks[DIK] & 0x80)
		{
			if(old_diks[DIK] == OFF)
			{
				*Key = PUSH;
			}
			else
			{
				*Key = ON;
			}
			old_diks[DIK] = ON;
		}
		else
		{
			if(old_diks[DIK] == ON)
			{
				*Key = RELEASE;
			}
			else
			{
				*Key = OFF;
			}
			old_diks[DIK] = OFF;
		}
	}
}



/**
 *マウスの状態をチェックする関数
 *MOUSEKINDはマウスの状態を格納する構造体
 *一度呼ぶだけですべての状態をとってくれる。
 */
void Mouse_Check(MOUSEKIND* Mouse)
{
	DIMOUSESTATE dims;

	pMouseDevice->Acquire();
	HRESULT hr = pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
	if(SUCCEEDED(hr))
	{
		if(dims.rgbButtons[0])
		{
			Mouse->LEFT = true;
		}
		else
		{
			Mouse->LEFT = false;
		}

		if(dims.rgbButtons[1])
		{
			Mouse->RIGHT = true;
		}
		else
		{
			Mouse->RIGHT = false;
		}

		if(dims.lX)
		{
			Mouse->lx = dims.lX;
		}

		if(dims.lY)
		{
			Mouse->ly = dims.lY;
		}

		if(dims.lZ)
		{
			Mouse->lz = dims.lZ;
		}
	}
}


//pDinputのリリース
void Release_pDinput()
{
	SAFE_RELEASE(pDinput);
}

//キーボードのリリース
void Release_Key()
{
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	SAFE_RELEASE(pKeyDevice);
}

//マウスのリリース
void Release_Mouse()
{
	if(pMouseDevice)
	{
		pMouseDevice->Unacquire();
	}

	SAFE_RELEASE(pMouseDevice);
}


