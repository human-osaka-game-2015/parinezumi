#include "Tex_lib.h"
#include "Main.h"
#include "directinput_lib.h"


#define MAP_HEIGHT 20
#define MAP_WIDTH 130
#define TIPSIZE 32

extern IDirect3DDevice9* pD3Device;
extern CUSTOMVERTEX background[4];
extern CUSTOMVERTEX TITLE_select[4];
extern LPDIRECT3DTEXTURE9 pTexture[TEX_MAX];
extern KEYSTATE Key[KEYMAX];

extern int current_scene;
extern bool isGame1_start;


extern int map[MAP_HEIGHT][MAP_WIDTH];




bool Map_Hit(int x, int y)
{
	int map_x = x / 32;
	int map_y = y / 32;




	if (map_y < 0)
	{
		map_y = 0;
	}

	
	if (map[map_y][map_x] != 0 && map[map_y][map_x] != 11 && map[map_y][map_x] != 12)
	{
		return true;
	}

	return false;
}

void Map_Search(int x, int y, int* px, int* py)
{
	int map_x = x / 32;
	int map_y = y / 32;


	if (x % 32 != 0)
	{
		x++;
	}

	if (y % 32 != 0)
	{
		y++;
	}

	*px = map_x;
	*py = map_y;

}



