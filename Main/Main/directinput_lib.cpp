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

//�L�[
LPDIRECTINPUTDEVICE8 pKeyDevice = NULL;

//�}�E�X
LPDIRECTINPUTDEVICE8 pMouseDevice = NULL;	//�@�}�E�X�f�o�C�X 










/**
 * �_�C���N�g�C���v�b�g�̏������֐�
 */
HRESULT Init_Dinput()
{
	HRESULT hr;

	//�uDirectInput�v�I�u�W�F�N�g�̍쐬
	if(FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return hr;
	}

	return S_OK;
}


/**
 * �_�C���N�g�C���v�b�g�̃L�[�{�[�h������
 */
HRESULT Init_Dinput_Key(HWND hWnd)
{
	HRESULT hr;

	//�L�[�{�[�h�̏�����
	if(FAILED(hr = pDinput->CreateDevice(GUID_SysKeyboard,
		&pKeyDevice, NULL)))
	{
		return hr;
	}

	//�f�[�^�t�H�[�}�b�g
	if(FAILED(hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}

	//�������x��
	if(FAILED(hr = pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}

	pKeyDevice->Acquire();
	return S_OK;
}



/**
 * �_�C���N�g�C���v�b�g�̃}�E�X������
 */
HRESULT Init_Dinput_Mouse(HWND hWnd)
{
	HRESULT hr;


	//�}�E�X�̏�����
	if (FAILED(hr = pDinput->CreateDevice(GUID_SysMouse,
		&pMouseDevice, NULL)))
	{
		return hr;
	}

	//�f�[�^�t�H�[�}�b�g
	if (FAILED(hr = pMouseDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return hr;
	}

	//�������x��
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


	// �o�b�t�@
	dipdw.dwData = BUFFER_SIZE;

	if (FAILED(hr = pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		return hr;
	}

	pMouseDevice->Acquire();
	return S_OK;
}






/**
 * KEYSTATE* Key�̓L�[�̏�Ԃ�ۑ�����ꏊ(KEYSTATE�^��test_lib.h�Ő錾���Ă���)
 * int DIK��Directinput�̕������ʃR�[�h�A��Ԃ��`�F�b�N�������L�[�̏�Ԃ�ۑ�����
 * LPDIRECTINPUTDEVICE8�@�̓f�o�C�X�̃I�u�W�F�N�g
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
 *�}�E�X�̏�Ԃ��`�F�b�N����֐�
 *MOUSEKIND�̓}�E�X�̏�Ԃ��i�[����\����
 *��x�ĂԂ����ł��ׂĂ̏�Ԃ��Ƃ��Ă����B
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


//pDinput�̃����[�X
void Release_pDinput()
{
	SAFE_RELEASE(pDinput);
}

//�L�[�{�[�h�̃����[�X
void Release_Key()
{
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
	}

	SAFE_RELEASE(pKeyDevice);
}

//�}�E�X�̃����[�X
void Release_Mouse()
{
	if(pMouseDevice)
	{
		pMouseDevice->Unacquire();
	}

	SAFE_RELEASE(pMouseDevice);
}


