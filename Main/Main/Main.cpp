#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <stdio.h>

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
#define PI 3.1415926535
#define MAP_HEIGHT 23
#define MAP_WIDTH 100
#define TIPSIZE 32
#define STAGE_MAX 1



/*enum*/
enum SCENE
{
	LOGO_SCENE,
	TITLE_SCENE,
	STAGE_SELECT_SCENE,
	GAME1_SCENE,
	GAME2_SCENE,
	RESULT_SCENE,
	GAMEOVER_SCENE,
	SCENE_MAX
};

enum TEX_INDEX
{
	LOGO_TEX,
	BLACK_TEX,
	TITLE_BACKGROUND_TEX,
	STAGE_SELECT_BACKGROUND_TEX,
	STAGE_SELECT_TEX,
	ENEMY_TEX,
	START_TEX,
	OPTION_TEX,
	OPTION_SCENE_TEX,

	TUTORIAL_TEX,
	TUTORIAL_SCENE_TEX,

	
	UI_TEX,
	GAMEOVER_TEX,
	RESULT_TEX,
	GAME_BACKGROUND_TEX,

	C0101_TEX,



	TEST_TEX,
	TEX_MAX
};



/*directxのオブジェクト*/
IDirect3D9*	pDirect3D;
IDirect3DDevice9*	pD3Device;
LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];


/*グローバル変数*/
int current_scene = LOGO_SCENE;
int map[MAP_HEIGHT][MAP_WIDTH];
int title_select_cursor = 0;
int title_select_option_cursor = 0;
int stage_select_cursor = 0;

unsigned int stage_select_time = 0;


bool Logo_change_flag = false;

bool stage_left_direction = false;

bool isGame1_start = false;
bool isGame2_start = false;
bool isTutorial = false;
bool isOption = false;




KEYSTATE Key[KEYMAX];


