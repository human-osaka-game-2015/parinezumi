#include <dinput.h>



#ifndef _KEY_

#define _KEY_

/**
 * KEY�̏�Ԃ𐔒l�ɂ��Ă���B
 */
enum KEYSTATE
{
	PUSH,
	RELEASE,
	ON,
	OFF
};



/**
 * KEY�̎�ނ𐔒l�ɂ��āA�L�[�̏�Ԃ��i�[�ł���悤�ɂ���
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
 * �}�E�X�̏�Ԃ��i�[����\����
 */
typedef struct
{
	bool LEFT, RIGHT;
	int lx, ly, lz;

} MOUSEKIND;



#endif


//------------------------------Directinput------------------------------------




//-----------------�f�o�C�X�̏������֐�-------------------

/**
 *@brief	Dinput�̏������֐�
 *
 *@return	���������ꍇ��S_OK���A���s�����ꍇ�̓G���[�R�[�h��Ԃ��B
 */
HRESULT Init_Dinput();

/**
 *@brief	Dinput�̃L�[�{�[�h�̏�����
 *
 *@param	hWnd �E�B���h�E�n���h��
 *
 *@return	���������ꍇ��S_OK���A���s�����ꍇ�̓G���[�R�[�h��Ԃ��B
 */
HRESULT Init_Dinput_Key(HWND hwnd);

/**
 *@brief	Dinput�̃}�E�X�̏�����
 *
 *@param	hWnd �E�B���h�E�n���h��
 *
 *@return	���������ꍇ��S_OK���A���s�����ꍇ�̓G���[�R�[�h��Ԃ��B
 */
HRESULT Init_Dinput_Mouse(HWND hwnd);





//-------------------�f�o�C�X�̏�ԃ`�F�b�N---------------

/**
 *@brief		Directinput���g�����L�[��Ԃ̃`�F�b�N
 *
 *@param Key	�`�F�b�N�����L�[�̏�Ԃ��i�[����A�h���X
 *@param DIK	�`�F�b�N����L�[��DIK
 */
void Key_Check_Dinput(KEYSTATE* Key, int DIK);

/**
 *@brief		Directinput���g�����}�E�X�̏�ԃ`�F�b�N
 *
 *@param Mouse	�`�F�b�N�����}�E�X�̏�Ԃ��i�[����A�h���X
 */
void Mouse_Check(MOUSEKIND* Mouse);




//--------------------directinput ����֐�-------------------------

/**
 *@brief	Dinput�̃����[�X�֐�
 */
void Release_pDinput();


/**
 *@brief	Dinput�̃L�[�f�o�C�X�̃����[�X�֐�
 */
void Release_Key();


/**
 *@brief	�}�E�X�̉��
 */
void Release_Mouse();




