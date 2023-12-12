//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "playerEntry.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"
#include "playerAI.h"

#include "multiModel.h"
#include "objectOrbit.h"
#include "object2D.h"
#include "timerManager.h"
#include "retentionManager.h"
#include "rankingManager.h"
#include "stage.h"
#include "field.h"
#include "liquid.h"
#include "scrollMeshField.h"

#include "effect3D.h"

#include "particle3D.h"

#include "statusManager.h"
#include "flail.h"
#include "spawnpoint.h"
#include "obstacle.h"
#include "playerAI.h"

#include "orbitalEffect.h"
#include "orbitalParticle.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	GRAVITY		= 1.0f;		// �d��
	const float	RADIUS		= 50.0f;	// ���a
	const float	HEIGHT		= 100.0f;	// �c��

	const float	DASH_UP			= 10.0f;	// �_�b�V���㏸��
	const float	DASH_SIDE		= 15.0f;	// �_�b�V�����ړ���
	const float	DASH_REV		= 0.25f;	// �_�b�V���̌��Z�W��
	const float	DASH_MINMOVE	= 0.06f;	// �_�b�V���ēx�\�ɂȂ�ړ���

	const float	KNOCK_UP	= 15.0f;	// �m�b�N�㏸��
	const float	KNOCK_SIDE	= 35.0f;	// �m�b�N���ړ���
	const int	DMG_KILLY	= 50;		// �L��Y���W�̃_���[�W��

	const float MAX_KNOCK_RATE	= 4.0f;		// �ő吁����є{��
	const float	INVULN_ALPHA	= 0.5f;		// ���G��Ԃ̓����x
	const int	INVULN_CNT		= 60;		// ���G���Ԃ̃t���[����
	const int	DROWN_CNT		= 120;		// �M�ꎞ�Ԃ̃t���[����
	const float	ADD_SINROT		= 0.25f;	// �����x�ӂ�ӂ킳����ۂ̃T�C���J�[�u�������Z��
	const float	MAX_ADD_ALPHA	= 0.25f;	// �����x�̍ő���Z��

	const float	NORMAL_JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	NORMAL_LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��

	const float	KNOCK_REV	= 0.08f;	// �m�b�N��Ԏ��̈ړ��ʂ̌����W��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	STICK_REV	= 0.00015f;	// �X�e�B�b�N�̌X���ʂ̕␳�W��

	const float	DEAD_ZONE	= (float)USHRT_MAX * 0.01f;	// �X�e�B�b�N�̖�������X����
	const float	SPAWN_ADD_ALPHA		= 0.0075f;			// �X�|�[����Ԏ��̓����x�̉��Z��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPlayer::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\PLAYER\\00_waist.x",		// ��
	"data\\MODEL\\PLAYER\\01_body.x",		// ��
	"data\\MODEL\\PLAYER\\02_head.x",		// ��
	"data\\MODEL\\PLAYER\\03_shoulder_R.x",	// ����r
	"data\\MODEL\\PLAYER\\04_arm_R.x",		// �E��r
	"data\\MODEL\\PLAYER\\05_hand_R.x",		// �����r
	"data\\MODEL\\PLAYER\\06_shoulder_L.x",	// �E���r
	"data\\MODEL\\PLAYER\\07_arm_L.x",		// ����
	"data\\MODEL\\PLAYER\\08_hand_L.x",		// �E��
	"data\\MODEL\\PLAYER\\09_thigh_R.x",	// ��������
	"data\\MODEL\\PLAYER\\10_leg_R.x",		// �E������
	"data\\MODEL\\PLAYER\\11_thigh_L.x",	// ����
	"data\\MODEL\\PLAYER\\12_leg_L.x",		// �E��
	"data\\MODEL\\PLAYER\\13_stick.x",		// �t���C��������
};

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer(const int nPad) : CObjectChara(CObject::LABEL_PLAYER, PRIORITY), m_nPadID(nPad)
{
	// �����o�ϐ����N���A
	m_pStatus		= NULL;			// �X�e�[�^�X�̏��
	m_pFlail		= NULL;			// �t���C���̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_dashRot		= VEC3_ZERO;	// �_�b�V������
	m_state			= STATE_NONE;	// ���
	m_motionOld		= 0;			// �ߋ����[�V����
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterFlail	= 0;			// �t���C���Ǘ��J�E���^�[
	m_fPlusMove		= 0.0f;			// �v���X�ړ���
	m_fSinAlpha		= 0.0f;			// ��������
	m_bDash			= false;		// �_�b�V����
	m_bJump			= false;		// �W�����v��
	m_bAI			= false;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	m_pStatus		= NULL;			// �X�e�[�^�X�̏��
	m_pFlail		= NULL;			// �t���C���̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_dashRot		= VEC3_ZERO;	// �_�b�V������
	m_state			= STATE_NONE;	// ���
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterFlail	= 0;			// �t���C���Ǘ��J�E���^�[
	m_fPlusMove		= 0.0f;			// �v���X�ړ���
	m_fSinAlpha		= 0.0f;			// ��������
	m_bDash			= false;		// �_�b�V����
	m_bJump			= true;			// �W�����v��
	m_SItemPermanent[0] = {};
	m_SItemPermanent[1] = {};
	m_SItemTemporary = {};
	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�b�g�A�b�v�̓ǂݍ���
	LoadSetup();

	// ���f�����̐ݒ�
	SetModelInfo();

	// �X�e�[�^�X���̐���
	m_pStatus = CStatusManager::Create(m_nPadID);
	if (m_pStatus == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t���C���̐���
	m_pFlail = CFlail::Create(*this,VEC3_ZERO);
	if (m_pFlail == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	m_pFlail->SetPlayerID(m_nPadID);

	// �t���C���̐���
	m_pAI = CPlayerAI::Create(m_nPadID);
	if (m_pAI == NULL)
	{ // ��g�p���̏ꍇ

	  // ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���C���J���[��ݒ�
	SetMainMaterial();
	SetEnableDepthShadow(true);
	SetEnableZTex(true);

	m_pGuide = CObject3D::Create(GetVec3Position(), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
	m_pGuide->SetEnableDraw(false);
	m_pGuide->SetLabel(ELabel::LABEL_UI);
	m_pGuide->BindTexture("data\\TEXTURE\\Guide.png");

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �X�e�[�^�X���̔j��
	if (FAILED(m_pStatus->Release(m_pStatus)))
	{ assert(false); }	// �j�����s

	// �t���C���̏I��
	m_pFlail->Uninit();

	if (m_pAI != NULL)
	{ // �g�p���̏ꍇ
		// �������J��
		delete m_pAI;
		m_pAI = NULL;
	}
	if (m_pGuide != NULL)
	{ // �g�p���̏ꍇ
	  // �������J��
		m_pGuide->Uninit();
		m_pGuide = NULL;
	}
	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_DEATH;	// ���݂̃��[�V����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	if (CManager::GetInstance()->GetRetentionManager()->GetNumSurvival() == 1)
	{ // �c��l����1�l�̏ꍇ

		if (m_state != STATE_DEATH)
		{ // ���S���Ă��Ȃ��ꍇ

			// ���������L���O���X�V (��ʂ�ݒ�)
			CManager::GetInstance()->GetRetentionManager()->SetSurvivalRank(m_nPadID);
		}
	}

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// �ҋ@���[�V�����ɂ���
		currentMotion = MOTION_IDOL;

		break;

	case STATE_SPAWN:	// �X�|�[�����

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:	// �ʏ���

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal();

		break;

	case STATE_KNOCK:	// �m�b�N���

		// �m�b�N��Ԃ̍X�V
		currentMotion = UpdateKnock();

		break;

	case STATE_INVULN:	// ���G���

		// ���G��Ԏ��̍X�V
		currentMotion = UpdateInvuln();

		break;

	case STATE_DROWN:	// �M����

		// �M���Ԏ��̍X�V
		currentMotion = UpdateDrown();

		break;

	case STATE_DEATH:	// ���S���

		// ���S��Ԏ��̍X�V
		UpdateDeath();

		break;

	default:
		assert(false);
		break;
	}
	if (m_SItemTemporary.type != ITEM_EMPTY)
	{
		m_SItemTemporary.nLife--;
		if (m_SItemTemporary.nLife <= 0)
		{
			m_SItemTemporary.type = ITEM_EMPTY;
			m_SItemTemporary.nLife = 0;
		}
	}


	// �t���C���̍X�V
	m_pFlail->Update();

	// �X�e�[�^�X���̍X�V
	m_pStatus->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �X�e���V���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	
	// ��r�Q�ƒl��ݒ肷��
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);
	
	// �X�e���V���}�X�N���w�肷�� 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// �X�e���V����r�֐����w�肷��
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

	// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_REPLACE);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(void)
{
	EItem Item = ITEM_EMPTY;
	while (Item == ITEM_EMPTY)
	{
		Item = (EItem)(rand() % ITEM_MAX);
	}
	switch (Item)
	{
	case CPlayer::ITEM_EMPTY:
		break;
	case CPlayer::ITEM_HEAL:
		m_pStatus->SetNumLife(m_pStatus->GetNumLife() + 20);
		m_pStatus->SetNumRate(m_pStatus->GetNumRate() - 20);
		break;
	case CPlayer::ITEM_BOOST_ATTACK:
		m_SItemTemporary.type = ITEM_BOOST_ATTACK;
		m_SItemTemporary.nLife = 600;
		break;
	case CPlayer::ITEM_BOOST_KNOCKBACK:
		m_SItemTemporary.type = ITEM_BOOST_KNOCKBACK;
		m_SItemTemporary.nLife = 600;
		break;
	case CPlayer::ITEM_SUPERARMOR:
		m_SItemTemporary.type = ITEM_BOOST_KNOCKBACK;
		m_SItemTemporary.nLife = 450;
		break;
	case CPlayer::ITEM_BIGFLAIL:
		SetItemPermanent(ITEM_BIGFLAIL);
		break;
	case CPlayer::ITEM_LONGFLAIL:
		SetItemPermanent(ITEM_LONGFLAIL);
		break;
	case CPlayer::ITEM_GHOSTFLAIL:
		SetItemPermanent(ITEM_GHOSTFLAIL);
		break;
	case CPlayer::ITEM_MULTIFLAIL:
		SetItemPermanent(ITEM_MULTIFLAIL);
		break;
	case CPlayer::ITEM_BURNINGFLAIL:
		SetItemPermanent(ITEM_BURNINGFLAIL);
		break;
	case CPlayer::ITEM_MAX:
		break;
	default:
		break;
	}
}
//============================================================
//�A�C�e���̌��ʐݒ�
//============================================================
void CPlayer::SetItemPermanent(EItem Item)
{
	for (int i = 0; i < 2; i++)
	{
		if (m_SItemPermanent[i].type == ITEM_EMPTY)
		{
			m_SItemPermanent[i].type = Item;
			return;
		}
	}
	
	m_SItemPermanent[0].type = m_SItemPermanent[1].type;
	m_SItemPermanent[1].type = Item;
}
//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
void CPlayer::HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock)
{
	
	if (IsDeath())
	{ // ���S�t���O�������Ă���ꍇ

		return;
	}

	if (m_state != STATE_NORMAL)
	{ // �ʏ��Ԃł͂Ȃ��ꍇ

		return;
	}

	// �ϐ���錾
	bool bDeath = false;	// ���S��

	// ���S�󋵂̐ݒ�
	{
		// �|�C���^��錾
		CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

		switch (pRetention->GetKillState())
		{ // �����������Ƃ̏���
		case CRetentionManager::KILL_LIFE:	// �̗͐�

			if (m_pStatus->GetNumLife() <= 0)
			{ // �̗͂����łɂȂ��ꍇ

				return;
			}

			// �̗͂Ƀ_���[�W��^����
			m_pStatus->AddNumLife(-nDmg);

			if (m_pStatus->GetNumLife() <= 0)
			{ // �̗͂��Ȃ��Ȃ����ꍇ

				// ���S��Ԃɂ���
				bDeath = true;
			}

			break;

		case CRetentionManager::KILL_KNOCK:	// ������΂���
			break;

		default:
			assert(false);
			break;
		}
	}

	// ��Ԃ̐ݒ�
	{
		// �ϐ���錾
		D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
		D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

		// �J�E���^�[��������
		m_nCounterState = 0;

		if (bDeath)
		{ // ���S���Ă���ꍇ

			// ���S��Ԃ�ݒ�
			SetState(STATE_DEATH);

			// ���S���[�V������ݒ�
			SetMotion(MOTION_DEATH);

			// �����p�[�e�B�N���𐶐�
		

		}
		else
		{ // ���S���Ă��Ȃ��ꍇ

			// NAKAMURA�F�ӂ��Ƃїʂ̌��ߕ���������Ες���
			// �ϐ���錾
			float fKnockRate = ((MAX_KNOCK_RATE - 1.0f) / (float)m_pStatus->GetNumMaxLife()) * m_pStatus->GetNumRate() + 1.0f;	// ������΂���

			// �m�b�N�o�b�N�ړ��ʂ�ݒ�
			m_move.x = fKnockRate * vecKnock.x * KNOCK_SIDE;
			m_move.y = KNOCK_UP;
			m_move.z = fKnockRate * vecKnock.z * KNOCK_SIDE;

			// �m�b�N�o�b�N�����Ɍ�����ݒ�
			rotPlayer.y = atan2f(vecKnock.x, vecKnock.z);	// ������ь������v�Z
			m_destRot.y = rotPlayer.y;	// �ڕW������ݒ�
			SetVec3Rotation(rotPlayer);	// ������ݒ�

			// �󒆏�Ԃɂ���
			m_bJump = true;

			// �m�b�N��Ԃ�ݒ�
			SetState(STATE_KNOCK);

			// ������у��[�V������ݒ�
			SetMotion(MOTION_KNOCK);

			// �����p�[�e�B�N���𐶐�

			CorbitalParticle::Create(GetVec3Position(), D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), VEC3_ZERO, VEC3_ZERO, D3DXVECTOR3(0.0f, -5.0f, 0.0f), 6, 800, 60, 120, 300, 0.5f,0.99f);
			CorbitalParticle::Create(GetVec3Position(), D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f), VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 6, 1600, 60, 60, 600, 1.0f, 0.8f);
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// �q�b�g��
	}

	// NAKAMURA�F������ї��̌��ߕ��͔C���܂�
	// ������ї������Z
	m_pStatus->AddNumRate(100);
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // �͈͓��̏ꍇ

		if (m_state != STATE_DEATH)
		{ // ���S��Ԃ���Ȃ��ꍇ

			// �}�e���A�����Đݒ�
			ResetMaterial();

			// ���C���J���[��ݒ�
			SetMainMaterial();

			// �����x�𓧖��ɍĐݒ�
			SetAlpha(1.0f);

			// �����̏�Ԃ�ݒ�
			m_state = (EState)nState;

			if (m_state == STATE_DEATH)
			{ // ���S��Ԃ̏ꍇ

				// ���������L���O���X�V
				CManager::GetInstance()->GetRetentionManager()->SetSurvivalRank(m_nPadID);
			}
		}
	}
	else { assert(false); }
}

//============================================================
//	��Ԏ擾����
//============================================================
int CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
//	�c���擾����
//============================================================
float CPlayer::GetHeight(void) const
{
	// �c����Ԃ�
	return HEIGHT;
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g
	m_pFlail->SetEnableUpdate(bUpdate);	// �t���C��
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g
	m_pFlail->SetEnableDraw(bDraw);	// �t���C��
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CPlayer::GetMtxWorld(void) const
{
	// �ϐ���錾
	D3DXMATRIX  mtxRot, mtxTrans, mtxWorld;		// �v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotPlayer.y, rotPlayer.x, rotPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posPlayer.x, posPlayer.y, posPlayer.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X��Ԃ�
	return mtxWorld;
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode, const int nPad)
{
	// �|�C���^��錾
	CPlayer *pPlayer = NULL;	// �v���C���[�����p

	if (pPlayer == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���[�h�I�[�o�[
		assert(mode > NONE_IDX && mode < CScene::MODE_MAX);

		switch (mode)
		{ // ���[�h���Ƃ̏���
		case CScene::MODE_ENTRY:

			// �������m��
			pPlayer = new CPlayerEntry(nPad);	// �v���C���[�G���g���[

			break;

		case CScene::MODE_GAME:

			// �������m��
			pPlayer = new CPlayer(nPad);	// �v���C���[

			break;
		}
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pPlayer != NULL)
	{ // �g�p����Ă���ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPlayer;
			pPlayer = NULL;

			// ���s��Ԃ�
			return NULL;
		}
	}

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
}

//============================================================
//	�o���̐ݒ菈��
//============================================================
void CPlayer::SetSpawn(void)
{
	// �ϐ���錾
	CObject *pSpawnPoint = CSpawnPoint::GetSavePoint(m_nPadID);
	if (pSpawnPoint != NULL)
	{ // �X�|�[���|�C���g������ꍇ

		// �ʒu��ݒ�
		SetVec3Position(pSpawnPoint->GetVec3Position());

		// ������ݒ�
		SetVec3Rotation(pSpawnPoint->GetVec3Rotation());
		m_destRot = pSpawnPoint->GetVec3Rotation();

		m_pAI->SetRotstickL(m_destRot.y);
	}
	else
	{ // �X�|�[���|�C���g���Ȃ��ꍇ

		// �ʒu��ݒ�
		SetVec3Position(VEC3_ZERO);

		// ������ݒ�
		SetVec3Rotation(VEC3_ZERO);
		m_destRot = VEC3_ZERO;
	}

	// ����������
	SetState(STATE_SPAWN);	// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �t���C���������I�ɏ���
	m_pFlail->CatchFlail();

	// �J�E���^�[�̐ݒ�
	m_nCounterFlail = flail::FLAIL_DEF;

	// �}�e���A�����Đݒ�
	ResetMaterial();
	
	// ���C���J���[��ݒ�
	SetMainMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �O�Ղ̏�����
	m_pFlail->InitOrbit();

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// ������
}

//============================================================
//	���G�̐ݒ菈��
//============================================================
void CPlayer::SetInvuln(void)
{
	// ����������
	SetState(STATE_INVULN);	// ���G��Ԃ̐ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �}�e���A�����Đݒ�
	ResetMaterial();
	
	// ���C���J���[��ݒ�
	SetMainMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(INVULN_ALPHA);

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// ������
}

//============================================================
//	UI�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDrawUI(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	m_pStatus->SetEnableDrawLife(bDraw);	// �̗�
	m_pStatus->SetEnableDrawRate(bDraw);	// ������ї�

	// UI�̕`��󋵂�ݒ�
	m_pStatus->SetEnableDrawUI(bDraw);
}

//============================================================
//	�L��Y���W�q�b�g����
//============================================================
void CPlayer::HitKillY(const int nDmg)
{
	if (IsDeath())
	{ // ���S�t���O�������Ă���ꍇ

		return;
	}

	if (m_state != STATE_NORMAL
	&&  m_state != STATE_KNOCK
	&&  m_state != STATE_INVULN)
	{ // �ʏ�E�m�b�N�E���G��Ԃł͂Ȃ��ꍇ

		return;
	}

	// �ϐ���錾
	bool bDeath = false;	// ���S��

	// ���S�󋵂̐ݒ�
	{
		// �|�C���^��錾
		CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����

		switch (pRetention->GetKillState())
		{ // �����������Ƃ̏���
		case CRetentionManager::KILL_LIFE:	// �̗͐�

			if (m_pStatus->GetNumLife() <= 0)
			{ // �̗͂����łɂȂ��ꍇ

				return;
			}

			// �̗͂Ƀ_���[�W��^����
			m_pStatus->AddNumLife(-nDmg);

			if (m_pStatus->GetNumLife() <= 0)
			{ // �̗͂��Ȃ��Ȃ����ꍇ

				// ���S��Ԃɂ���
				bDeath = true;
			}

			break;

		case CRetentionManager::KILL_KNOCK:	// ������΂���

			// ���S��Ԃɂ���
			bDeath = true;

			break;

		default:
			assert(false);
			break;
		}
	}

	// ��Ԃ̐ݒ�
	{
		// �ϐ���錾
		D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu

		// �J�E���^�[��������
		m_nCounterState = 0;

		if (bDeath)
		{ // ���S���Ă���ꍇ

			// ���S��Ԃ�ݒ�
			SetState(STATE_DEATH);

			// ���S���[�V������ݒ�
			SetMotion(MOTION_DEATH);

			// �����p�[�e�B�N���𐶐�
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}
		else
		{ // ���S���Ă��Ȃ��ꍇ

			// �M���Ԃ�ݒ�
			SetState(STATE_DROWN);

			// �M�ꃂ�[�V������ݒ�
			SetMotion(MOTION_DROWN);

			// �����p�[�e�B�N���𐶐�
			CParticle3D::Create(CParticle3D::TYPE_BIG_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + HEIGHT * 0.5f, posPlayer.z));
		}

		// �T�E���h�̍Đ�
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// �q�b�g��
	}
}

//============================================================
//	�p�b�h�C���f�b�N�X�擾����
//============================================================
int CPlayer::GetPadID(void) const
{
	// �p�b�h�C���f�b�N�X��Ԃ�
	return m_nPadID;
}

//============================================================
//	�t���C���J�E���^�[�擾����
//============================================================
int CPlayer::GetCounterFlail(void) const
{
	// �t���C���J�E���^�[��Ԃ�
	return m_nCounterFlail;
}

//============================================================
//	�t���C���J�E���^�[�ݒ菈��
//============================================================
void CPlayer::SetCounterFlail(const int nCounterFlail)
{
	m_nCounterFlail = nCounterFlail;
}

//============================================================
//	���g�̃��C���J���[�}�e���A���ݒ菈��
//============================================================
void CPlayer::SetMainMaterial(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MODEL_MAX; nCntPlayer++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// ���f���̐擪�}�e���A����ݒ�
		SetMaterial(GetMainMaterial(), nCntPlayer, 0);
	}
}

//============================================================
//	���g�̃��C���J���[�}�e���A���擾����
//============================================================
D3DXMATERIAL CPlayer::GetMainMaterial(void)
{
	switch (m_nPadID)
	{ // �p�b�h�C���f�b�N�X���Ƃ̏���
	case PLAYER_RED:	// �v���C���[1�F��

		// �ԃ}�e���A����Ԃ�
		return material::Red();

	case PLAYER_BLUE:	// �v���C���[2�F��

		// �}�e���A����Ԃ�
		return material::Blue();

	case PLAYER_YELLOW:	// �v���C���[3�F��

		// ���}�e���A����Ԃ�
		return material::Yellow();

	case PLAYER_GREEN:	// �v���C���[4�F��

		// �΃}�e���A����Ԃ�
		return material::Green();

	default:	// ��O����
		assert(false);
		return material::White();
	}
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CPlayer::UpdateMotion(int nMotion)
{
	// �ϐ���錾
	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����

	m_motionOld = nAnimMotion;

	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop(nAnimMotion))
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion);
			}
		}
		else
		{ // ���[�v���Ȃ����[�V�����������ꍇ

			switch (GetMotionType())
			{ // ���[�V�������Ƃ̏���
			case MOTION_ATTACK:	// �U�����[�V�����F���[�vOFF
			case MOTION_DASH:	// �_�b�V�����[�V�����F���[�vOFF
			case MOTION_KNOCK:	// ������у��[�V�����F���[�vOFF
			case MOTION_DEATH:	// ���S���[�V�����F���[�vOFF

				break;

			case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

				if (nMotion != MOTION_IDOL)
				{ // �ҋ@���[�V�����ł͂Ȃ��ꍇ

					// ���݂̃��[�V�����̐ݒ�
					SetMotion(nMotion);
				}

				break;

			default:	// ��O����
				assert(false);
				break;
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();

	switch (GetMotionType())
	{ // ���[�V�������Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V�����F���[�vON
	case MOTION_CHARGE:	// �`���[�W���[�V�����F���[�vON
	case MOTION_PULL:	// �������胂�[�V�����F���[�vON
	case MOTION_MOVE:	// �ړ����[�V�����F���[�vON
	case MOTION_DROWN:	// �M�ꃂ�[�V�����F���[�vON

		break;

	case MOTION_DASH:	// �_�b�V�����[�V�����F���[�vOFF
	case MOTION_LAND:	// ���n���[�V�����F���[�vOFF

		if (IsMotionFinish())
		{ // ���[�V�������I�����Ă����ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion);
		}

		break;

	case MOTION_ATTACK:	// �U�����[�V�����F���[�vOFF
	case MOTION_KNOCK:	// ������у��[�V�����F���[�vOFF
	case MOTION_DEATH:	// ���S���[�V�����F���[�vOFF

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	UpdateFadeOut(SPAWN_ADD_ALPHA);

	if (GetAlpha() >= INVULN_ALPHA)
	{ // ���G���̓����x�ȏ�̏ꍇ

		// ���G�̐l�ɂ���
		SetInvuln();
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return MOTION_IDOL;
	}

	if (m_bAI)
	{
		// CPU����
		currentMotion = m_pAI->playerAI(m_pFlail, GetVec3Position(), m_move, m_destRot, m_nCounterFlail, m_motionOld);
	}
	else
	{
		// �ړ�����
		currentMotion = UpdateMove(posPlayer);
	}

	// �d�͂̍X�V
	UpdateGravity();

	// ���n����
	UpdateLanding(posPlayer);

	if (m_bAI)
	{
		m_pAI->AIDash
		(
			GetVec3Position(),
			m_move,
			m_dashRot,
			m_destRot,
			m_fPlusMove,
			m_bDash
		);
	}
	else
	{
		// �_�b�V���X�V
		UpdateDash();
	}

	// �����X�V
	UpdateRotation(rotPlayer);

	// ��Q���Ƃ̓����蔻��
	CollisionObstacle(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �L��Y���W�Ƃ̓����蔻��
	if (pStage->CollisionKillY(posPlayer))
	{ // ���S���W�ɓ��B���Ă����ꍇ

		// �L��Y���W�q�b�g����
		HitKillY(DMG_KILLY);
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�m�b�N��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateKnock(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return MOTION_KNOCK;
	}

	// �d�͂̍X�V
	UpdateGravity();

	// ���n����
	if (UpdateLanding(posPlayer))
	{ // ���n�����ꍇ

		// ���G�̐l�ɂ���
		SetInvuln();

		// ���n���[�V������ݒ�
		SetMotion(MOTION_LAND);
	}

	// �����X�V
	UpdateRotation(rotPlayer);

	// ��Q���Ƃ̓����蔻��
	CollisionObstacle(posPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �L��Y���W�Ƃ̓����蔻��
	if (pStage->CollisionKillY(posPlayer))
	{ // ���S���W�ɓ��B���Ă����ꍇ

		// �L��Y���W�q�b�g����
		HitKillY(DMG_KILLY);
	}

	// ���݂̃��[�V������Ԃ�
	return (EMotion)GetMotionType();
}

//============================================================
//	���G��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateInvuln(void)
{
	// �ϐ���錾
	float fAddAlpha = 0.0f;	// �����x�̉��Z��

	// �����x���グ��
	m_fSinAlpha += ADD_SINROT;
	useful::NormalizeRot(m_fSinAlpha);	// �������K��

	// �����x���Z�ʂ����߂�
	fAddAlpha = (MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

	// �����x��ݒ�
	SetAlpha(INVULN_ALPHA + fAddAlpha);

	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState > INVULN_CNT)
	{ // ���G���Ԃ̏I���J�E���g�ɂȂ����ꍇ

		// �ʏ��Ԃ�ݒ�
		SetState(STATE_NORMAL);

		// �����x��s�����ɍĐݒ�
		SetAlpha(1.0f);
	}

	// �ʏ��Ԃ̏������s���A���̕Ԃ�l�̃��[�V������Ԃ�
	return UpdateNormal();
}

//============================================================
//	�M���Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateDrown(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
	}

	// �t���C���������I�ɏ���
	m_pFlail->CatchFlail();

	// �d�͂̍X�V
	UpdateGravity();

	// ���n���� (�ʒu�X�V)
	UpdateLanding(posPlayer);

	// �����X�V
	UpdateRotation(rotPlayer);

	// �g�ɒ��n������
	pStage->GetLiquid()->GetScrollMeshField(0)->LandPosition(posPlayer, m_move);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �J�E���^�[�����Z
	m_nCounterState++;

	if (m_nCounterState > DROWN_CNT)
	{ // �M�ꎞ�Ԃ̏I���J�E���g�ɂȂ����ꍇ

		// �ďo��������
		SetSpawn();
	}

	// �M�ꃂ�[�V������Ԃ�
	return MOTION_DROWN;
}

//============================================================
//	���S��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateDeath(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == NULL)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
	}

	// �t���C���������I�ɏ���
	m_pFlail->CatchFlail();

	// �d�͂̍X�V
	UpdateGravity();

	// ���n���� (�ʒu�X�V)
	UpdateLanding(posPlayer);

	// �����X�V
	UpdateRotation(rotPlayer);

	// �g�ɒ��n������
	pStage->GetLiquid()->GetScrollMeshField(0)->LandPosition(posPlayer, m_move);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����

	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h
	CCamera			*pCamera	= CManager::GetInstance()->GetCamera();		// �J����

	if (pCamera == NULL) { assert(false); return MOTION_IDOL; }	// ��g�p��

	// PC����
#if 0
	if (m_nPadID == 0)
	{
		if (pKeyboard->IsPress(DIK_W))
		{
			// �ړ��ʂ��X�V
			m_move.z += 2.0f;
		}
		else if (pKeyboard->IsPress(DIK_S))
		{
			// �ړ��ʂ��X�V
			m_move.z -= 2.0f;
		}
		else if (pKeyboard->IsPress(DIK_A))
		{
			// �ړ��ʂ��X�V
			m_move.x -= 2.0f;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{
			// �ړ��ʂ��X�V
			m_move.x += 2.0f;
		}
	}
#endif

	// �ϐ���錾
	D3DXVECTOR3 vecStick = D3DXVECTOR3((float)pPad->GetPressLStickX(m_nPadID), (float)pPad->GetPressLStickY(m_nPadID), 0.0f);	// �X�e�B�b�N�e���̓|����
	float fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// �X�e�B�b�N�̓|����
	
	if (DEAD_ZONE < fStick)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		// �ϐ���錾
		float fMove = fStick * STICK_REV;	// �v���C���[�ړ���

		if (!m_bDash)
		{ // �_�b�V�����ł͂Ȃ��ꍇ

			// �ړ��ʂ��X�V
			m_move.x += sinf(pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI) * fMove;
			m_move.z += cosf(pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI) * fMove;

			// �ړ����[�V������ݒ�
			currentMotion = MOTION_MOVE;
		}
		else
		{ // �_�b�V�����̏ꍇ

			// �ړ��ʂ��X�V
			m_move.x += sinf(m_dashRot.y) * fMove;
			m_move.z += cosf(m_dashRot.y) * fMove;

			// �_�b�V�����[�V������ݒ�
			currentMotion = MOTION_DASH;
		}

		if (m_pFlail->GetLengthChain() >= flail::FLAIL_RADIUS * (m_pFlail->GetNumChain() - 1) || m_motionOld == MOTION_PULL)
		{ // �������苗���̏ꍇ

			// �ړ��ʂ��X�V
			m_move.x *= 0.7f;
			m_move.z *= 0.7f;

			D3DXVECTOR3 vecFlail;
			float rotMove, rotFlail, rotDiff;

			vecFlail = m_pFlail->GetVec3Position() - GetVec3Position();

			rotMove = atan2f(m_move.x, m_move.z);
			rotFlail = atan2f(vecFlail.x, vecFlail.z);

			rotDiff = rotMove - rotFlail;
			useful::NormalizeRot(rotDiff);

			if (rotDiff > D3DX_PI * 0.5f || rotDiff < D3DX_PI * -0.5f)
			{
				// �������胂�[�V������ݒ�
				currentMotion = MOTION_PULL;
			}
		}

		// �ڕW������ݒ�
		m_destRot.y = atan2f(-m_move.x, -m_move.z);
	}

	vecStick = D3DXVECTOR3((float)pPad->GetPressRStickX(m_nPadID), (float)pPad->GetPressRStickY(m_nPadID), 0.0f);	// �X�e�B�b�N�e���̓|����
	D3DXVECTOR3 vec;
	//�K�C�h�\���v�Z
	D3DXVec3Normalize(&vec, &D3DXVECTOR3(vecStick.x, 100.0f, -vecStick.y));
	m_pGuide->SetVec3Position(GetVec3Position() + (vec * 300.0f));
	m_pGuide->SetVec3Rotation(D3DXVECTOR3(0.0f, atan2f(vecStick.x, -vecStick.y), 0.0f));
	if (vecStick != VEC3_ZERO)
	{
		m_pGuide->SetEnableDraw(true);
	}
	else
	{
		m_pGuide->SetEnableDraw(false);
	}
	fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// �X�e�B�b�N�̓|����

	
	// �J�E���^�[�̒l�ɂ���ċ�����ύX
	if (m_nCounterFlail > flail::FLAIL_DEF)
	{// 0���傫����

		if ((CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail <= flail::FLAIL_CHARGE)
		{// ������{�^����������Ă��鎞
		 // �J�E���^�[�A�b�v
			m_nCounterFlail++;

			// ���l�ŃJ�E���^�[���~�߂�
			if (m_nCounterFlail > flail::FLAIL_CHARGE)
			{
				m_nCounterFlail = flail::FLAIL_CHARGE;
			}

			// ���߂Ă�ԓS����U���
			m_pFlail->SetChainRotMove((-0.002f * m_nCounterFlail) - 0.12f);

			m_pFlail->SetLengthTarget(flail::FLAIL_RADIUS * 5.0f);

			// �ړ��ʂ��X�V
			m_move.x *= 1.0f - (0.0042f * m_nCounterFlail);
			m_move.z *= 1.0f - (0.0042f * m_nCounterFlail);

			// �ڕW������ݒ�
			m_destRot.y = m_pFlail->GetChainRotTarget() + D3DX_PI;

			// �`���[�W���[�V������ݒ�
			currentMotion = MOTION_CHARGE;
		}

		// ����
		if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail != flail::FLAIL_THROW)
		{
			// ���߂����Ԃɉ����Ĕ򋗗�����
			D3DXVECTOR3 move = VEC3_ZERO;
			float lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));
			move.x = (sinf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			move.z = (cosf(m_pFlail->GetChainRotTarget()) * 5.0f * m_nCounterFlail);
			m_pFlail->SetMove(move);
			m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove());

			D3DXVECTOR3 posFlail = m_pFlail->GetVec3Position();
			if (posFlail.y < 0.0f)
			{
				posFlail.y = 0.0f;
				m_pFlail->SetVec3Position(posFlail);
			}

			if (DEAD_ZONE < fStick)
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1));

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN;
				}

				// �ڕW�p�x�ɍ��킹��
				m_pFlail->ShotFlail(m_pFlail->GetChainRotTarget() - D3DX_PI * 0.5f);
				m_pFlail->SetChainRotMove(0.0f);
			}
			else
			{
				lengthTarget = flail::FLAIL_RADIUS * (float)(((float)m_nCounterFlail / (float)flail::FLAIL_CHARGE) * (m_pFlail->GetNumChain() - 1)) * 0.6f;

				if (lengthTarget < flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f)
				{
					lengthTarget = flail::FLAIL_RADIUS * flail::FLAIL_NUM_MIN * 0.5f;
				}
			}

			m_pFlail->SetLengthTarget(lengthTarget);

			// �J�E���^�[�̐ݒ�
			m_nCounterFlail = flail::FLAIL_THROW;

			// �U�����[�V������ݒ�
			currentMotion = MOTION_ATTACK;

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SWING);
		}

		if (m_nCounterFlail == flail::FLAIL_THROW)
		{
			// �ړ��ʂ��X�V
			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// �t���C�����~�܂�����J�E���^�[�����̒i�K��
			if (m_pFlail->GetLengthChain() >= m_pFlail->GetLengthTarget())
			{
				// ���[�V�������U������ύX
				SetMotion(currentMotion);

				m_nCounterFlail = flail::FLAIL_DROP;
			}
		}
	}
	else if (m_nCounterFlail == flail::FLAIL_DEF)
	{
		// �t���C���������I�ɏ���
		m_pFlail->CatchFlail();

		m_pFlail->SetChainRotTarget(m_destRot.y + D3DX_PI);

		// �J�E���^�[�A�b�v�J�n
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_R1, m_nPadID) == TRUE)
		{
			m_nCounterFlail++;
		}
	}
	else
	{
		// �S���ƃv���C���[�̋�������薢���̎��v���C���[�ʒu�ɓS���Œ�
		if (m_pFlail->GetLengthChain() <= flail::FLAIL_RADIUS * 3.0f || m_pFlail->GetLengthTarget() <= flail::FLAIL_RADIUS * 6.0f)
		{
			m_nCounterFlail = flail::FLAIL_DEF;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->CatchFlail();
		}
		else
		{
			// �����߂�
			if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE)
			{
				m_nCounterFlail -= 1;

				if (m_nCounterFlail < -60)
				{
					m_nCounterFlail = -60;
				}

				if (m_nCounterFlail == -2)
				{
					float rot1 = CManager::GetInstance()->GetPad()->GetPressRStickRot(m_nPadID) + 1.57f;
					float rot2 = m_destRot.y;
					float rot3 = rot2 - rot1;

					useful::NormalizeRot(rot3);

					if (DEAD_ZONE < fStick)
					{ // �f�b�h�]�[���ȏ�̏ꍇ

						if (rot3 > 0.0f)
						{
							// ���߂Ă�ԓS����U���
							m_pFlail->SetChainRotMove(0.03f);
						}
						else
						{
							// ���߂Ă�ԓS����U���
							m_pFlail->SetChainRotMove(-0.03f);
						}
					}
					else
					{
						// ���߂Ă�ԓS����U���
						m_pFlail->SetChainRotMove(0.0f);
					}
				}

				// ���߂����Ԃɉ����Ĕ򋗗�����
				D3DXVECTOR3 move = VEC3_ZERO;
				move.x = (sinf(m_pFlail->GetChainRotTarget()) * (m_nCounterFlail * -m_nCounterFlail));
				move.z = (cosf(m_pFlail->GetChainRotTarget()) * (m_nCounterFlail * -m_nCounterFlail));
				m_pFlail->SetMove(move);

				// �ړ��ʂ��X�V
				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			// ����
			if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE))
			{
				D3DXVECTOR3 vecPtoFTarget = VEC3_ZERO;
				float lengthPtoFTarget = 0.0f;

				m_nCounterFlail = flail::FLAIL_DROP;

				vecPtoFTarget.x = GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_41 - m_pFlail->GetVec3Position().x;
				vecPtoFTarget.y = GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_42 - m_pFlail->GetVec3Position().y;
				vecPtoFTarget.z = GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_43 - m_pFlail->GetVec3Position().z;

				lengthPtoFTarget = D3DXVec3Length(&vecPtoFTarget);

				m_pFlail->SetLengthTarget(lengthPtoFTarget);
			}
		}
	}

	if (DEAD_ZONE < fStick)
	{ // �f�b�h�]�[���ȏ�̏ꍇ
		m_pFlail->SetChainRotTarget(CManager::GetInstance()->GetPad()->GetPressRStickRot(m_nPadID) + 1.57f);
	}

	// �ʒu��\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ʒu]�F%f %f %f\n", rPos.x, rPos.y, rPos.z);
	//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ړ���]�F%f %f %f\n", m_move.x, m_move.y, m_move.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�J�E���^�[]�F%d\n", m_nCounterFlail);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[������]�F%f\n", m_pFlail->GetLengthChain());
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[���ڕW����]�F%f\n", m_pFlail->GetLengthTarget());

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�_�b�V���̍X�V����
//============================================================
void CPlayer::UpdateDash(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h
	CCamera			*pCamera	= CManager::GetInstance()->GetCamera();		// �J����

	if (pCamera == NULL) { assert(false); }	// ��g�p��

	// �ϐ���錾
	D3DXVECTOR3 vecStick = D3DXVECTOR3((float)pPad->GetPressLStickX(m_nPadID), (float)pPad->GetPressLStickY(m_nPadID), 0.0f);	// �X�e�B�b�N�e���̓|����
	float fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// �X�e�B�b�N�̓|����

	if (DEAD_ZONE < fStick)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		if (pKeyboard->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_A, m_nPadID))
		{ // �_�b�V���̑��삪�s��ꂽ�ꍇ

			if (!m_bDash)
			{ // �_�b�V�����Ă��Ȃ��ꍇ

				// ��ړ��ʂ����Z
				m_move.y += DASH_UP;

				// �v���X�ړ��ʂ����Z
				m_fPlusMove += DASH_SIDE;

				// �_�b�V�����̌�����ۑ�
				m_dashRot.y = pPad->GetPressLStickRot(m_nPadID) + pCamera->GetVec3Rotation().y + HALF_PI;

				// �_�b�V�����Ă����Ԃɂ���
				m_bDash = true;

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_JUMP);	// �W�����v��
			}
		}
	}

	if (m_bDash)
	{ // �_�b�V�����Ă���ꍇ

		// �ړ��ʂ��X�V
		m_move.x += sinf(m_dashRot.y) * m_fPlusMove;
		m_move.z += cosf(m_dashRot.y) * m_fPlusMove;

		// �v���X�ړ��ʂ����Z
		m_fPlusMove += (0.0f - m_fPlusMove) * DASH_REV;

		if (fabsf(m_fPlusMove) < DASH_MINMOVE)
		{ // �ړ��ʂ����l��菭�Ȃ��Ȃ����ꍇ

			// �ă_�b�V���ł���悤�ɂ���
			m_fPlusMove = 0.0f;
			m_bDash = false;
		}
	}
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CPlayer::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �ʒu�̍X�V
	// �n�Ղ̓����蔻��E���n����
	if (CollisionGroundBlock(rPos))
	{ // �n�Ղɒ��n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// �n�ʂ̒��n����
	if (CScene::GetStage()->LandFieldPosition(rPos, m_move))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�n�ՂƂ̈ꎲ���Ƃ̓����蔻��
//============================================================
bool CPlayer::ResponseSingleGround(const EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);	// �v���C���[�ő�傫��
	bool bMin = false;	// �s�̕����̔����
	bool bMax = false;	// ���̕����̔����
	bool bHit = false;	// ���n�̔�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 posGround = VEC3_ZERO;		// �n�Ոʒu
				D3DXVECTOR3 rotGround = VEC3_ZERO;		// �n�Ռ���
				D3DXVECTOR3 sizeMinGround = VEC3_ZERO;	// �n�Սŏ��傫��
				D3DXVECTOR3 sizeMaxGround = VEC3_ZERO;	// �n�Սő�傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �n�Ղ̈ʒu��ݒ�
				posGround = pObjCheck->GetVec3Position();

				// �n�Ղ̌�����ݒ�
				rotGround = pObjCheck->GetVec3Rotation();

				// �n�Ղ̍ŏ��̑傫����ݒ�
				sizeMinGround = pObjCheck->GetVec3Sizing();
				sizeMinGround.y *= 2.0f;	// �c�̑傫����{�ɂ���

				// �n�Ղ̍ő�̑傫����ݒ�
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				sizeMaxGround.y = 0.0f;		// �c�̑傫����������

				switch (axis)
				{ // ���莲���Ƃ̏���
				case AXIS_X:	// X��

					// X���̏Փ˔���
					collision::ResponseSingleX
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				case AXIS_Y:	// Y��

					// Y���̏Փ˔���
					collision::ResponseSingleY
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						true,			// X����
						true,			// Z����
						&bMin,			// ������̔���
						&bMax			// �ォ��̔���
					);

					if (bMax)
					{ // �ォ�瓖�����Ă����ꍇ

						// ���n���Ă���󋵂ɂ���
						bHit = true;
					}

					break;

				case AXIS_Z:	// Z��

					// Z���̏Փ˔���
					collision::ResponseSingleZ
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �e���̔������Ԃ�
	return bHit;
}

