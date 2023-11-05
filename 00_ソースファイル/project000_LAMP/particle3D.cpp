//============================================================
//
//	�p�[�e�B�N��3D���� [particle3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "particle3D.h"
#include "effect3D.h"

//************************************************************
//	�}�N����`
//************************************************************
// �_���[�W�E�o�u������
#define DMG_MOVE		(5.0f)	// �_���[�W�̈ړ���
#define DMG_SPAWN		(35)	// �_���[�W�̐�����
#define DMG_EFF_LIFE	(120)	// �_���[�W�̎���
#define DMG_SIZE		(80.0f)	// �_���[�W�̑傫��
#define DMG_SUB_SIZE	(2.4f)	// �_���[�W�̔��a�̌��Z��

// ��
#define HEAL_POSGAP		(24.0f)	// �񕜂̈ʒu�����
#define HEAL_MOVE		(1.2f)	// �񕜂̈ړ���
#define HEAL_SPAWN		(6)		// �񕜂̐�����
#define HEAL_EFF_LIFE	(50)	// �񕜂̎���
#define HEAL_SIZE		(55.0f)	// �񕜂̑傫��
#define HEAL_SUB_SIZE	(0.5f)	// �񕜂̔��a�̌��Z��

// �A�����݂Ԃ�
#define STOMP_MOVE		(2.5f)	// �A�����݂Ԃ��̈ړ���
#define STOMP_SPAWN		(8)		// �A�����݂Ԃ��̐�����
#define STOMP_EFF_LIFE	(28)	// �A�����݂Ԃ��̎���
#define STOMP_SIZE		(36.0f)	// �A�����݂Ԃ��̑傫��
#define STOMP_SUB_SIZE	(0.05f)	// �A�����݂Ԃ��̔��a�̌��Z��

// ������
#define SMALL_EXP_FIRE_COL			(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �����̉��̐F
#define SMALL_EXP_FIRE_POSGAP		(5.0f)	// �����̉��̈ʒu�����
#define SMALL_EXP_FIRE_MOVE			(2.0f)	// �����̉��̈ړ���
#define SMALL_EXP_FIRE_SPAWN		(48)	// �����̉��̐�����
#define SMALL_EXP_FIRE_EFF_LIFE		(18)	// �����̉��̎���
#define SMALL_EXP_FIRE_RAND_LIFE	(8)		// �����̉��̃����_���������Z�ʂ̍ő�l
#define SMALL_EXP_FIRE_SIZE			(3.27f)	// �����̉��̑傫��
#define SMALL_EXP_FIRE_SUB_SIZE		(-8.5f)	// �����̉��̔��a�̌��Z��

#define SMALL_EXP_SMOKE_COL			(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f))	// �����̉��̐F
#define SMALL_EXP_SMOKE_POSGAP		(3.0f)	// �����̉��̈ʒu�����
#define SMALL_EXP_SMOKE_MOVE		(1.2f)	// �����̉��̈ړ���
#define SMALL_EXP_SMOKE_SPAWN		(66)	// �����̉��̐�����
#define SMALL_EXP_SMOKE_EFF_LIFE	(44)	// �����̉��̎���
#define SMALL_EXP_SMOKE_RAND_LIFE	(12)	// �����̉��̃����_���������Z�ʂ̍ő�l
#define SMALL_EXP_SMOKE_SIZE		(70.0f)	// �����̉��̑傫��
#define SMALL_EXP_SMOKE_SUB_SIZE	(-1.0f)	// �����̉��̔��a�̌��Z��

// �唚��
#define BIG_EXP_FIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �����̉��̐F
#define BIG_EXP_FIRE_POSGAP		(10.0f)	// �����̉��̈ʒu�����
#define BIG_EXP_FIRE_MOVE		(2.54f)	// �����̉��̈ړ���
#define BIG_EXP_FIRE_SPAWN		(52)	// �����̉��̐�����
#define BIG_EXP_FIRE_EFF_LIFE	(36)	// �����̉��̎���
#define BIG_EXP_FIRE_RAND_LIFE	(12)	// �����̉��̃����_���������Z�ʂ̍ő�l
#define BIG_EXP_FIRE_SIZE		(3.57f)	// �����̉��̑傫��
#define BIG_EXP_FIRE_SUB_SIZE	(-7.0f)	// �����̉��̔��a�̌��Z��

