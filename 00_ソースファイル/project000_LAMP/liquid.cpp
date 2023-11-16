//============================================================
//
//	�t�̏��� [liquid.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "liquid.h"
#include "manager.h"
#include "renderer.h"
#include "scrollMeshField.h"

//************************************************************
//	�}�N����`
//************************************************************
#define LIQUID_PRIO	(0)		// �t�̗̂D�揇��
#define ADD_ROT		(80)	// ���_���Ƃ̃J�[�u���Z��
#define ADD_UP		(10.0f)	// �g�̍ō��_

#define ADD_SINROT	(D3DXToRadian(0.5f))	// ���t���[���̔g�ł��������Z��

#define LIQUID_DOWN	(D3DXVECTOR2(-0.0f, 0.002f))	// �t��(��)�̈ړ���
#define LIQUID_UP	(D3DXVECTOR2(-0.0f, 0.0001f))	// �t��(��)�̈ړ���

#define LIQUID_ROT	(D3DXVECTOR3(0.0f, D3DXToRadian(45), 0.0f))	// ����
#define LIQUID_SIZE	(D3DXVECTOR2(24000.0f, 24000.0f))			// �傫��

#define LIQUID_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �F
#define LIQUID_PART	(POSGRID2(128, 128))				// ������

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CLiquid::mc_apTextureFile[][LIQUID_MAX] =	// �e�N�X�`���萔
{
	{ // �C�e�N�X�`��
		"data\\TEXTURE\\sea000.jpg",	// �C(��)
		"data\\TEXTURE\\sea000.png",	// �C(��)
	},
};

//************************************************************
//	�q�N���X [CLiquid] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLiquid::CLiquid() : CObject(CObject::LABEL_LIQUID, LIQUID_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
	m_fSinRot = 0.0f;	// �g�ł�����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLiquid::~CLiquid()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLiquid::Init(void)
{
	// �ϐ��z���錾
	D3DXVECTOR2 aMoveTexU[LIQUID_MAX] =	// �e�N�X�`���̉��ړ��ʂ̐ݒ�p
	{ // �����l
		LIQUID_UP,		// �t��(��)�̈ړ���
		LIQUID_DOWN,	// �t��(��)�̈ړ���
	};

	D3DXCOLOR aCol[LIQUID_MAX] =	// �F�̐ݒ�p
	{ // �����l
		XCOL_WHITE,	// �t��(��)�̐F
		LIQUID_COL,	// �t��(��)�̐F
	};

	// �����o�ϐ���������
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
	m_fSinRot = 0.0f;	// �g�ł�����

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̐���
		m_apLiquid[nCntLiquid] = CScrollMeshField::Create
		( // ����
			aMoveTexU[nCntLiquid].x,	// �����W�̈ړ���
			aMoveTexU[nCntLiquid].y,	// �c���W�̈ړ���
			VEC3_ZERO,		// �ʒu
			LIQUID_ROT,		// ����
			LIQUID_SIZE,		// �傫��
			aCol[nCntLiquid],	// �F
			LIQUID_PART,		// ������
			D3DCULL_CCW,	// �J�����O��
			false			// ���C�e�B���O��
		);
		if (m_apLiquid[nCntLiquid] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apLiquid[nCntLiquid]->BindTexture(mc_apTextureFile[0][nCntLiquid]);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLiquid::Uninit(void)
{
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̏I��
		m_apLiquid[nCntLiquid]->Uninit();
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CLiquid::Update(void)
{
	// �ϐ���錾
	POSGRID2 part = m_apLiquid[0]->GetPattern();	// ������
	D3DXVECTOR3 pos;	// ���_���W�擾�p
	int nLine;			// ���_�̍s
	float fSinRot;		// �T�C���J�[�u�Ɏg�p����p�x

	// ���b�V���̒��_���W�̍X�V
	for (int nCntVtx = 0; nCntVtx < (part.x + 1) * (part.y + 1); nCntVtx++)
	{ // �g�p���_�����J��Ԃ�

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // �t�̂̍ő吔���J��Ԃ�

			// ���_���W���擾
			pos = m_apLiquid[nCntLiquid]->GetMeshVertexPosition(nCntVtx);

			// �g�ł��p�̊p�x�����߂�
			nLine = nCntVtx / (part.x + 1);							// ���_�̍s
			fSinRot = m_fSinRot + D3DXToRadian(nLine * ADD_ROT);	// �T�C���J�[�u�Ɏg�p����p�x

			// �T�C���J�[�u�p�̌�����␳
			useful::NormalizeRot(fSinRot);

			// ���_���W��ݒ�
			m_apLiquid[nCntLiquid]->SetMeshVertexPosition(nCntVtx, D3DXVECTOR3(pos.x, (sinf(fSinRot) * ADD_UP) + ADD_UP, pos.z));
		}
	}

	// �g�ł����������Z
	m_fSinRot += ADD_SINROT;

	// ������␳
	useful::NormalizeRot(m_fSinRot);

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̍X�V
		m_apLiquid[nCntLiquid]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLiquid::Draw(void)
{

}

//============================================================
//	��������
//============================================================
CLiquid *CLiquid::Create(void)
{
	// �|�C���^��錾
	CLiquid *pLiquid = NULL;	// �t�̐����p

	if (pLiquid == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pLiquid = new CLiquid;	// �t��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pLiquid != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �t�̂̏�����
		if (FAILED(pLiquid->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLiquid;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
void CLiquid::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
