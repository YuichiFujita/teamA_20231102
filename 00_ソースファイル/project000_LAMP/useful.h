//============================================================
//
//	�֗��֐��w�b�_�[ [useful.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _USEFUL_H_
#define _USEFUL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "renderState.h"

//************************************************************
//	�}�N����`
//************************************************************
// �֗��}�N��
#define SCREEN_WIDTH	(1280)	// �E�C���h�E�̉���
#define SCREEN_HEIGHT	(720)	// �E�C���h�E�̏c��
#define MAX_STRING		(128)	// ������̍ő啶����
#define MAX_FILENAME	(128)	// �t�@�C�����̍ő啶����
#define NONE_IDX		(-1)	// �C���f�b�N�X��g�p
#define NONE_STRING		("\0")	// �������g�p

#define PI_TWO_DP		(3.14f)	// �����_���܂ł̉~����
#define HALF_PI_TWO_DP	(1.57f)	// �����_���܂ł̓񕪂̈�̉~���� (��/�Q)

#define HALF_PI	(D3DX_PI * 0.5f)	// �񕪂̈�̉~���� (��/�Q)
#define QRTR_PI	(D3DX_PI * 0.25f)	// �l���̈�̉~���� (��/�S)

// D3DXVECTOR2�֌W
#define VEC2_ZERO	(D3DXVECTOR2(0.0f, 0.0f))	// 0�N���A
#define VEC2_ONE	(D3DXVECTOR2(1.0f, 1.0f))	// 1�N���A
#define VEC2_ALL(f)	(D3DXVECTOR2((f), (f))		// ���l�N���A

// D3DXVECTOR3�֌W
#define VEC3_ZERO	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 0�N���A
#define VEC3_ONE	(D3DXVECTOR3(1.0f, 1.0f, 1.0f))	// 1�N���A
#define VEC3_ALL(f)	(D3DXVECTOR3((f), (f), (f)))	// ���l�N���A

#define SCREEN_CENT	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))	// �E�C���h�E�̒������W
#define SCREEN_SIZE	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))				// �E�C���h�E�̉�ʃT�C�Y

// POSGRID2�֌W
#define GRID2_ZERO	(POSGRID2(0, 0))	// 0�N���A
#define GRID2_ONE	(POSGRID2(1, 1))	// 1�N���A

// POSGRID3�֌W
#define GRID3_ZERO	(POSGRID3(0, 0, 0))	// 0�N���A
#define GRID3_ONE	(POSGRID3(1, 1, 1))	// 1�N���A

// D3DXCOLOR�֌W
#define XCOL_ABLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))	// ��(����)
#define XCOL_BLACK	(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	// ��(�s����)
#define XCOL_AWHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))	// ��(����)
#define XCOL_WHITE	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// ��(�s����)
#define XCOL_ARED	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f))	// ��(����)
#define XCOL_RED	(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	// ��(�s����)
#define XCOL_AGREEN	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f))	// ��(����)
#define XCOL_GREEN	(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	// ��(�s����)
#define XCOL_ABLUE	(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f))	// ��(����)
#define XCOL_BLUE	(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	// ��(�s����)

// RenderState�֌W
#define RS_BL_NORMAL	(CRenderState::SBlendAlpha(D3DBLENDOP_ADD,			D3DBLEND_SRCALPHA,	D3DBLEND_INVSRCALPHA))	// �ʏ탿�u�����h
#define RS_BL_ADD		(CRenderState::SBlendAlpha(D3DBLENDOP_ADD,			D3DBLEND_SRCALPHA,	D3DBLEND_ONE))			// ���Z���u�����h
#define RS_BL_SUB		(CRenderState::SBlendAlpha(D3DBLENDOP_REVSUBTRACT,	D3DBLEND_SRCALPHA,	D3DBLEND_ONE))			// ���Z���u�����h

//************************************************************
//	�\���̒�`
//************************************************************
// 2�������W
struct POSGRID2
{
	// �R���X�g���N�^
	POSGRID2() {}
	POSGRID2(const int nx, const int ny) { x = nx; y = ny; }	// ���

	// �f�X�g���N�^
	~POSGRID2() {}

	// �����o�֐�
	bool operator==(const POSGRID2& grid) const;
	bool operator!=(const POSGRID2& grid) const;

	// �����o�ϐ�
	int x;	// ������W (x)
	int y;	// ������W (y)
};

// 3�������W
struct POSGRID3
{
	// �R���X�g���N�^
	POSGRID3() {}
	POSGRID3(const int nx, const int ny, const int nz) { x = nx; y = ny; z = nz; }	// ���

	// �f�X�g���N�^
	~POSGRID3() {}

