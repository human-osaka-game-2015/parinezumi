#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>

/*自作ライブラリ*/
#include "directinput_lib.h" 
#include "Tex_lib.h"
#include "directx.h"

/*自作ヘッダ*/



/*メモリリーク検出用関数を使うためのヘッダー*/
#include <crtdbg.h>

/*define*/
#define TITLE 	TEXT("ハリネズミ　ケンジ君の大冒険!!")
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }


/*enum*/
enum SCENE
{
	LOGO_SCENE,
	TITLE_SCENE,
	STAGE_SELECT_SCENE,
	GAME_SCENE,
	RESULT_SCENE,
	SCENE_MAX
};

enum TEX_INDEX
{
	LOGO_TEX,
	TITLE_BACKGROUND_TEX,
	STAGE_SELECT,
	TEX_MAX
};



/*directxのオブジェクト*/
IDirect3D9*	pDirect3D;
IDirect3DDevice9*	pD3Device;
LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];


/*グローバル変数*/
int current_scene = LOGO_SCENE;

KEYSTATE Key[KEYMAX];

CUSTOMVERTEX background_TITLE[] =
{
	{    0.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1024.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1024.0f, 768.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{    0.0f, 768.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


/*関数のプロトタイプ宣言*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void Render_Init();

void Render();

void Control();

void Control_Key();










//-------------------------------------------------------------
//
//	メインルーチン
//
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASS winc;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);





	winc.style = CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc = WndProc;
	winc.cbClsExtra = winc.cbWndExtra = 0;
	winc.hInstance = hInstance;
	winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winc.hCursor = LoadCursor(NULL, IDC_ARROW);
	winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName = NULL;
	winc.lpszClassName = TITLE;

	
	if (!RegisterClass(&winc)) return 0;

	
	int dH = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2;
	int dW = GetSystemMetrics(SM_CXFRAME) * 2;

					
	hWnd = CreateWindow(
		TITLE,								
		TITLE, 								
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024 + dW,
		768 + dH,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if (!hWnd) return 0;


	Init_DX(&pDirect3D, hWnd, &pD3Device);

	Init_Dinput();

	Init_Dinput_Key(hWnd);


	Render_Init();


	Tex_Load("LOGO_test.png", &pTexture[LOGO_TEX],pD3Device);
	Tex_Load("TITLE_test.png", &pTexture[TITLE_BACKGROUND_TEX], pD3Device);
	Tex_Load("STAGE_SELECT_test.png", &pTexture[STAGE_SELECT], pD3Device);





	

	DWORD SyncOld = timeGetTime();
	DWORD SyncNow;

	timeBeginPeriod(1);



	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60)
			{
				Render();
				
				Control();

				SyncOld = SyncNow;

			}
		}
	}

	timeEndPeriod(1);


	return (int)msg.wParam;
}



//-------------------------------------------------------------
//
//	メッセージ処理
//
//-------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{

	switch (msg)
	{
	case WM_DESTROY:	//	ウインドウが閉じられた時とか
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wp, lp);

}



//-------------------------------------------------------------
//
//	描画方法の設定
//
//-------------------------------------------------------------
void Render_Init()
{
	pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pD3Device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}


//-------------------------------------------------------------
//
//	描画関数
//
//-------------------------------------------------------------
void Render()
{
	switch (current_scene)
	{
	case LOGO_SCENE:
		
		Draw_Ready(pD3Device);

		Tex_Set_Draw(pD3Device,pTexture[LOGO_TEX],background_TITLE);

		End_Scene(pD3Device);

		break;

	case TITLE_SCENE:

		Draw_Ready(pD3Device);

		Tex_Set_Draw(pD3Device, pTexture[TITLE_BACKGROUND_TEX], background_TITLE);

		End_Scene(pD3Device);

		break;

	case STAGE_SELECT_SCENE:



		break;

	case GAME_SCENE:



		break;

	case RESULT_SCENE:



		break;
	}
}


//-------------------------------------------------------------
//
//	コントロール関数
//
//-------------------------------------------------------------
void Control()
{
	Control_Key();
}


void Control_Key()
{
	switch (current_scene)
	{
	case LOGO_SCENE:

		Key_Check_Dinput(&Key[Z],DIK_Z);

		

		break;

	case TITLE_SCENE:



		break;

	case STAGE_SELECT_SCENE:



		break;

	case GAME_SCENE:



		break;

	case RESULT_SCENE:



		break;
	}
}

//-------------------------------------------------------------
//
//	オブジェクトの解放関数
//
//-------------------------------------------------------------
void Free_DX()
{


	SAFE_RELEASE(pD3Device);

	SAFE_RELEASE(pDirect3D);


}

