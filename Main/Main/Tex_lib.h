#include <dinput.h>
#include <d3dx9tex.h>




#ifndef _VERTEX_
#define _VERTEX_

typedef struct _CUSTOMVERTEX
{
	float           x, y, z;                //�ʒu���
	float           rhw;                    //���_�ϊ��l
	D3DCOLOR        color;                  //���_�J���[
	float           tu, tv;                 //�e�N�X�`�����W
} CUSTOMVERTEX, *LPCUSTOMVERTEX;

#endif


/**
 *@brief Texture�����[�h����֐�
 *
 *@param name		���[�h����摜�t�@�C���̖��O
 *@param pTexture	�摜�����[�h����ꏊ
 *@param pD3Device	�f�o�C�X�̃I�u�W�F�N�g
 *
 *@return ���������ꍇ��0���A���s�����ꍇ��1��Ԃ��B
 */
int Tex_Load(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device);

/**
 *@brief			Texture�����[�h����֐��@����������
 *
 *@param name		���[�h����摜�t�@�C���̖��O
 *@param pTexture	�摜�����[�h����ꏊ
 *@param pD3Device	�f�o�C�X�̃I�u�W�F�N�g
 *@param alpha		�����F�̃A���t�@
 *@param red		�����F�̐�
 *@param grren		�����F�̗�
 *@param blue		�����F�̐�
 *
 *@return			���������ꍇ��0���A���s�����ꍇ��1��Ԃ��B 
 */
int Tex_Load_EX(const char* name, LPDIRECT3DTEXTURE9* pTexture, IDirect3DDevice9* pD3Device, int alpha, int red, int grren, int blue);



/**
 *@brief		�摜�̊g�k�̊֐�
 *
 *@param Vertex	�g�k����摜�̃p�����[�^�[
 *@param x		x���̊g�k�̔{��
 *@param y		y���̊g�k�̔{��
 */
void Tex_Scaling(CUSTOMVERTEX* Vertex, double x, double y);

/**
 *@brief		��]�̊֐�
 *
 *@param TEX	��]����摜�̃p�����[�^�[
 *@param angle	�}�C�i�X������]�E�v���X���E��]
 */
void Tex_Turn(CUSTOMVERTEX* Vertex, double angle);


/**
 *@brief	�摜�̃Z�b�g�֐�
 *
 *@param	pD3Device �`�悷�邽�߂Ƀf�o�C�X�̃I�u�W�F�N�g
 *@param	pTexture �摜���Z�b�g����֐�
 */
void Tex_Set(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture);

/**
 *@brief	�摜�̕`��֐�
 *
 *@param	pD3Device �`�悷�邽�߂Ƀf�o�C�X�̃I�u�W�F�N�g
 *@param	Vertex �\������摜�̃p�����[�^�[��n���B
 */
void Tex_Draw(IDirect3DDevice9* pD3Device, CUSTOMVERTEX* Vertex);

/**
 *@brief	�摜�̃Z�b�g&�`��֐�
 *
 *@param	pD3Device �`�悷�邽�߂Ƀf�o�C�X�̃I�u�W�F�N�g
 *@param	Vertex �\������摜�̃p�����[�^�[��n���B
 *@param	pTexture �摜���Z�b�g����֐�
 */
void Tex_Set_Draw(IDirect3DDevice9* pD3Device, LPDIRECT3DTEXTURE9 pTexture, CUSTOMVERTEX* Vertex);


/**
 *@brief	�`�悷�邽�߂̏����֐�
 *
 *@param	pD3Device �f�o�C�X�̃I�u�W�F�N�g��n���B
 */
void Draw_Ready(IDirect3DDevice9* pD3Device);


/**
 *@brief	�`��̏I���֐�
 *
 *@param	pD3Device �f�o�C�X�̃I�u�W�F�N�g��n���B
 */
void End_Scene(IDirect3DDevice9* pD3Device);


//
//void Vertex_tmp(CUSTOMVERTEX* Vertex, CUSTOMVERTEX* tmp);

