#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9tex.h>
#include <stdio.h>

/*自作ライブラリ*/
#include "directinput_lib.h" 
#include "Tex_lib.h"
#include "directx.h"
#include "Main.h"
#include "Render_Control.h"
#include "Game.h"

/*自作ヘッダ*/



/*メモリリーク検出用関数を使うためのヘッダー*/
#include <crtdbg.h>

/*define*/
#define TITLE 	TEXT("ハリネズミ　ケンジ君の大冒険!!")
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
#define PI 3.1415926535
#define MAP_HEIGHT 20
#define MAP_WIDTH 300
#define TIPSIZE 32
#define GRAVITY 9.8




/*enum*/




/*directxのオブジェクト*/
IDirect3D9*	pDirect3D;
IDirect3DDevice9*	pD3Device;
LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];


/*グローバル変数*/
int current_scene = LOGO_SCENE;

int map[MAP_HEIGHT][MAP_WIDTH];
int heart_num = 5;
int map_error = 0;

unsigned int game_time = 0;





bool isGame1_start = false;
bool isGame2_start = false;



double fall_time = 0.0;



/**ジャンプ系の変数
 *v0はジャンプの初速
 *
 *
 *
 */
float v0;
bool isjump_flag = false;
int jump_time = 0;






//Game1

//Sunder系の変数
/**
 *cooltime_Sunderは雷のクールタイム
 *sunder_timeは雷の描画時間
 *isSunder_flagは雷の描画フラグ
 *use_sunderは雷を何度使ったかを格納する変数　スコアで使う
 */
int cooltime_Thunder = 0;
int Thunder_time = 0;
bool isThunder_flag = false;
int use_Thunder = 0;


/**
 *cooltime_Windは風のクールタイム
 *Wind_timeは風の描画時間
 *isWind_flagは風の描画フラグ
 *use_Windは風を何度使ったかを格納する変数　スコアで使う
 */
int cooltime_Wind = 0;
int Wind_time = 0;
bool isWind_flag = false;
int use_Wind = 0;


//神様行動をしているかのフラグ
//trueだったら、神様行動は行えないようにする
bool Effect_flag = false;





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

CUSTOMVERTEX player_chara[] =
{
	{ 0.0f,   150.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f,  0.0f },
	{ 100.0f, 150.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ 100.0f, 250.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ 0.0f,   250.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f,  1.0f },
};

//ゲームUI 
CUSTOMVERTEX game_ui[] = 
{
	{ 0.0f,    540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f,  0.0f },
	{ 1280.0f, 540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ 0.0f,    720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f,  1.0f },
};