//============================================================
//	�u���b�N�Ƃ̈ꎲ���Ƃ̓����蔻��
//============================================================
bool CPlayer::ResponseSingleBlock(const EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(RADIUS, 0.0f, RADIUS);		// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);	// �v���C���[�ő�傫��
	bool bHit = false;	// ���n�̔�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				D3DXVECTOR3 posBlock = VEC3_ZERO;	// �u���b�N�ʒu
				D3DXVECTOR3 rotBlock = VEC3_ZERO;	// �u���b�N����
				D3DXVECTOR3 sizeBlock = VEC3_ZERO;	// �u���b�N�傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // �I�u�W�F�N�g���x�����u���b�N�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �u���b�N�̈ʒu��ݒ�
				posBlock = pObjCheck->GetVec3Position();

				// �u���b�N�̌�����ݒ�
				rotBlock = pObjCheck->GetVec3Rotation();

				// �u���b�N�̑傫����ݒ�
				sizeBlock = pObjCheck->GetVec3Sizing();

				switch (axis)
				{ // ���莲���Ƃ̏���
				case AXIS_X:	// X��

					// X���̏Փ˔���
					collision::ResponseSingleX
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posBlock,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeBlock,		// ����ڕW�T�C�Y(�E�E��E��)
						sizeBlock,		// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						false			// Y����
					);

					break;

				case AXIS_Z:	// Z��

					// Z���̏Փ˔���
					collision::ResponseSingleZ
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posBlock,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeBlock,		// ����ڕW�T�C�Y(�E�E��E��)
						sizeBlock,		// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						true,			// X����
						false			// Y����
					);

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �e���̔������Ԃ�
	return bHit;
}