#define BIG_EXP_SMOKE_COL		(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f))	// �����̉��̐F
#define BIG_EXP_SMOKE_POSGAP	(8.0f)	// �����̉��̈ʒu�����
#define BIG_EXP_SMOKE_MOVE		(0.9f)	// �����̉��̈ړ���
#define BIG_EXP_SMOKE_SPAWN		(72)	// �����̉��̐�����
#define BIG_EXP_SMOKE_EFF_LIFE	(96)	// �����̉��̎���
#define BIG_EXP_SMOKE_RAND_LIFE	(18)	// �����̉��̃����_���������Z�ʂ̍ő�l
#define BIG_EXP_SMOKE_SIZE		(70.0f)	// �����̉��̑傫��
#define BIG_EXP_SMOKE_SUB_SIZE	(-2.5f)	// �����̉��̔��a�̌��Z��

// �v���C���[�_���[�W
#define PLAY_DMG_POSGAP		(12.0f)	// �v���C���[�_���[�W�̈ʒu�����
#define PLAY_DMG_MOVE_S		(6.6f)	// �v���C���[�_���[�W�̈ړ��� (��)
#define PLAY_DMG_MOVE_M		(5.4f)	// �v���C���[�_���[�W�̈ړ��� (��)
#define PLAY_DMG_MOVE_L		(4.2f)	// �v���C���[�_���[�W�̈ړ��� (��)
#define PLAY_DMG_EFF_SPAWN	(4)		// �v���C���[�_���[�W�̐�����
#define PLAY_DMG_RAND_SPAWN	(6)		// �v���C���[�_���[�W�̃����_�����������Z�ʂ̍ő�l
#define PLAY_DMG_EFF_LIFE	(28)	// �v���C���[�_���[�W�̎���
#define PLAY_DMG_RAND_LIFE	(12)	// �v���C���[�_���[�W�̃����_���������Z�ʂ̍ő�l
#define PLAY_DMG_SIZE_S		(32.0f)	// �v���C���[�_���[�W�̑傫�� (��)
#define PLAY_DMG_SIZE_M		(42.0f)	// �v���C���[�_���[�W�̑傫�� (��)
#define PLAY_DMG_SIZE_L		(52.0f)	// �v���C���[�_���[�W�̑傫�� (��)
#define PLAY_DMG_SUB_SIZE	(0.25f)	// �v���C���[�_���[�W�̔��a�̌��Z��

// �}�Y���t���b�V��
#define MUZZLEFLASH_COL			(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �}�Y���t���b�V���̐F
#define MUZZLEFLASH_MOVE		(0.64f)		// �}�Y���t���b�V���̈ړ���
#define MUZZLEFLASH_SPAWN		(48)		// �}�Y���t���b�V���̐�����
#define MUZZLEFLASH_EFF_LIFE	(8)			// �}�Y���t���b�V���̎���
#define MUZZLEFLASH_SIZE		(0.57f)		// �}�Y���t���b�V���̑傫��
#define MUZZLEFLASH_SUB_SIZE	(-1.42f)	// �}�Y���t���b�V���̔��a�̌��Z��

// �^���N�t�@�C�A
#define TANKFIRE_COL		(D3DXCOLOR(1.0f, 0.38f, 0.23f, 1.0f))	// �^���N�t�@�C�A�̐F
#define TANKFIRE_MOVE		(2.64f)		// �^���N�t�@�C�A�̈ړ���
#define TANKFIRE_SPAWN		(58)		// �^���N�t�@�C�A�̐�����
#define TANKFIRE_EFF_LIFE	(22)		// �^���N�t�@�C�A�̎���
#define TANKFIRE_SIZE		(2.57f)		// �^���N�t�@�C�A�̑傫��
#define TANKFIRE_SUB_SIZE	(-3.42f)	// �^���N�t�@�C�A�̔��a�̌��Z��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const int CParticle3D::mc_aLife[CParticle3D::TYPE_MAX] =	// �����萔
{
	0,	// �Ȃ�
	24,	// �_���[�W
	1,	// ��
	1,	// �A�����݂Ԃ�
	1,	// ������
	1,	// �唚��
	1,	// �v���C���[�_���[�W
	1,	// �}�Y���t���b�V��
	1,	// �^���N�t�@�C�A
	24,	// �o�u������
};

