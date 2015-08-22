#include "Tex_lib.h"
#include "Main.h"
#include "directinput_lib.h"


#define STAGE_MAX 1


extern IDirect3DDevice9* pD3Device;
extern CUSTOMVERTEX background[4];
extern CUSTOMVERTEX TITLE_select[4];
extern LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];
extern KEYSTATE Key[KEYMAX];
extern int current_scene;


int stage_select_cursor = 0;
unsigned int stage_select_time = 0;

bool stage_left_direction = false;
extern bool isGame1_start;
extern bool isGame2_start;



CUSTOMVERTEX stage_select_char[] =
{
	{ 150.0f, 370.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 250.0f, 370.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 0.0f },
	{ 250.0f, 470.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.25f, 1.0f },
	{ 150.0f, 470.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};

CUSTOMVERTEX stage_select_map[] =
{
	{ 0.0f, 300.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
	{ 1280.0f, 300.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
	{ 1280.0f, 500.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
	{ 0.0f, 500.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
};



void Stage_Select_Render()
{


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
	Tex_Set_Draw(pD3Device, pTexture[C_01_01_TEX], stage_select_char);



	End_Scene(pD3Device);

}




void Stage_Select_Control()
{

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





	//DefWindowProc
}