CUSTOMVERTEX background[] =
{
	{    0.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{    0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX black_background[] =
{
	{    0.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{    0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX background_LOGO[] =
{
	{    0.0f, 0.0f,   0.5f, 1.0f, 0x00FFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 0.0f,   0.5f, 1.0f, 0x00FFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 1.0f, 1.0f },
	{    0.0f, 720.0f, 0.5f, 1.0f, 0x00FFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX TITLE_select[] =
{
	{ 520.0f,  350.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 780.0f,  350.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 780.0f,  430.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 520.0f,  430.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX map_tip[] =
{
	{ 0.0f,  0.0f,  0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 32.0f, 0.0f,  0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 32.0f, 32.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f,  32.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX stage_select_map[] =
{
	{ 0.0f,    300.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 300.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 500.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f,    500.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX stage_select_char[] =
{
	{ 150.0f, 370.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 250.0f, 370.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ 250.0f, 470.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ 150.0f, 470.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};




/*関数のプロトタイプ宣言*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void Render_Init();

void Render();

void Control();

void Control_Key();

void Free_DX();










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
		1280 + dW,
		720 + dH,
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
	Tex_Load("STAGE_SELECT_test.png", &pTexture[STAGE_SELECT_TEX], pD3Device);
	Tex_Load("Test.bmp", &pTexture[TEST_TEX], pD3Device);
	Tex_Load("GAME.png", &pTexture[GAME_BACKGROUND_TEX], pD3Device);
	Tex_Load("GAMECLEAR.png", &pTexture[RESULT_TEX], pD3Device);
	Tex_Load("GAMEOVER.png", &pTexture[GAMEOVER_TEX], pD3Device);
	Tex_Load("black.png", &pTexture[BLACK_TEX], pD3Device);
	Tex_Load("STAGE_SELECT_test.png", &pTexture[STAGE_SELECT_BACKGROUND_TEX],pD3Device);
	Tex_Load("option_scene.png", &pTexture[OPTION_SCENE_TEX], pD3Device);
	Tex_Load("tutorial_scene.png", &pTexture[TUTORIAL_SCENE_TEX], pD3Device);



	Tex_Load_EX("stage_select_map.png", &pTexture[STAGE_SELECT_TEX], pD3Device,255,255,255,255);
	Tex_Load_EX("C0101.png", &pTexture[C0101_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("start.png", &pTexture[START_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("tutorial.png", &pTexture[TUTORIAL_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("option.png", &pTexture[OPTION_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("ui.png", &pTexture[UI_TEX], pD3Device, 255, 255, 255, 255);


		
	

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
	
	Free_DX();

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

		

		
		if (Logo_change_flag == true)
		{
			for (int i = 0; i < 4; i++)
			{

				black_background[i].color += 0x03000000;
			}
		}


		if (Logo_change_flag == false)
		{
			for (int i = 0; i < 4; i++)
			{
				black_background[i].color -= 0x03000000;
			}
		}

	
		if (black_background[0].color <= 0x00FFFFFF && Logo_change_flag == false)
		{
			Logo_change_flag = true;
		}
				
		if (black_background[0].color >= 0xFFFFFFFF && Logo_change_flag == true)
		{
			current_scene = TITLE_SCENE;
		}
			
		Tex_Set_Draw(pD3Device, pTexture[LOGO_TEX], background);
		Tex_Set_Draw(pD3Device, pTexture[BLACK_TEX], black_background);

	
		End_Scene(pD3Device);

		break;

	case TITLE_SCENE:

		Draw_Ready(pD3Device);


		CUSTOMVERTEX background_TITLE[4];
		CUSTOMVERTEX UI[4];
		CUSTOMVERTEX Tutorial[4];
		CUSTOMVERTEX Option[4];

		
		for (int i = 0; i < 4; i++)
		{
			background_TITLE[i] = background[i];
		}

		UI[0] = { 400.0f, 420.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f };
		UI[1] = { 510.0f, 420.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f };
		UI[2] = { 510.0f, 540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f };
		UI[3] = { 400.0f, 540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f };


		
		for (int i = 0; i < 4; i++)
		{
			Tutorial[i] = TITLE_select[i];
			Option[i] = TITLE_select[i];

			Tutorial[i].y += 80;
			Option[i].y += 160;
		}
		



		Tex_Set_Draw(pD3Device, pTexture[TITLE_BACKGROUND_TEX], background_TITLE);
		Tex_Set_Draw(pD3Device, pTexture[START_TEX], TITLE_select);
		Tex_Set_Draw(pD3Device, pTexture[TUTORIAL_TEX], Tutorial);
		Tex_Set_Draw(pD3Device, pTexture[OPTION_TEX], Option);







		if (title_select_cursor == 0)
		{
			for (int i = 0; i < 4; i++)
			{
				UI[i].y = TITLE_select[i].y;
			}

			Tex_Set_Draw(pD3Device, pTexture[UI_TEX], UI);
		}

		if (title_select_cursor == 1)
		{
			for (int i = 0; i < 4; i++)
			{
				UI[i].y = TITLE_select[i].y;
			}
			UI[0].y += 80;
			UI[1].y += 80;
			UI[2].y += 80;
			UI[3].y += 80;

			Tex_Set_Draw(pD3Device, pTexture[UI_TEX], UI);
		}

		if (title_select_cursor == 2)
		{
			for (int i = 0; i < 4; i++)
			{
				UI[i].y = TITLE_select[i].y;
			}
			UI[0].y += 160;
			UI[1].y += 160;
			UI[2].y += 160;
			UI[3].y += 160;

			Tex_Set_Draw(pD3Device, pTexture[UI_TEX], UI);
		}



		


		if (isTutorial == true)
		{
			CUSTOMVERTEX tutorial[] =
			{
				{   0.0f,  0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
				{ 1280.0f, 0.0f,   0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
				{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
				{   0.0f,  720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
			};

			Tex_Set_Draw(pD3Device, pTexture[TUTORIAL_SCENE_TEX],tutorial);
		}


		if (isOption == true)
		{
			CUSTOMVERTEX option[] =
			{
				{ 300.0f, 100.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
				{ 980.0f, 100.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
				{ 980.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
				{ 300.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
			};

			Tex_Set_Draw(pD3Device, pTexture[OPTION_SCENE_TEX], option);
		}



		End_Scene(pD3Device);

		break;


	case STAGE_SELECT_SCENE:



		Draw_Ready(pD3Device);

		stage_select_time++;
		if (stage_select_time > 100000)
		{
			stage_select_time = 0;
		}

		float temp;

		/*キャラクターの向きが左向きの時に、キャラクターが右を向いてたら向きを変える*/
		if (stage_left_direction == true && stage_select_char[0].tu < stage_select_char[1].tu)
		{
			temp = stage_select_char[0].tu;
			stage_select_char[0].tu = stage_select_char[1].tu;
			stage_select_char[1].tu = temp;

			temp = stage_select_char[2].tu;
			stage_select_char[2].tu = stage_select_char[3].tu;
			stage_select_char[3].tu = temp;
		}

		/*キャラクターの向きが右向きの時に、キャラクターが左を向いてたら向きを変える*/
		if (stage_left_direction == false && stage_select_char[0].tu > stage_select_char[1].tu)
		{
			temp = stage_select_char[0].tu;
			stage_select_char[0].tu = stage_select_char[1].tu;
			stage_select_char[1].tu = temp;

			temp = stage_select_char[2].tu;
			stage_select_char[2].tu = stage_select_char[3].tu;
			stage_select_char[3].tu = temp;
		}


		if (stage_select_time % 5 == 0)
		{
			if (stage_select_char[1].tu == 1.0 && stage_left_direction == false)
			{
				stage_select_char[0].tu = 0.0;
				stage_select_char[1].tu = 0.25;
				stage_select_char[2].tu = 0.25;
				stage_select_char[3].tu = 0.0;
			}
			else if (stage_select_char[0].tu == 1.0 && stage_left_direction == true)
			{
				stage_select_char[0].tu = 0.25;
				stage_select_char[1].tu = 0.0;
				stage_select_char[2].tu = 0.0;
				stage_select_char[3].tu = 0.25;
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					stage_select_char[i].tu += 0.25;

				}
			}
		}


		CUSTOMVERTEX background_STAGESELECT[4];

		for (int i = 0; i < 4; i++)
		{
			background_STAGESELECT[i] = background[i];
		}


		Tex_Set_Draw(pD3Device, pTexture[STAGE_SELECT_BACKGROUND_TEX], background_STAGESELECT);
		Tex_Set_Draw(pD3Device, pTexture[STAGE_SELECT_TEX], stage_select_map);
		Tex_Set_Draw(pD3Device, pTexture[C0101_TEX], stage_select_char);



		End_Scene(pD3Device);


		break;

	case GAME1_SCENE:

		Draw_Ready(pD3Device);

		if (isGame1_start == true)
		{
			FILE*  fp;
			fopen_s(&fp, "Game1.csv", "r");

			for (int i = 0; i < MAP_HEIGHT; i++)
			{
				for (int j = 0; j < MAP_WIDTH; j++)
				{
					fscanf_s(fp, "%d,", &map[i][j], _countof(map));
				}
			}

			isGame1_start = false;
		}



		/*背景の描画処理*/
		CUSTOMVERTEX background_GAME1[4];

		for (int i = 0; i < 4; i++)
		{
			background_GAME1[i] = background[i];
		}
		
		Tex_Set_Draw(pD3Device, pTexture[GAME_BACKGROUND_TEX], background_GAME1);



		/*マップの描画処理*/
		CUSTOMVERTEX tmp_map1[4];
		
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{

				for (int i = 0; i < 4; i++)
				{
					tmp_map1[i] = map_tip[i];
				}

				for (int i = 0; i < 4; i++)
				{
					tmp_map1[i].x += (x * 32);
					tmp_map1[i].y += (y * 32);
				}
				

				if (map[y][x] == 1)
				{
					tmp_map1[0].tu = 0.25;
					tmp_map1[0].tv = 0.0;
					tmp_map1[1].tu = 0.5;
					tmp_map1[1].tv = 0.0;
					tmp_map1[2].tu = 0.5;
					tmp_map1[2].tv = 1.0;
					tmp_map1[3].tu = 0.25;
					tmp_map1[3].tv = 1.0;

					Tex_Set_Draw(pD3Device, pTexture[TEST_TEX], tmp_map1);
				}

				if (map[y][x] == 2)
				{
					tmp_map1[0].tu = 0.5;
					tmp_map1[0].tv = 0.0;
					tmp_map1[1].tu = 0.75;
					tmp_map1[1].tv = 1.0;
					tmp_map1[2].tu = 0.75;
					tmp_map1[2].tv = 1.0;
					tmp_map1[3].tu = 0.5;
					tmp_map1[3].tv = 1.0;

					Tex_Set_Draw(pD3Device, pTexture[TEST_TEX], tmp_map1);
				}
			}
		}
		
		End_Scene(pD3Device);


		break;

	case GAME2_SCENE:

		Draw_Ready(pD3Device);

		if (isGame2_start == true)
		{
			FILE*  fp;
			fopen_s(&fp, "Game2.csv", "r");

			for (int i = 0; i < MAP_HEIGHT; i++)
			{
				for (int j = 0; j < MAP_WIDTH; j++)
				{
					fscanf_s(fp, "%d,", &map[i][j], _countof(map));
				}
			}

			isGame2_start = false;
		}



		/*背景の描画処理*/
		CUSTOMVERTEX background_GAME2[4];

		for (int i = 0; i < 4; i++)
		{
			background_GAME2[i] = background[i];
		}

		Tex_Set_Draw(pD3Device, pTexture[GAME_BACKGROUND_TEX], background_GAME2);



		/*マップの描画処理*/
		CUSTOMVERTEX tmp_map2[4];

		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{

				for (int i = 0; i < 4; i++)
				{
					tmp_map2[i] = map_tip[i];
				}

				for (int i = 0; i < 4; i++)
				{
					tmp_map2[i].x += (x * 32);
					tmp_map2[i].y += (y * 32);
				}


				if (map[y][x] == 1)
				{
					tmp_map2[0].tu = 0.25;
					tmp_map2[0].tv = 0.0;
					tmp_map2[1].tu = 0.5;
					tmp_map2[1].tv = 0.0;
					tmp_map2[2].tu = 0.5;
					tmp_map2[2].tv = 1.0;
					tmp_map2[3].tu = 0.25;
					tmp_map2[3].tv = 1.0;

					Tex_Set_Draw(pD3Device, pTexture[TEST_TEX], tmp_map2);
				}

				if (map[y][x] == 2)
				{
					tmp_map2[0].tu = 0.5;
					tmp_map2[0].tv = 0.0;
					tmp_map2[1].tu = 0.75;
					tmp_map2[1].tv = 1.0;
					tmp_map2[2].tu = 0.75;
					tmp_map2[2].tv = 1.0;
					tmp_map2[3].tu = 0.5;
					tmp_map2[3].tv = 1.0;

					Tex_Set_Draw(pD3Device, pTexture[TEST_TEX], tmp_map2);
				}
			}
		}



		End_Scene(pD3Device);


		break;

	case RESULT_SCENE:


	

		break;

	case GAMEOVER_SCENE:

	




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
		Key_Check_Dinput(&Key[ESC], DIK_ESCAPE);



		if (Key[Z] == PUSH)
		{
			current_scene = TITLE_SCENE;
		}

		if (Key[ESC] == PUSH)
		{
			Free_DX();
			exit(1);
		}

		break;

	case TITLE_SCENE:

		Key_Check_Dinput(&Key[Z], DIK_Z);
		Key_Check_Dinput(&Key[UP], DIK_UP);
		Key_Check_Dinput(&Key[DOWN], DIK_DOWN);
		Key_Check_Dinput(&Key[ESC], DIK_ESCAPE);


		if (Key[DOWN] == PUSH && isTutorial == false && isOption == false)
		{
			if (title_select_cursor == 2)
			{
				title_select_cursor = 0;
			}
			else
			{
				title_select_cursor += 1;
			}
		}

		if (Key[UP] == PUSH  && isTutorial == false && isOption == false)
		{
			if (title_select_cursor == 0)
			{
				title_select_cursor = 2;
			}
			else
			{
				title_select_cursor -= 1;
			}
		}


		
		if (Key[Z] == PUSH && isTutorial == false && isOption == false)
		{
			if (title_select_cursor == 0)
			{
				current_scene = STAGE_SELECT_SCENE;
			}


			if (title_select_cursor == 1)
			{
				isTutorial = true;
			}

			if (title_select_cursor == 2)
			{
				isOption = true;
			}
		}
		else if (Key[Z] == PUSH && isTutorial == true)
		{
			isTutorial = false;
		}
		else if (Key[Z] == PUSH && isOption == true)
		{
			isOption = false;
		}

		



		if (Key[ESC] == PUSH)
		{
			Free_DX();
			exit(1);
		}


		break;

	case STAGE_SELECT_SCENE:

		Key_Check_Dinput(&Key[RIGHT], DIK_RIGHT);
		Key_Check_Dinput(&Key[LEFT], DIK_LEFT);
		Key_Check_Dinput(&Key[ESC], DIK_ESCAPE);
		Key_Check_Dinput(&Key[Z], DIK_Z);



		if (Key[LEFT] == PUSH)
		{
			if (stage_select_cursor != 0)
			{
				stage_select_cursor--;
			}

			stage_left_direction = true;

		}

		if (Key[RIGHT] == PUSH)
		{
			if (stage_select_cursor != STAGE_MAX)
			{
				stage_select_cursor++;
			}
			stage_left_direction = false;

		}

		if (stage_select_char[0].x < (150 + (stage_select_cursor * 850)))
		{
			for (int i = 0; i < 4; i++)
			{
				stage_select_char[i].x += 5;
			}
		}
		else if (stage_select_char[0].x >(150 + (stage_select_cursor * 850)))
		{
			for (int i = 0; i < 4; i++)
			{
				stage_select_char[i].x -= 5;
			}
		}

		if (Key[Z] == PUSH && stage_select_cursor == 0 && stage_select_char[0].x == 150)
		{
			current_scene = GAME1_SCENE;
			isGame1_start = true;
		}

		if (Key[Z] == PUSH && stage_select_cursor == 1 && stage_select_char[0].x == 1000)
		{
			current_scene = GAME2_SCENE;
			isGame2_start = true;
		}




		if (Key[ESC] == PUSH)
		{
			Free_DX();
			exit(1);
		}
		//DefWindowProc

		break;

	case GAME1_SCENE:


		Key_Check_Dinput(&Key[Z], DIK_Z);
		Key_Check_Dinput(&Key[X], DIK_X);

		if (Key[X] == PUSH)
		{
			current_scene = STAGE_SELECT_SCENE;
		}
	


		break;


	case GAME2_SCENE:


		Key_Check_Dinput(&Key[Z], DIK_Z);
		Key_Check_Dinput(&Key[X], DIK_X);

		if (Key[X] == PUSH)
		{
			current_scene = STAGE_SELECT_SCENE;
		}



		break;

	case RESULT_SCENE:

		Key_Check_Dinput(&Key[Z], DIK_Z);




		break;

		
	case GAMEOVER_SCENE:

		Key_Check_Dinput(&Key[Z], DIK_Z);




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