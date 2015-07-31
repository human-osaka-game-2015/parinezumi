#include "directx.h"

D3DPRESENT_PARAMETERS d3dpp;		//	パラメーター

D3DDISPLAYMODE d3ddm;


/**
 *directxの初期化関数
 *
 */
void Init_DX(LPDIRECT3D9* lpDirect3D, HWND hWnd, LPDIRECT3DDEVICE9* pD3Device)
{
	

	//DirectX オブジェクトの生成
	*lpDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	//Display Mode の設定
	(*lpDirect3D)->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT,
		&d3ddm);
	ZeroMemory(&d3dpp,
			   sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = TRUE;


	//デバイスを作る
	(*lpDirect3D)->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, pD3Device);
}

