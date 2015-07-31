#include "directx.h"

D3DPRESENT_PARAMETERS d3dpp;		//	�p�����[�^�[

D3DDISPLAYMODE d3ddm;


/**
 *directx�̏������֐�
 *
 */
void Init_DX(LPDIRECT3D9* lpDirect3D, HWND hWnd, LPDIRECT3DDEVICE9* pD3Device)
{
	

	//DirectX �I�u�W�F�N�g�̐���
	*lpDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//Display Mode �̐ݒ�
	(*lpDirect3D)->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&d3ddm);
	ZeroMemory(&d3dpp,
			   sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;


	//�f�o�C�X�����
	(*lpDirect3D)->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, pD3Device);
}

