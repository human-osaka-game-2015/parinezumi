#include "Tex_lib.h"
#include "Main.h"
#include "directinput_lib.h"


extern IDirect3DDevice9* pD3Device;
extern CUSTOMVERTEX background[4];
extern CUSTOMVERTEX TITLE_select[4];
extern LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];
extern KEYSTATE Key[KEYMAX];
extern int current_scene;

extern CUSTOMVERTEX black_background[4];


bool Logo_change_flag = false;



void Logo_Render()
{
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

}

void Logo_Control()
{
	Key_Check_Dinput(&Key[Z], DIK_Z);
	Key_Check_Dinput(&Key[ESC], DIK_ESCAPE);



	if (Key[Z] == PUSH)
	{
		current_scene = TITLE_SCENE;
	}

}

