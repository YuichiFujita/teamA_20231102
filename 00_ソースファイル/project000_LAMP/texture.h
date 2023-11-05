//============================================================
//
//	�e�N�X�`���w�b�_�[ [texture.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_TEXTURE		(256)	// �e�N�X�`���̍ő吔

//************************************************************
//	�N���X��`
//************************************************************
// �e�N�X�`���N���X
class CTexture
{
public:
	// �R���X�g���N�^
	CTexture();

	// �f�X�g���N�^
	~CTexture();

	// �����o�֐�
	int Regist(const char *pFileName);				// �e�N�X�`���o�^
	LPDIRECT3DTEXTURE9 GetTexture(const int nID);	// �e�N�X�`�����擾

	// �ÓI�����o�֐�
	static CTexture *Create(void);					// ����
	static HRESULT Release(CTexture *&pTexture);	// �j��

private:
	// �����o�֐�
	HRESULT Load(void);		// �e�N�X�`������
	void Unload(void);		// �e�N�X�`���j��
	void LoadSetup(void);	// �Z�b�g�A�b�v

	// �����o�ϐ�
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];	// �e�N�X�`���ւ̃|�C���^
	char m_pFileName[MAX_TEXTURE][MAX_FILENAME];	// �ǂݍ��񂾃e�N�X�`���t�@�C����

	// �ÓI�����o�ϐ�
	static int m_nNumAll;	// �e�N�X�`���̑���
};

#endif	// _TEXTURE_H_