	// �����o�֐�
	bool operator==(const POSGRID3& grid) const;
	bool operator!=(const POSGRID3& grid) const;

	// �����o�ϐ�
	int x;	// ������W (x)
	int y;	// ������W (y)
	int z;	// ������W (z)
};

//************************************************************
//	���O��Ԑ錾
//************************************************************
// �֗��֐����
namespace useful
{
	// �ʏ�֐�
	void DivideDigitNum		// ���l�̌������Ƃ̕���
	( // ����
		int *pNumDivide,	// �������ʂ̊i�[�z��
		const int nNum,		// �������鐔�l
		const int nMaxDigit	// �������鐔���̌���
	);
	void NormalizeNormal	// �@���̐��K��
	( // ����
		const D3DXVECTOR3& rPosLeft,	// ���ʒu
		const D3DXVECTOR3& rPosCenter,	// ���S�ʒu
		const D3DXVECTOR3& rPosRight,	// �E�ʒu
		D3DXVECTOR3& rNor				// �@��
	);
	void NormalizeRot(float& rRot);				// �����̐��K��
	void Vec3NormalizeRot(D3DXVECTOR3& rRot);	// �O�������̐��K��

	// �e���v���[�g�֐�
	template<class T> bool LimitNum	// �l�͈͓̔�����
	( // ����
		T& rNum,		// �������l
		const T min,	// �ŏ��͈�
		const T max		// �ő�͈�
	);
	template<class T> void SortNum	// �l�̃\�[�g
	( // ����
		T *pSortNum,		// �\�[�g�z��
		const int nMaxKeep	// �z��T�C�Y
	);
	template<class T> void Shuffle	// �V���b�t��
	(
		T *pShuffleData,		// �V���b�t���z��
		const int nMaxShuffle	// �z��T�C�Y
	);
}

// �}�e���A�����
namespace material
{
	// �ʏ�֐�
	D3DXMATERIAL Red(void);			// �ԃ}�e���A���擾
	D3DXMATERIAL GlowGreen(void);	// �����΃}�e���A���擾
}

//************************************************************
//	�e���v���[�g�֐�
//************************************************************
//	�֗��֐����
//************************************************************
//============================================================
//	�l�͈͓̔���������
//============================================================
template<class T> bool useful::LimitNum
(
	T& rNum,		// �������l
	const T min,	// �ŏ��͈�
	const T max		// �ő�͈�
)
{
	if (rNum < min)
	{ // �������l���ŏ��͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = min;

		// �^��Ԃ�
		return true;
	}
	else if (rNum > max)
	{ // �������l���ő�͈͂𒴂��Ă����ꍇ

		// �͈͓��ɕ␳
		rNum = max;

		// �^��Ԃ�
		return true;
	}

	// �U��Ԃ�
	return false;
}

//============================================================
//	�\�[�g����
//============================================================
template<class T> void useful::SortNum
(
	T *pSortNum,		// �\�[�g�z��
	const int nMaxKeep	// �z��T�C�Y
)
{
	// �ϐ���錾
	T	keepNum;		// �\�[�g�p
	int	nCurrentMaxID;	// �ő�l�̃C���f�b�N�X

	for (int nCntKeep = 0; nCntKeep < (nMaxKeep - 1); nCntKeep++)
	{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nCurrentMaxID = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < nMaxKeep; nCntSort++)
		{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

			if (pSortNum[nCurrentMaxID] < pSortNum[nCntSort])
			{ // �ő�l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����傫���ꍇ

				// ���݂̗v�f�ԍ����ő�l�ɐݒ�
				nCurrentMaxID = nCntSort;
			}
		}

		if (nCntKeep != nCurrentMaxID)
		{ // �ő�l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �v�f�̓���ւ�
			keepNum					= pSortNum[nCntKeep];
			pSortNum[nCntKeep]		= pSortNum[nCurrentMaxID];
			pSortNum[nCurrentMaxID]	= keepNum;
		}
	}
}

//============================================================
//	�V���b�t������
//============================================================
template<class T> void useful::Shuffle
(
	T *pShuffleData,		// �V���b�t���z��
	const int nMaxShuffle	// �z��T�C�Y
)
{
	for (int nCntShu = nMaxShuffle - 1; nCntShu > 0; nCntShu--)
	{ // �����̑������J��Ԃ�

		// �ϐ���錾
		T swap;		// �v�f�̓���ւ��p
		int nRandom = rand() % nCntShu;	// �v�f�����ւ���C���f�b�N�X

		// �v�f�̓���ւ�
		swap					= pShuffleData[nRandom];
		pShuffleData[nRandom]	= pShuffleData[nCntShu];
		pShuffleData[nCntShu]	= swap;
	}
}

#endif	// _USEFUL_H_
