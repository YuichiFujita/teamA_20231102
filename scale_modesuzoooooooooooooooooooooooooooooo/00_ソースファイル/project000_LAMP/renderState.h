//============================================================
//
//	�����_�[�X�e�[�g�w�b�_�[ [renderState.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����_�[�X�e�[�g�N���X
class CRenderState
{
public:
	// �R���X�g���N�^
	CRenderState();

	// �f�X�g���N�^
	~CRenderState();

	// ���u�����h���
	struct SBlendAlpha
	{
	public:
		// �R���X�g���N�^
		SBlendAlpha() {}
		SBlendAlpha(const D3DBLENDOP Op, const D3DBLEND Scr, const D3DBLEND Dest)
		{ op = Op; scr = Scr; dest = Dest; bBlend = true; }

		// �f�X�g���N�^
		~SBlendAlpha() {}

		// �����o�ϐ�
		D3DBLENDOP	op;		// ���u�����h���
		D3DBLEND	scr;	// SCR�u�����h
		D3DBLEND	dest;	// DEST�u�����h
		bool		bBlend;	// ���u�����h��
	};

	// ���e�X�g���
	struct STestAlpha
	{
		D3DCMPFUNC	func;	// ���e�X�g���
		int			nRef;	// ���e�X�g�Q�ƒl
		bool		bTest;	// ���e�X�g��
	};

	// Z�e�X�g���
	struct STestZ
	{
		D3DCMPFUNC	func;		// Z�e�X�g���
		bool		bUpdate;	// Z�o�b�t�@�X�V��
	};

	// �����_�[�X�e�[�g���
	struct SInfo
	{
		SBlendAlpha	blendAlpha;	// ���u�����h���
		STestAlpha	testAlpha;	// ���e�X�g���
		STestZ		testZ;		// Z�e�X�g���
		D3DCULL		cull;		// �J�����O���
		bool		bLight;		// ���C�e�B���O��
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Set(void);		// �ݒ�
	void Reset(void);	// �Đݒ�

	// �ÓI�����o�֐�
	static CRenderState *Create(void);	// ����
	static void InitRenderState(const LPDIRECT3DDEVICE9 &rDevice);	// �����_�[�X�e�[�g��񏉊���

private:
	// �����o�֐�
	static void SetRenderState(const LPDIRECT3DDEVICE9 &rDevice, const SInfo info);	// �����_�[�X�e�[�g�ݒ�

	// �ÓI�����o�ϐ�
	static SInfo m_save;	// �ۑ������_�[�X�e�[�g���

	// �����o�ϐ�
	SInfo m_info;	// �����_�[�X�e�[�g���
};

#endif	// _RENDER_STATE_H_
