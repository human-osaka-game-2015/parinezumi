#include <dinput.h>



#ifndef _KEY_

#define _KEY_

/**
 * KEYの状態を数値にしている。
 */
enum KEYSTATE
{
	PUSH,
	RELEASE,
	ON,
	OFF
};



/**
 * KEYの種類を数値にして、キーの状態を格納できるようにする
 */
enum KEYKIND
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	ENTER,
	SPACE,
	ESC,
	LSHIFT,
	RSHIFT,
	KEYMAX
};


/**
 * マウスの状態を格納する構造体
 */
typedef struct
{
	bool LEFT, RIGHT;
	int lx, ly, lz;

} MOUSEKIND;



#endif


//------------------------------Directinput------------------------------------




//-----------------デバイスの初期化関数-------------------

/**
 *@brief	Dinputの初期化関数
 *
 *@return	成功した場合はS_OKを、失敗した場合はエラーコードを返す。
 */
HRESULT Init_Dinput();

/**
 *@brief	Dinputのキーボードの初期化
 *
 *@param	hWnd ウィンドウハンドル
 *
 *@return	成功した場合はS_OKを、失敗した場合はエラーコードを返す。
 */
HRESULT Init_Dinput_Key(HWND hwnd);

/**
 *@brief	Dinputのマウスの初期化
 *
 *@param	hWnd ウィンドウハンドル
 *
 *@return	成功した場合はS_OKを、失敗した場合はエラーコードを返す。
 */
HRESULT Init_Dinput_Mouse(HWND hwnd);





//-------------------デバイスの状態チェック---------------

/**
 *@brief		Directinputを使ったキー状態のチェック
 *
 *@param Key	チェックしたキーの状態を格納するアドレス
 *@param DIK	チェックするキーのDIK
 */
void Key_Check_Dinput(KEYSTATE* Key, int DIK);

/**
 *@brief		Directinputを使ったマウスの状態チェック
 *
 *@param Mouse	チェックしたマウスの状態を格納するアドレス
 */
void Mouse_Check(MOUSEKIND* Mouse);




//--------------------directinput 解放関数-------------------------

/**
 *@brief	Dinputのリリース関数
 */
void Release_pDinput();


/**
 *@brief	Dinputのキーデバイスのリリース関数
 */
void Release_Key();


/**
 *@brief	マウスの解放
 */
void Release_Mouse();




