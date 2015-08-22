#include "Tex_lib.h"
#include "Main.h"
#include "directinput_lib.h"


extern IDirect3DDevice9* pD3Device;
extern CUSTOMVERTEX background[4];
extern CUSTOMVERTEX TITLE_select[4];
extern LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];
extern KEYSTATE Key[KEYMAX];
extern int current_scene;


int title_select_cursor = 0;
int title_select_option_cursor = 0;


bool isTutorial = false;
bool isOption = false;



void Title_Render()
{
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
			{ 0.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f },
			{ 1280.0f, 0.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f },
			{ 1280.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f },
			{ 0.0f, 720.0f, 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f },
		};

		Tex_Set_Draw(pD3Device, pTexture[TUTORIAL_SCENE_TEX], tutorial);
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
}






void Title_Control()
{
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






}