//============================================================
//	�n�ՁE�u���b�N�Ƃ̓����蔻��
//============================================================
bool CPlayer::CollisionGroundBlock(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �ړ��ʂ����Z
	rPos.x += m_move.x;

	// X���̓����蔻��
	ResponseSingleGround(AXIS_X, rPos);
	ResponseSingleBlock(AXIS_X, rPos);

	// �ړ��ʂ����Z
	rPos.y += m_move.y;

	// Y���̓����蔻��
	bLand = ResponseSingleGround(AXIS_Y, rPos);

	// �ړ��ʂ����Z
	rPos.z += m_move.z;

	// Z���̓����蔻��
	ResponseSingleGround(AXIS_Z, rPos);
	ResponseSingleBlock(AXIS_Z, rPos);

	if (m_state == STATE_KNOCK)
	{ // �m�b�N��Ԃ̏ꍇ

		m_move.x += (0.0f - m_move.x) * KNOCK_REV;
		m_move.z += (0.0f - m_move.z) * KNOCK_REV;
	}
	else
	{ // ����ȊO�̏�Ԃ̏ꍇ

		// �ړ��ʂ�����
		if (m_bJump)
		{ // �󒆂̏ꍇ

			m_move.x += (0.0f - m_move.x) * NORMAL_JUMP_REV;
			m_move.z += (0.0f - m_move.z) * NORMAL_JUMP_REV;
		}
		else
		{ // �n��̏ꍇ

			m_move.x += (0.0f - m_move.x) * NORMAL_LAND_REV;
			m_move.z += (0.0f - m_move.z) * NORMAL_LAND_REV;
		}
	}

	// ���n�󋵂�Ԃ�
	return bLand;
}