CUSTOMVERTEX game_ui_apple[] =
{
	{ 0.0f,  540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 60.0f, 540.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 60.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f,  600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX game_ui_heart[] =
{
	{ 0.0f,  600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 60.0f, 600.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 60.0f, 660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f,  660.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX game_ui_hand[]
{
	{ 190.0f,  0.0f,  0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 280.0f, 0.0f,  0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 280.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 190.0f,  60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};


CUSTOMVERTEX thunder[] =
{
	{ 0.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 60.0f, 60.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 60.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX wind[] =
{
	{ 0.0f, 400.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 200.0f, 400.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 200.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
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


	Tex_Load("Tex/LOGO_test.png", &pTexture[LOGO_TEX],pD3Device);
	Tex_Load("Tex/TITLE_test.png", &pTexture[TITLE_BACKGROUND_TEX], pD3Device);
	Tex_Load("Tex/STAGE_SELECT_test.png", &pTexture[STAGE_SELECT_TEX], pD3Device);
	Tex_Load("Tex/Test.bmp", &pTexture[TEST_TEX], pD3Device);
	Tex_Load("Tex/GAME.png", &pTexture[GAME_BACKGROUND_TEX], pD3Device);
	Tex_Load("Tex/GAMECLEAR.png", &pTexture[RESULT_TEX], pD3Device);
	Tex_Load("Tex/GAMEOVER.png", &pTexture[GAMEOVER_TEX], pD3Device);
	Tex_Load("Tex/black.png", &pTexture[BLACK_TEX], pD3Device);
	Tex_Load("Tex/STAGE_SELECT_test.png", &pTexture[STAGE_SELECT_BACKGROUND_TEX],pD3Device);
	Tex_Load("Tex/option_scene.png", &pTexture[OPTION_SCENE_TEX], pD3Device);
	Tex_Load("Tex/tutorial_scene.png", &pTexture[TUTORIAL_SCENE_TEX], pD3Device);
	Tex_Load("Tex/main_game_ui.png", &pTexture[GAME_UI_TEX], pD3Device);
	
	
	Tex_Load_EX("Tex/stage_select_map.png", &pTexture[STAGE_SELECT_TEX], pD3Device,255,255,255,255);
	Tex_Load_EX("Tex/start.png", &pTexture[START_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/tutorial.png", &pTexture[TUTORIAL_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/option.png", &pTexture[OPTION_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/title_ui.png", &pTexture[UI_TEX], pD3Device, 255, 255, 255, 255);

	//神様エフェクト＆UI
	Tex_Load_EX("Tex/G_01_02.png", &pTexture[G_01_02_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/G_01_04.png", &pTexture[G_01_04_TEX], pD3Device, 255, 255, 255, 255);

	Tex_Load_EX("Tex/G_test.png", &pTexture[HUND_UI_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/heart.png", &pTexture[UI_HEART_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/apple_test.png", &pTexture[UI_APPLE_TEX], pD3Device, 255, 0, 0, 0);



	//G_01_02.png


	//ハリネズミ
	Tex_Load_EX("Tex/C_01_01.png", &pTexture[C_01_01_TEX], pD3Device, 255, 255, 255, 255);
	Tex_Load_EX("Tex/C_01_02.png", &pTexture[C_01_02_TEX], pD3Device, 255, 255, 255, 255);




	//初期化関数作るのもいいかも
		
	

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
		
		Logo_Render();

		break;


	case TITLE_SCENE:

		Title_Render();

		break;


	case STAGE_SELECT_SCENE:

		Stage_Select_Render();

		break;


	case GAME1_SCENE:

		Draw_Ready(pD3Device);


		game_time++;
		if (game_time == 30000)
		{
			game_time = 0;
		}

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



		Tex_Set_Draw(pD3Device, pTexture[HUND_UI_TEX], game_ui_hand);


		//雷描画処理
		if (isThunder_flag == true)
		{
			for (int i = 0; i < 4; i++)
			{
				thunder[i].x = game_ui_hand[i].x + 8;
			}
			Tex_Set_Draw(pD3Device, pTexture[G_01_02_TEX], thunder);

			Thunder_time++;

			if (Thunder_time == 30)
			{
				Effect_flag = false;
				isThunder_flag = false;
				Thunder_time =0;
			}
		}



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

		

		
		
		

		Tex_Set_Draw(pD3Device, pTexture[GAME_UI_TEX], game_ui);


		CUSTOMVERTEX  temp_ui_heart[4];
		CUSTOMVERTEX  temp_ui_apple[4];

		//UIの描画　体力　腹減り度
		for (int i = 0; i < heart_num; i++)
		{
			for (int n = 0; n < 4; n++)
			{
				temp_ui_heart[n] = game_ui_heart[n];
				temp_ui_heart[n].x += (i * 60);

				temp_ui_apple[n] = game_ui_apple[n];
				temp_ui_apple[n].x += (i * 60);

			}

			Tex_Set_Draw(pD3Device, pTexture[UI_HEART_TEX], temp_ui_heart);
			Tex_Set_Draw(pD3Device, pTexture[UI_APPLE_TEX], temp_ui_apple);
		}



		//通常時のハリネズミ描画
		//念のために分けてる
		if (isjump_flag == false && jump_time == 0)
		{
			Tex_Set_Draw(pD3Device, pTexture[C_01_01_TEX], player_chara);

		}
		else if (isjump_flag == false && jump_time != 0)
		{
			//寝転がってる時間
			jump_time--;

			


			//寝転る時間が０になったらtutvを通常時のものに
			if (jump_time == 0)
			{
				if (player_chara[0].tu < player_chara[1].tu)
				{
					player_chara[0].tu = 0.0f;
					player_chara[1].tu = 0.25f;
					player_chara[2].tu = 0.25f;
					player_chara[3].tu = 0.0f;
					Tex_Set_Draw(pD3Device, pTexture[C_01_01_TEX], player_chara);

				}
				else
				{
					player_chara[0].tu = 0.25f;
					player_chara[1].tu = 0.0f;
					player_chara[2].tu = 0.0f;
					player_chara[3].tu = 0.25f;
					Tex_Set_Draw(pD3Device, pTexture[C_01_01_TEX], player_chara);

				}
			}
			else
			{

				if (player_chara[0].tu < player_chara[1].tu)
				{
					player_chara[0].tu = 0.5f;
					player_chara[1].tu = 1.0f;
					player_chara[2].tu = 1.0f;
					player_chara[3].tu = 0.5f;
					Tex_Set_Draw(pD3Device, pTexture[C_01_02_TEX], player_chara);
				}
				else
				{
					player_chara[0].tu = 1.0f;
					player_chara[1].tu = 0.5f;
					player_chara[2].tu = 0.5f;
					player_chara[3].tu = 1.0f;
					Tex_Set_Draw(pD3Device, pTexture[C_01_02_TEX], player_chara);
				}
			}
		}

		
	
		
		
		
		//ジャンプのフラグが立った時の処理
		if (isjump_flag == true)
		{

			//ジャンプ中の描画
			if (player_chara[0].tu < player_chara[1].tu)
			{
				player_chara[0].tu = 0.0f;
				player_chara[1].tu = 0.5f;
				player_chara[2].tu = 0.5f;
				player_chara[3].tu = 0.0f;
			}
			else
			{
				player_chara[0].tu = 0.5f;
				player_chara[1].tu = 0.0f;
				player_chara[2].tu = 0.0f;
				player_chara[3].tu = 0.5f;
			}

					
			Tex_Set_Draw(pD3Device, pTexture[C_01_02_TEX], player_chara);
						
			
			for (int i = 0; i < 4; i++)
			{
				player_chara[i].y += v0;
			}
			
			v0 += 1.0;

			//着地時の処理
			if (v0 > 0)
			{	
				
				jump_time = 10;
				isjump_flag = false;
			}
		}
		

		if (Map_Hit(player_chara[2].x - 15 - map_error, player_chara[2].y) == false && Map_Hit(player_chara[3].x+15 - map_error, player_chara[3].y) == false)
		{
			static float hoge[4];
			for (int i = 0; i < 4; i++)
			{
				hoge[i] = player_chara[i].y;
			}
		



			fall_time += 1.0/13.0;

			if (Map_Hit(player_chara[2].x - 15 - map_error, (hoge[2] + GRAVITY * fall_time * fall_time)) == true)
			{
				int px, py;
				float temp;

				Map_Search(player_chara[2].x - 15 - map_error, (hoge[2] + GRAVITY * fall_time * fall_time), &px, &py);

				temp = player_chara[2].y - (py * 32);

				for (int i = 0; i < 4; i++)
				{
					player_chara[i].y -= temp;
				}

			}
			else if (Map_Hit(player_chara[3].x + 15 - map_error, (hoge[3] + GRAVITY * fall_time * fall_time)) == true)
			{
				int px, py;
				float temp;

				Map_Search(player_chara[3].x + 15 - map_error, (hoge[3] + GRAVITY * fall_time * fall_time), &px, &py);

				temp = player_chara[3].y - (py * 32);

				for (int i = 0; i < 4; i++)
				{
					player_chara[i].y -= temp;
				}
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					player_chara[i].y = hoge[i] + GRAVITY * fall_time * fall_time;
				}
			}
			
		}
		else
		{
			
			fall_time = 0;
		}
		

		/*
		if (game_ui_hand[1].x > 1200 && game_ui_hand[1].x < 1270)
		{
			for (int i = 0; i < 4; i++)
			{
				map_tip[i].x-=3;
				player_chara[i].x-=3;
			}
			map_error -= 3;
		}
		else if (game_ui_hand[0].x < 80 && game_ui_hand[0].x > 10)
		{
			for (int i = 0; i < 4; i++)
			{
				map_tip[i].x+=3;
				player_chara[i].x+=3;
			}
			map_error += 3;
		}
		else */
		
		if (game_ui_hand[1].x > 1130)
		{
			for (int i = 0; i < 4; i++)
			{
				map_tip[i].x -= game_ui_hand[1].x - 1130;
				player_chara[i].x -= game_ui_hand[1].x -1130;
			}
			map_error -= game_ui_hand[1].x - 1130;
		}
		else if (game_ui_hand[0].x < 100)
		{
			for (int i = 0; i < 4; i++)
			{
				map_tip[i].x += 100 - game_ui_hand[0].x;
				player_chara[i].x += 100 - game_ui_hand[0].x;
			}
			map_error += 100 - game_ui_hand[0].x;
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

		Logo_Control();
		

		break;


	case TITLE_SCENE:

		Title_Control();


		break;


	case STAGE_SELECT_SCENE:

		Stage_Select_Control();

		break;


	case GAME1_SCENE:


		Key_Check_Dinput(&Key[Z], DIK_Z);
		Key_Check_Dinput(&Key[X], DIK_X);
		Key_Check_Dinput(&Key[S], DIK_S);
		Key_Check_Dinput(&Key[RIGHT], DIK_RIGHT);
		Key_Check_Dinput(&Key[LEFT], DIK_LEFT);
		Key_Check_Dinput(&Key[UP], DIK_UP);
		Key_Check_Dinput(&Key[J], DIK_J);
		Key_Check_Dinput(&Key[K], DIK_K);
		Key_Check_Dinput(&Key[L], DIK_L);
		Key_Check_Dinput(&Key[I], DIK_I);





		if (jump_time == 0)
		{
			if (Key[L] == ON)
			{
				if (player_chara[0].tu > player_chara[1].tu)
				{
					float tmp_tu;

					tmp_tu = player_chara[0].tu;
					player_chara[0].tu = player_chara[1].tu;
					player_chara[1].tu = tmp_tu;

					tmp_tu = player_chara[2].tu;
					player_chara[2].tu = player_chara[3].tu;
					player_chara[3].tu = tmp_tu;
				}

				//右上と右下のあたり判定
				if (Map_Hit(player_chara[2].x - map_error, player_chara[2].y - 20) == false && Map_Hit(player_chara[1].x - map_error, player_chara[1].y) == false)
				{
					for (int i = 0; i < 4; i++)
					{
						player_chara[i].x += 5;

						if (game_time % 4 == 0)
						{
							player_chara[i].tu += 0.25f;
						}
					}
				}

				if (player_chara[0].tu == 1.0f)
				{
					player_chara[0].tu = 0.0f;
					player_chara[1].tu = 0.25f;
					player_chara[2].tu = 0.25f;
					player_chara[3].tu = 0.0f;
				}
			}


			if (Key[J] == ON)
			{
				if (player_chara[0].tu < player_chara[1].tu)
				{
					float tmp_tu;

					tmp_tu = player_chara[0].tu;
					player_chara[0].tu = player_chara[1].tu;
					player_chara[1].tu = tmp_tu;

					tmp_tu = player_chara[2].tu;
					player_chara[2].tu = player_chara[3].tu;
					player_chara[3].tu = tmp_tu;
				}

				if (Map_Hit(player_chara[0].x - map_error, player_chara[0].y) == false && Map_Hit(player_chara[3].x - map_error, player_chara[3].y - 20) == false)
				{
					for (int i = 0; i < 4; i++)
					{
						player_chara[i].x -= 5;

						if (game_time % 4 == 0)
						{
							player_chara[i].tu += 0.25f;
						}
					}
				}



				if (player_chara[1].tu == 1.0f)
				{
					player_chara[0].tu = 0.25f;
					player_chara[1].tu = 0.0f;
					player_chara[2].tu = 0.0f;
					player_chara[3].tu = 0.25f;
				}
			}



			if (Key[I] == PUSH)
			{
				if ((Map_Hit(player_chara[3].x + 15 - map_error, player_chara[3].y) == true || Map_Hit(player_chara[2].x - 15- map_error, player_chara[2].y) == true))
				{
					isjump_flag = true;
					v0 = -20;
				}
			}
		}
		



		if (Key[RIGHT] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				game_ui_hand[i].x += 8;
			}
		}
		

		if (Key[LEFT] == ON)
		{
			for (int i = 0; i < 4; i++)
			{
				game_ui_hand[i].x -= 8;
			}
		}


		

		
		if (Key[S] == PUSH && isThunder_flag == false && cooltime_Thunder == 120 && Effect_flag == false)
		{
			Effect_flag = true;
			isThunder_flag = true;
			cooltime_Thunder = 0;
			use_Thunder++;
		}
		else if (cooltime_Thunder != 120)
		{
			cooltime_Thunder++;
		}
		
		

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