//************************************************************
//	�q�N���X [CParticle3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CParticle3D::CParticle3D() : CObject(CObject::LABEL_PARTICLE)
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
CParticle3D::~CParticle3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CParticle3D::Init(void)
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
HRESULT CParticle3D::Init(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
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
void CParticle3D::Uninit(void)
{
	// �p�[�e�B�N��3D�I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CParticle3D::Update(void)
{
	if (m_nLife > 0)
	{ // ����������ꍇ

		// ���������Z
		m_nLife--;
	}
	else
	{ // �������Ȃ��ꍇ

		// �p�[�e�B�N��3D�I�u�W�F�N�g�̏I��
		Uninit();

		// �֐��𔲂���
		return;
	}

	switch (m_type)
	{ // ��ނ��Ƃ̏���
	case TYPE_DAMAGE:

		// �_���[�W
		Damage(m_pos, m_col);

		break;

	case TYPE_HEAL:

		// ��
		Heal(m_pos, m_col);

		break;

	case TYPE_STOMP_PLANT:

		// �A�����݂Ԃ�
		StompPlant(m_pos, m_col);

		break;

	case TYPE_SMALL_EXPLOSION:

		// ������
		SmallExplosion(m_pos);

		break;

	case TYPE_BIG_EXPLOSION:

		// �唚��
		BigExplosion(m_pos);

		break;

	case TYPE_PLAYER_DAMAGE:

		// �v���C���[�_���[�W
		PlayerDamage(m_pos);

		break;

	case TYPE_MUZZLE_FLASH:

		// �}�Y���t���b�V��
		MuzzleFlash(m_pos);

		break;

	case TYPE_TANK_FIRE:

		// �^���N�t�@�C�A
		TankFire(m_pos);

		break;

	case TYPE_BUBBLE_EXPLOSION:

		// �o�u������
		BubbleExplosion(m_pos);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CParticle3D::Draw(void)
{

}

//============================================================
//	�_���[�W
//============================================================
void CParticle3D::Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 12 == 0)
	{ // ������12�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				rPos,					// �ʒu
				DMG_SIZE,				// ���a
				CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
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
//	��
//============================================================
void CParticle3D::Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < HEAL_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * HEAL_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * HEAL_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,					// �ʒu
			HEAL_SIZE,				// ���a
			CEffect3D::TYPE_HEAL,	// �e�N�X�`��
			HEAL_EFF_LIFE,			// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			HEAL_SUB_SIZE,			// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�A�����݂Ԃ�
//============================================================
void CParticle3D::StompPlant(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p

	for (int nCntPart = 0; nCntPart < STOMP_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= STOMP_MOVE;
		move.y *= STOMP_MOVE;
		move.z *= STOMP_MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			STOMP_SIZE,				// ���a
			CEffect3D::TYPE_LEAF,	// �e�N�X�`��
			STOMP_EFF_LIFE,			// ����
			move,					// �ړ���
			rot,					// ����
			rCol,					// �F
			STOMP_SUB_SIZE,			// ���a�̌��Z��
			false,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	������
//============================================================
void CParticle3D::SmallExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < SMALL_EXP_SMOKE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * SMALL_EXP_SMOKE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * SMALL_EXP_SMOKE_MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// ������ݒ�
		nLife = (rand() % SMALL_EXP_SMOKE_RAND_LIFE) + SMALL_EXP_SMOKE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			SMALL_EXP_SMOKE_SIZE,		// ���a
			CEffect3D::TYPE_SMOKE,		// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			SMALL_EXP_SMOKE_COL,		// �F
			SMALL_EXP_SMOKE_SUB_SIZE,	// ���a�̌��Z��
			false,						// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}

	for (int nCntPart = 0; nCntPart < SMALL_EXP_FIRE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * SMALL_EXP_FIRE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * SMALL_EXP_FIRE_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % SMALL_EXP_FIRE_RAND_LIFE) + SMALL_EXP_FIRE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			SMALL_EXP_FIRE_SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,		// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			SMALL_EXP_FIRE_COL,			// �F
			SMALL_EXP_FIRE_SUB_SIZE,	// ���a�̌��Z��
			true,						// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�唚��
//============================================================
void CParticle3D::BigExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < BIG_EXP_SMOKE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * BIG_EXP_SMOKE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * BIG_EXP_SMOKE_MOVE;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// ������ݒ�
		nLife = (rand() % BIG_EXP_SMOKE_RAND_LIFE) + BIG_EXP_SMOKE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,					// �ʒu
			BIG_EXP_SMOKE_SIZE,		// ���a
			CEffect3D::TYPE_SMOKE,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			BIG_EXP_SMOKE_COL,		// �F
			BIG_EXP_SMOKE_SUB_SIZE,	// ���a�̌��Z��
			false,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}

	for (int nCntPart = 0; nCntPart < BIG_EXP_FIRE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * BIG_EXP_FIRE_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * BIG_EXP_FIRE_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % BIG_EXP_FIRE_RAND_LIFE) + BIG_EXP_FIRE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,					// �ʒu
			BIG_EXP_FIRE_SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			BIG_EXP_FIRE_COL,		// �F
			BIG_EXP_FIRE_SUB_SIZE,	// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�v���C���[�_���[�W
//============================================================
void CParticle3D::PlayerDamage(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 vec  = VEC3_ZERO;	// �x�N�g���̐ݒ�p
	D3DXVECTOR3 pos  = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	D3DXCOLOR   col  = XCOL_WHITE;	// �F�̑���p
	int nSpawn = 0;	// �������̑���p
	int nLife = 0;	// �����̑���p

	// ��������ݒ�
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * PLAY_DMG_MOVE_S;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 20 + 80) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			PLAY_DMG_SIZE_S,			// ���a
			CEffect3D::TYPE_PIECE_S,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			PLAY_DMG_SUB_SIZE,			// ���a�̌��Z��
			true,						// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}

	// ��������ݒ�
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * PLAY_DMG_MOVE_M;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 20 + 80) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 80 + 20) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			PLAY_DMG_SIZE_M,			// ���a
			CEffect3D::TYPE_PIECE_M,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			PLAY_DMG_SUB_SIZE,			// ���a�̌��Z��
			true,						// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}

	// ��������ݒ�
	nSpawn = (rand() % PLAY_DMG_EFF_SPAWN) + PLAY_DMG_RAND_SPAWN;

	for (int nCntPart = 0; nCntPart < nSpawn; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		vec.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		vec.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&vec, &vec);

		// �ʒu��ݒ�
		pos = rPos + vec * PLAY_DMG_POSGAP;

		// �ړ��ʂ�ݒ�
		move = vec * PLAY_DMG_MOVE_L;

		// ������ݒ�
		rot.x = 0.0f;
		rot.y = 0.0f;
		rot.z = (float)(rand() % 629 - 314) / 100.0f;

		// �F��ݒ�
		col.r = (float)(rand() % 80 + 20) / 100.0f;
		col.g = (float)(rand() % 80 + 20) / 100.0f;
		col.b = (float)(rand() % 20 + 80) / 100.0f;
		col.a = 1.0f;

		// ������ݒ�
		nLife = (rand() % PLAY_DMG_RAND_LIFE) + PLAY_DMG_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			pos,						// �ʒu
			PLAY_DMG_SIZE_L,			// ���a
			CEffect3D::TYPE_PIECE_L,	// �e�N�X�`��
			nLife,						// ����
			move,						// �ړ���
			rot,						// ����
			col,						// �F
			PLAY_DMG_SUB_SIZE,			// ���a�̌��Z��
			true,						// ���Z������
			LABEL_PARTICLE				// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�}�Y���t���b�V��
//============================================================
void CParticle3D::MuzzleFlash(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < MUZZLEFLASH_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= MUZZLEFLASH_MOVE;
		move.y *= MUZZLEFLASH_MOVE;
		move.z *= MUZZLEFLASH_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + MUZZLEFLASH_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			MUZZLEFLASH_SIZE,		// ���a
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			MUZZLEFLASH_COL,		// �F
			MUZZLEFLASH_SUB_SIZE,	// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�^���N�t�@�C�A
//============================================================
void CParticle3D::TankFire(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p
	int nLife = 0;	// �����̑���p

	for (int nCntPart = 0; nCntPart < TANKFIRE_SPAWN; nCntPart++)
	{ // ���������G�t�F�N�g�����J��Ԃ�

		// �x�N�g���������_���ɐݒ�
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

		// �x�N�g���𐳋K��
		D3DXVec3Normalize(&move, &move);

		// �ړ��ʂ�ݒ�
		move.x *= TANKFIRE_MOVE;
		move.y *= TANKFIRE_MOVE;
		move.z *= TANKFIRE_MOVE;

		// ������ݒ�
		rot = VEC3_ZERO;

		// ������ݒ�
		nLife = (rand() % 6) + TANKFIRE_EFF_LIFE;

		// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
		CEffect3D::Create
		( // ����
			rPos,					// �ʒu
			TANKFIRE_SIZE,			// ���a
			CEffect3D::TYPE_NORMAL,	// �e�N�X�`��
			nLife,					// ����
			move,					// �ړ���
			rot,					// ����
			TANKFIRE_COL,			// �F
			TANKFIRE_SUB_SIZE,		// ���a�̌��Z��
			true,					// ���Z������
			LABEL_PARTICLE			// �I�u�W�F�N�g���x��
		);
	}
}

//============================================================
//	�o�u������
//============================================================
void CParticle3D::BubbleExplosion(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 move = VEC3_ZERO;	// �ړ��ʂ̑���p
	D3DXVECTOR3 rot  = VEC3_ZERO;	// �����̑���p

	if ((m_nLife + 1) % 12 == 0)
	{ // ������12�̔{���̏ꍇ

		for (int nCntPart = 0; nCntPart < DMG_SPAWN; nCntPart++)
		{ // ���������G�t�F�N�g�����J��Ԃ�

			// �x�N�g���������_���ɐݒ�
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * 1.0f;

			// �x�N�g���𐳋K��
			D3DXVec3Normalize(&move, &move);

			// �ړ��ʂ�ݒ�
			move.x *= DMG_MOVE;
			move.y *= DMG_MOVE;
			move.z *= DMG_MOVE;

			// ������ݒ�
			rot = VEC3_ZERO;

			// �G�t�F�N�g3D�I�u�W�F�N�g�̐���
			CEffect3D::Create
			( // ����
				rPos,					// �ʒu
				DMG_SIZE,				// ���a
				CEffect3D::TYPE_BUBBLE,	// �e�N�X�`��
				DMG_EFF_LIFE,			// ����
				move,					// �ړ���
				rot,					// ����
				XCOL_WHITE,				// �F
				DMG_SUB_SIZE,			// ���a�̌��Z��
				true,					// ���Z������
				LABEL_PARTICLE			// �I�u�W�F�N�g���x��
			);
		}
	}
}

//============================================================
//	��������
//============================================================
CParticle3D *CParticle3D::Create(const EType type, const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CParticle3D *pParticle3D = NULL;	// �p�[�e�B�N��3D�����p

	if (pParticle3D == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pParticle3D = new CParticle3D;	// �p�[�e�B�N��3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pParticle3D != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �p�[�e�B�N��3D�̏�����
		if (FAILED(pParticle3D->Init(type, rPos, rCol)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pParticle3D;
			pParticle3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pParticle3D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}