//============================================================
//	��Q���Ƃ̓����蔻��
//============================================================
bool CPlayer::CollisionObstacle(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bHit = false;	// ���n�̔�����

	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �ϐ���錾
				CObstacle::SStatusInfo status;		// ��Q���X�e�[�^�X
				D3DXVECTOR3 posObs = VEC3_ZERO;		// ��Q���ʒu
				D3DXVECTOR3 rotObs = VEC3_ZERO;		// ��Q������
				D3DXVECTOR3 sizeObsMin = VEC3_ZERO;	// ��Q���傫��
				D3DXVECTOR3 sizeObsMax = VEC3_ZERO;	// ��Q���傫��

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_OBSTACLE)
				{ // �I�u�W�F�N�g���x������Q���ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ��Q���̃X�e�[�^�X��ݒ�
				status = CObstacle::GetStatusInfo(pObjCheck->GetType());

				// ��Q���̌�����ݒ�
				rotObs = pObjCheck->GetVec3Rotation();

				// ��Q���̈ʒu��ݒ�
				posObs = pObjCheck->GetVec3Position();
				posObs.x += sinf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;
				posObs.y = 0.0f;
				posObs.z += cosf(rotObs.y + status.fAngleCenter) * status.fLengthCenter;

				// ��Q���̑傫����ݒ�
				sizeObsMax = status.sizeColl;
				sizeObsMin = sizeObsMax * -1.0f;

				// ��Q���Ƃ̔�������s
				collision::Square(posObs, &rPos, m_oldPos, rotObs, sizeObsMax, sizeObsMin);

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}

	// �������Ԃ�
	return bHit;
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CPlayer::LoadSetup(void)
{
	// �ϐ���錾
	CMotion::SMotionInfo info;		// �|�[�Y�̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p
	int nID			= 0;	// �C���f�b�N�X�̑���p
	int nParentID	= 0;	// �e�C���f�b�N�X�̑���p
	int nNowPose	= 0;	// ���݂̃|�[�Y�ԍ�
	int nNowKey		= 0;	// ���݂̃L�[�ԍ�
	int nLoop		= 0;	// ���[�v��ON/OFF�̕ϊ��p
	int nEnd		= 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �|�[�Y����p�̕ϐ���������
	memset(&info, 0, sizeof(info));

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �L�����N�^�[�̐ݒ�
			if (strcmp(&aString[0], "CHARACTERSET") == 0)
			{ // �ǂݍ��񂾕����� CHARACTERSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "PARTSSET") == 0)
					{ // �ǂݍ��񂾕����� PARTSSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "INDEX") == 0)
							{ // �ǂݍ��񂾕����� INDEX �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nID);			// ���f���̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "PARENT") == 0)
							{ // �ǂݍ��񂾕����� PARENT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nParentID);	// ���f���̐e�̃C���f�b�N�X��ǂݍ���
							}
							else if (strcmp(&aString[0], "POS") == 0)
							{ // �ǂݍ��񂾕����� POS �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &pos.x);		// X���W��ǂݍ���
								fscanf(pFile, "%f", &pos.y);		// Y���W��ǂݍ���
								fscanf(pFile, "%f", &pos.z);		// Z���W��ǂݍ���
							}
							else if (strcmp(&aString[0], "ROT") == 0)
							{ // �ǂݍ��񂾕����� ROT �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%f", &rot.x);		// X������ǂݍ���
								fscanf(pFile, "%f", &rot.y);		// Y������ǂݍ���
								fscanf(pFile, "%f", &rot.z);		// Z������ǂݍ���
							}
						} while (strcmp(&aString[0], "END_PARTSSET") != 0);	// �ǂݍ��񂾕����� END_PARTSSET �ł͂Ȃ��ꍇ���[�v

						// �p�[�c���̐ݒ�
						CObjectChara::SetPartsInfo(nID, nParentID, pos, rot, mc_apModelFile[nID]);
					}
				} while (strcmp(&aString[0], "END_CHARACTERSET") != 0);		// �ǂݍ��񂾕����� END_CHARACTERSET �ł͂Ȃ��ꍇ���[�v
			}

			// ���[�V�����̐ݒ�
			else if (strcmp(&aString[0], "MOTIONSET") == 0)
			{ // �ǂݍ��񂾕����� MOTIONSET �̏ꍇ

				// ���݂̃|�[�Y�ԍ���������
				nNowPose = 0;

				do
				{ // �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "LOOP") == 0)
					{ // �ǂݍ��񂾕����� LOOP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nLoop);		// ���[�v��ON/OFF��ǂݍ���

						// �ǂݍ��񂾒l��bool�^�ɕϊ�
						info.bLoop = (nLoop == 0) ? false : true;
					}
					else if (strcmp(&aString[0], "NUM_KEY") == 0)
					{ // �ǂݍ��񂾕����� NUM_KEY �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &info.nNumKey);	// �L�[�̑�����ǂݍ���
					}
					else if (strcmp(&aString[0], "KEYSET") == 0)
					{ // �ǂݍ��񂾕����� KEYSET �̏ꍇ

						// ���݂̃L�[�ԍ���������
						nNowKey = 0;

						do
						{ // �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "FRAME") == 0)
							{ // �ǂݍ��񂾕����� FRAME �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);						// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &info.aKeyInfo[nNowPose].nFrame);	// �L�[���؂�ւ��܂ł̃t���[������ǂݍ���
							}
							else if (strcmp(&aString[0], "KEY") == 0)
							{ // �ǂݍ��񂾕����� KEY �̏ꍇ

								do
								{ // �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

									// �t�@�C�����當�����ǂݍ���
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "POS") == 0)
									{ // �ǂݍ��񂾕����� POS �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.x);	// X�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.y);	// Y�ʒu��ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].pos.z);	// Z�ʒu��ǂݍ���

										// �ǂݍ��񂾈ʒu�Ƀp�[�c�̏����ʒu�����Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].pos += GetPartsPosition(nNowKey);
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{ // �ǂݍ��񂾕����� ROT �̏ꍇ

										fscanf(pFile, "%s", &aString[0]);									// = ��ǂݍ��� (�s�v)
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);	// X������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);	// Y������ǂݍ���
										fscanf(pFile, "%f", &info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);	// Z������ǂݍ���

										// �ǂݍ��񂾌����Ƀp�[�c�̏������������Z
										info.aKeyInfo[nNowPose].aKey[nNowKey].rot += GetPartsRotation(nNowKey);

										// ���������𐳋K��
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.x);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.y);
										useful::NormalizeRot(info.aKeyInfo[nNowPose].aKey[nNowKey].rot.z);
									}

								} while (strcmp(&aString[0], "END_KEY") != 0);	// �ǂݍ��񂾕����� END_KEY �ł͂Ȃ��ꍇ���[�v

								// ���݂̃L�[�ԍ������Z
								nNowKey++;
							}
						} while (strcmp(&aString[0], "END_KEYSET") != 0);	// �ǂݍ��񂾕����� END_KEYSET �ł͂Ȃ��ꍇ���[�v

						// ���݂̃|�[�Y�ԍ������Z
						nNowPose++;
					}
				} while (strcmp(&aString[0], "END_MOTIONSET") != 0);	// �ǂݍ��񂾕����� END_MOTIONSET �ł͂Ȃ��ꍇ���[�v

				// ���[�V�������̐ݒ�
				CObjectChara::SetMotionInfo(info);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�v���C���[�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
