//============================================================
//
//	�p�[�e�B�N��2D���� [particle2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "particle2D.h"
#include "effect2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define DMG_MOVE		(3.0f)		// �_���[�W�̈ړ���
#define DMG_SPAWN		(35)		// �_���[�W�̐�����
#define DMG_EFF_LIFE	(120)		// �_���[�W�̎���
#define DMG_SIZE		(50.0f)		// �_���[�W�̑傫��
#define DMG_SUB_SIZE	(1.8f)		// �_���[�W�̔��a�̌��Z��

#define ITEM_MOVE		(2.0f)		// �A�C�e���̈ړ���
#define ITEM_SPAWN		(6)			// �A�C�e���̐�����
#define ITEM_EFF_LIFE	(80)		// �A�C�e���̎���
#define ITEM_SIZE		(30.0f)		// �A�C�e���̑傫��
#define ITEM_SUB_SIZE	(1.2f)		// �A�C�e���̔��a�̌��Z��

#define GETITEM_MOVE		(3.0f)	// �A�C�e���擾�̈ړ���
#define GETITEM_SPAWN		(35)	// �A�C�e���擾�̐�����
#define GETITEM_EFF_LIFE	(120)	// �A�C�e���擾�̎���
#define GETITEM_SIZE		(50.0f)	// �A�C�e���擾�̑傫��
#define GETITEM_SUB_SIZE	(1.8f)	// �A�C�e���擾�̔��a�̌��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const int CParticle2D::mc_aLife[CParticle2D::TYPE_MAX] =	// �����萔
{
	0,	// �Ȃ�
	18,	// �_���[�W
	1,	// �A�C�e��
	2,	// �A�C�e���擾
};

//************************************************************
//	�q�N���X [CParticle2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CParticle2D::CParticle2D() : CObject(CObject::LABEL_PARTICLE)
{
	// �����o�ϐ����N���A
	m_pos	= VEC3_ZERO;	// �ʒu
	m_col	= XCOL_WHITE;	// �F
	m_type	= TYPE_NONE;	// ���
	m_nLife	= 0;			// ����
}

//============================================================
//	�f�X�g���N�^
//============================================================
CParticle2D::~CParticle2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CParticle2D::Init(void)
{
	// �����o�ϐ���������
	m_pos	= VEC3_ZERO;	// �ʒu
	m_col	= XCOL_WHITE;	// �F
	m_type	= TYPE_NONE;	// ���
	m_nLife	= 0;			// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����������
//============================================================
HRESULT CParticle2D::Init(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �����o�ϐ���������
	m_pos	= rPos;				// �ʒu
	m_col	= rCol;				// �F
	m_type	= type;				// ���
	m_nLife	= mc_aLife[type];	// ����

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CParticle2D::Uninit(void)
{
	// �p�[�e�B�N��2D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CParticle2D::Update(void)
{
	if (m_nLife > 0)
	{ // ����������ꍇ

		// ���������Z
		m_nLife--;
	}
	else
	{ // �������Ȃ��ꍇ

		// �p�[�e�B�N��2D�I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	switch (m_type)
	{ // ��ނ��Ƃ̏���
	case TYPE_DAMAGE:

		// �_���[�W�p�[�e�B�N��2D
		Damage(m_pos, m_col);

		break;

	case TYPE_ITEM:

		// �A�C�e���p�[�e�B�N��2D
		Item(m_pos, m_col);

		break;

	case TYPE_GETITEM:

		// �A�C�e���擾�p�[�e�B�N��2D
		GetItem(m_pos, m_col);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CParticle2D::Draw(void)
{

}

//============================================================
//	�_���[�W�p�[�e�B�N��2D����
//============================================================
void CParticle2D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 9 == 0)
	{ // ������9�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = 0.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z = 0.0f;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
			CEffect2D::Create
			( // ����
				rPos,					// �ʒu
				DMG_SIZE,				// ���a
				CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
				DMG_EFF_LIFE,			// ����
				move,					// �ړ���
				rot,					// ����
				rCol,					// �F
				DMG_SUB_SIZE,			// ���a�̌��Z��
				true,					// ���Z������
				LABEL_PARTICLE			// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	�A�C�e���p�[�e�B�N��2D����
//============================================================
void CParticle2D::Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < ITEM_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= ITEM_MOVE;
		move.y *= ITEM_MOVE;
		move.z = 0.0f;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
		CEffect2D::Create
		( // ����
			rPos,					// �ʒu
			ITEM_SIZE,				// ���a
			CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
			ITEM_EFF_LIFE,			// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			ITEM_SUB_SIZE,			// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�A�C�e���擾�p�[�e�B�N��2D����
//============================================================
void CParticle2D::GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < GETITEM_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = 0.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= GETITEM_MOVE;
		move.y *= GETITEM_MOVE;
		move.z = 0.0f;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g2D�I�u�W�F�N�g�̐���
		CEffect2D::Create
		( // ����
			rPos,					// �ʒu
			GETITEM_SIZE,			// ���a
			CEffect2D::TYPE_NORMAL,	// �e�N�X�`��
			GETITEM_EFF_LIFE,		// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			GETITEM_SUB_SIZE,		// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	��������
//============================================================
CParticle2D *CParticle2D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CParticle2D *pParticle2D = NULL;	// �p�[�e�B�N��2D�����p

	if (pParticle2D == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pParticle2D = new CParticle2D;	// �p�[�e�B�N��2D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pParticle2D != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �p�[�e�B�N��2D�̏�����
		if (FAILED(pParticle2D->Init(type, rPos, rCol)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pParticle2D;
			pParticle2D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pParticle2D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
