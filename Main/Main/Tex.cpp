#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include"Tex_lib.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//D3DXIMAGE_INFO
//&info  info.Width info.Height info.MipLevels
//EXを引数でとれるようにする

//レンダーの中身を関数でわけとく

/**
 * topleftを起点として画像が大きくなる
 * 拡大したい画像をCUSTOMVERTEX型のポインタで渡す。
 * double型のx,yは何倍にするかを指定する。(小数を使えば小さくもできる)
 */
void Tex_Scaling(CUSTOMVERTEX* Vertex, double x, double y)
{
	Vertex[0].x = (float)(Vertex[0].x * x + Vertex[0].y * 0);
	Vertex[0].y = (float)(Vertex[0].x * 0 + Vertex[0].y * y);

	Vertex[1].x = (float)(Vertex[1].x * x + Vertex[1].y * 0);
	Vertex[1].y = (float)(Vertex[1].x * 0 + Vertex[1].y * y);

	Vertex[2].x = (float)(Vertex[2].x * x + Vertex[2].y * 0);
	Vertex[2].y = (float)(Vertex[2].x * 0 + Vertex[2].y * y);

	Vertex[3].x = (float)(Vertex[3].x * x + Vertex[3].y * 0);
	Vertex[3].y = (float)(Vertex[3].x * 0 + Vertex[3].y * y);

}



/**
 * 回転したい画像のCUSTOMVERTEXのアドレスを渡す
 * 何度回転するかの値を渡す。
 * 回転は右回りで、関数内でラジアンに変換している。
 *
 */
void Tex_Turn(CUSTOMVERTEX* Vertex, double angle)
{

	int i;
	double rad;
	float temp_x;
	float temp_y;

	rad = angle * (3.141592 / 180);

	for (i = 0; i < 4; i++)
	{
		temp_x = Vertex[i].x;
		temp_y = Vertex[i].y;

		Vertex[i].x = (float)(temp_x * cos(rad) - temp_y * sin(rad));
		Vertex[i].y = (float)(temp_y * cos(rad) + temp_x * sin(rad));
	}
}




/**
 * Textureをロードする関数
 * D3DXCreateTextureFromFileを使ってロードする。
 * const char*が画像のファイル名
 * LPDIRECT3DTEXTURE9* は画像を保存する場所のアドレス
 */
int Tex_Load(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device)
{
	if (FAILED(D3DXCreateTextureFromFile(
		pD3Device,
		TEXT(name),
		pTexture)))
	{
		return 1;
	}
	return 0;
}


/**
 * Textureをロードする関数
 * D3DXCreateTextureFromFileExを使ってロードする
 * const char*が画像のファイル名
 * LPDIRECT3DTEXTURE9* は画像を保存する場所のアドレス
 * 
 */
int Tex_Load_EX(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device, int alpha ,int red, int grren, int blue)
{
	if (FAILED(D3DXCreateTextureFromFileEx(
		pD3Device,
		TEXT(name),
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		D3DCOLOR_ARGB(alpha, red, grren, blue),
		NULL, NULL,
		pTexture)))
	{
		return 1;
	}
	return 0;
}


void Tex_Set(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture)
{
	pD3Device->SetTexture(0, pTexture);
}

void Tex_Draw(IDirect3DDevice9* pD3Device, CUSTOMVERTEX* Vertex)
{
	pD3Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		Vertex,
		sizeof(CUSTOMVERTEX));
}

void Tex_Set_Draw(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture, CUSTOMVERTEX* Vertex)
{
	Tex_Set(pD3Device, pTexture);

	pD3Device->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		Vertex,
		sizeof(CUSTOMVERTEX));
}


void Draw_Ready(IDirect3DDevice9* pD3Device)
{
	//画面の消去
	pD3Device->Clear(0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0x00, 0x00, 0x00),
		1.0, 0);

	//描画の開始
	pD3Device->BeginScene();

	//画像の描画
	//描画のフォーマットを設定
	pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
}


void End_Scene(IDirect3DDevice9* pD3Device)
{
	//描画の終了
	pD3Device->EndScene();

	//表示
	pD3Device->Present(NULL, NULL, NULL, NULL);
}
