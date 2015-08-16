#include <dinput.h>
#include <d3dx9tex.h>




#ifndef _VERTEX_
#define _VERTEX_

typedef struct _CUSTOMVERTEX
{
	float           x, y, z;                //位置情報
	float           rhw;                    //頂点変換値
	D3DCOLOR        color;                  //頂点カラー
	float           tu, tv;                 //テクスチャ座標
} CUSTOMVERTEX, *LPCUSTOMVERTEX;

#endif


/**
 *@brief Textureをロードする関数
 *
 *@param name		ロードする画像ファイルの名前
 *@param pTexture	画像をロードする場所
 *@param pD3Device	デバイスのオブジェクト
 *
 *@return 成功した場合は0を、失敗した場合は1を返す。
 */
int Tex_Load(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device);

/**
 *@brief			Textureをロードする関数　黒が抜ける
 *
 *@param name		ロードする画像ファイルの名前
 *@param pTexture	画像をロードする場所
 *@param pD3Device	デバイスのオブジェクト
 *@param alpha		抜き色のアルファ
 *@param red		抜き色の赤
 *@param grren		抜き色の緑
 *@param blue		抜き色の青
 *
 *@return			成功した場合は0を、失敗した場合は1を返す。 
 */
int Tex_Load_EX(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device, int alpha, int red, int grren, int blue);



/**
 *@brief		画像の拡縮の関数
 *
 *@param Vertex	拡縮する画像のパラメーター
 *@param x		x軸の拡縮の倍率
 *@param y		y軸の拡縮の倍率
 */
void Tex_Scaling(CUSTOMVERTEX* Vertex, double x, double y);

/**
 *@brief		回転の関数
 *
 *@param TEX	回転する画像のパラメーター
 *@param angle	マイナスが左回転・プラスが右回転
 */
void Tex_Turn(CUSTOMVERTEX* Vertex, double angle);


/**
 *@brief	画像のセット関数
 *
 *@param	pD3Device 描画するためにデバイスのオブジェクト
 *@param	pTexture 画像をセットする関数
 */
void Tex_Set(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture);

/**
 *@brief	画像の描画関数
 *
 *@param	pD3Device 描画するためにデバイスのオブジェクト
 *@param	Vertex 表示する画像のパラメーターを渡す。
 */
void Tex_Draw(IDirect3DDevice9* pD3Device, CUSTOMVERTEX* Vertex);

/**
 *@brief	画像のセット&描画関数
 *
 *@param	pD3Device 描画するためにデバイスのオブジェクト
 *@param	Vertex 表示する画像のパラメーターを渡す。
 *@param	pTexture 画像をセットする関数
 */
void Tex_Set_Draw(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture, CUSTOMVERTEX* Vertex);


/**
 *@brief	描画するための準備関数
 *
 *@param	pD3Device デバイスのオブジェクトを渡す。
 */
void Draw_Ready(IDirect3DDevice9* pD3Device);


/**
 *@brief	描画の終了関数
 *
 *@param	pD3Device デバイスのオブジェクトを渡す。
 */
void End_Scene(IDirect3DDevice9* pD3Device);


//
//void Vertex_tmp(CUSTOMVERTEX* Vertex, CUSTOMVERTEX* tmp);

