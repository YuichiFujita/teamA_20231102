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

#include "multiModel.h"
#include "objectOrbit.h"
#include "shadow.h"
#include "object2D.h"
#include "timerManager.h"
#include "rankingManager.h"
#include "stage.h"
#include "field.h"

#include "effect3D.h"
#include "particle3D.h"

#include "flail.h"

#include "spawnpoint.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\player.txt";	// �v���C���[�Z�b�g�A�b�v�e�L�X�g

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	GRAVITY		= 1.0f;		// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float	HEIGHT		= 100.0f;	// �c��

	const float	DASH_UP			= 10.0f;	// �_�b�V���㏸��
	const float	DASH_SIDE		= 15.0f;	// �_�b�V�����ړ���
	const float	DASH_REV		= 0.25f;	// �_�b�V���̌��Z�W��
	const float	DASH_MINMOVE	= 0.06f;	// �_�b�V���ēx�\�ɂȂ�ړ���

	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	STICK_REV	= 0.00015f;	// �X�e�B�b�N�̌X���ʂ̕␳�W��

	const float	DEAD_ZONE	= (float)USHRT_MAX * 0.01f;	// �X�e�B�b�N�̖�������X����
	const float	SPAWN_ADD_ALPHA		= 0.03f;			// �X�|�[����Ԏ��̓����x�̉��Z��

	// �e�N���X���
	namespace shadow
	{
		const D3DXVECTOR3 SIZE = D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��
	}
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
	m_pShadow			= NULL;			// �e�̏��
	m_oldPos			= VEC3_ZERO;	// �ߋ��ʒu
	m_move				= VEC3_ZERO;	// �ړ���
	m_destRot			= VEC3_ZERO;	// �ڕW����
	m_dashRot			= VEC3_ZERO;	// �_�b�V������
	m_state				= STATE_NONE;	// ���
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterFlail		= 0;			// �t���C���Ǘ��J�E���^�[
	m_fPlusMove			= 0.0f;			// �v���X�ړ���
	m_bDash				= false;		// �_�b�V����
	m_bJump				= false;		// �W�����v��
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
	m_pShadow			= NULL;			// �e�̏��
	m_oldPos			= VEC3_ZERO;	// �ߋ��ʒu
	m_move				= VEC3_ZERO;	// �ړ���
	m_destRot			= VEC3_ZERO;	// �ڕW����
	m_dashRot			= VEC3_ZERO;	// �_�b�V������
	m_state				= STATE_NONE;	// ���
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[
	m_nCounterFlail		= 0;			// �t���C���Ǘ��J�E���^�[
	m_fPlusMove			= 0.0f;			// �v���X�ړ���
	m_bDash				= false;		// �_�b�V����
	m_bJump				= true;			// �W�����v��

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

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, shadow::SIZE, this);
	if (m_pShadow == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t���C���̐���
	m_pFlail = CFlail::Create(VEC3_ZERO);
	if (m_pFlail == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}
	m_pFlail->SetPlayerID(m_nPadID);

	// ���C���J���[��ݒ�
	SetMainMaterial();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �e�̏I��
	m_pShadow->Uninit();

	// �t���C���̏I��
	m_pFlail->Uninit();

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal();

		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	m_pShadow->Update();

	// �t���C���̍X�V
	D3DXMATRIX partsMtx = GetMultiModel(MODEL_HAND_R)->GetMtxWorld();
	D3DXVECTOR3 partsPos = D3DXVECTOR3(partsMtx._41, partsMtx._42, partsMtx._43);
	m_pFlail->SetVec3PosOrg(partsPos);

	if (D3DXVec3Length(&m_move) > 1.0f && m_nCounterFlail < 0)
	{
		D3DXVECTOR3 vecFlail = m_pFlail->GetVec3Position() - (m_pFlail->GetVec3PosOrg());
		float length, chainRot;

		vecFlail.y = 0.0f;
		length = D3DXVec3Length(&vecFlail);
		chainRot = atan2f(vecFlail.x, vecFlail.z);

		m_pFlail->SetChainRot(chainRot);
		m_pFlail->SetChainRotMove(chainRot);
		m_pFlail->SetLengthChain(length - 1.0f);
		m_pFlail->SetMove(VEC3_ZERO);
	}

	m_pFlail->Update();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(void)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();
}

//============================================================
//	�q�b�g����
//============================================================
void CPlayer::Hit(void)
{
	// �ϐ���錾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

#if 0

	if (IsDeath() != true)
	{ // ���S�t���O�������Ă��Ȃ��ꍇ

		if (m_state == STATE_NORMAL)
		{ // �ʏ��Ԃ̏ꍇ

			// �J�E���^�[��������
			m_nCounterState = 0;

			// �ҋ@���[�V������ݒ�
			SetMotion(MOTION_IDOL);

			// �����p�[�e�B�N���𐶐�
			CParticle3D::Create(CParticle3D::TYPE_SMALL_EXPLOSION, D3DXVECTOR3(posPlayer.x, posPlayer.y + basic::HEIGHT * 0.5f, posPlayer.z));

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT);	// �q�b�g��
		}
	}

#endif
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const int nState)
{
	if (nState > NONE_IDX && nState < STATE_MAX)
	{ // �͈͓��̏ꍇ

		// �����̏�Ԃ�ݒ�
		m_state = (EState)nState;
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
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
	m_pFlail->SetEnableUpdate(bUpdate);		// �t���C��
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
	m_pFlail->SetEnableDraw(bDraw);		// �t���C��
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
	if (pSpawnPoint == NULL)
	{ // �X�|�[���|�C���g�������ꍇ

		assert(false);
	}

	// ����������
	SetState(STATE_SPAWN);		// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);		// �ҋ@���[�V������ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ʒu��ݒ�
	SetVec3Position(pSpawnPoint->GetVec3Position());

	// ������ݒ�
	SetVec3Rotation(pSpawnPoint->GetVec3Rotation());
	m_destRot = pSpawnPoint->GetVec3Rotation();

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �}�e���A�����Đݒ�
	ResetMaterial();
	
	// ���C���J���[��ݒ�
	SetMainMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �����`���ON�ɂ���
	SetEnableDraw(true);

	// �����낵�J�����̖ڕW�ʒu�̐ݒ�
	CManager::GetInstance()->GetCamera()->SetDestLookDown();

	// �T�E���h�̍Đ�
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SPAWN);	// ������
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
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update();
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	if (UpdateFadeOut(SPAWN_ADD_ALPHA))
	{ // �s�����ɂȂ�؂����ꍇ

		// ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
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

	// �ړ�����
	currentMotion = UpdateMove(posPlayer);

	// �d�͂̍X�V
	UpdateGravity();

	// ���n����
	UpdateLanding(posPlayer);

	// �_�b�V���X�V
	UpdateDash();

	// �����X�V
	UpdateRotation(rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
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
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h
	CCamera			*pCamera	= CManager::GetInstance()->GetCamera();		// �J����

	if (pCamera == NULL) { assert(false); return MOTION_IDOL; }	// ��g�p��

	// PC����
#if 0
	if (pKeyboard->IsPress(DIK_W))
	{
		if (pKeyboard->IsPress(DIK_A))
		{
			// �ړ��ʂ��X�V
			m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{
			// �ړ��ʂ��X�V
			m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
		}
		else
		{
			// �ړ��ʂ��X�V
			m_move.x += sinf(pCamera->rot.y) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y) * MOVE_PLAYER;
		}
	}
	else if (pKeyboard->IsPress(DIK_S))
	{
		if (pKeyboard->IsPress(DIK_A))
		{
			// �ړ��ʂ��X�V
			m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
			m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * MOVE_PLAYER;
		}
		else if (pKeyboard->IsPress(DIK_D))
		{
			// �ړ��ʂ��X�V
			m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * MOVE_PLAYER;
		}
		else
		{
			// �ړ��ʂ��X�V
			m_move.x -= sinf(pCamera->rot.y) * MOVE_PLAYER;
			m_move.z -= cosf(pCamera->rot.y) * MOVE_PLAYER;
		}
	}
	else if (pKeyboard->IsPress(DIK_A))
	{
		// �ړ��ʂ��X�V
		m_move.x += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		m_move.z += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
	}
	else if (pKeyboard->IsPress(DIK_D))
	{
		// �ړ��ʂ��X�V
		m_move.x -= sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
		m_move.z -= cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * MOVE_PLAYER;
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
		}
		else
		{ // �_�b�V�����̏ꍇ

			// �ړ��ʂ��X�V
			m_move.x += sinf(m_dashRot.y) * fMove;
			m_move.z += cosf(m_dashRot.y) * fMove;
		}

		if (m_pFlail->GetLengthChain() >= 1000.0f)
		{
			// �ړ��ʂ��X�V
			m_move.x *= 0.7f;
			m_move.z *= 0.7f;
		}

		// �ڕW������ݒ�
		m_destRot.y = atan2f(-m_move.x, -m_move.z);
	}

	// �J�E���^�[�̒l�ɂ���ċ�����ύX
	if (m_nCounterFlail > 0)
	{// 0���傫����

		if ((CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail <= 60)
		{// ������{�^����������Ă��鎞
		 // �J�E���^�[�A�b�v
			m_nCounterFlail++;

			// ���l�ŃJ�E���^�[���~�߂�
			if (m_nCounterFlail > 60)
			{
				m_nCounterFlail = 60;
			}

			// ���߂Ă�ԓS����U���
			m_pFlail->SetChainRot(m_pFlail->GetChainRot() - (0.01f * m_nCounterFlail));
			m_pFlail->SetLengthChain(2.0f * m_nCounterFlail);

			// �ړ��ʂ��X�V
			m_move.x *= 0.5f;
			m_move.z *= 0.5f;
		}

		// ����
		if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE) && m_nCounterFlail != 120)
		{
			// ���߂����Ԃɉ����Ĕ򋗗�����
			D3DXVECTOR3 move = VEC3_ZERO;
			move.x = (sinf(m_pFlail->GetChainRotMove()) * 5.0f * m_nCounterFlail);
			move.z = (cosf(m_pFlail->GetChainRotMove()) * 5.0f * m_nCounterFlail);
			m_pFlail->SetMove(move);

			// �ڕW�p�x�ɍ��킹��
			m_pFlail->SetChainRot(m_pFlail->GetChainRotMove());

			// �J�E���^�[�̐ݒ�
			m_nCounterFlail = 120;
		}

		if (m_nCounterFlail == 120)
		{
			// �ړ��ʂ��X�V
			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// �t���C�����~�܂�����J�E���^�[�����̒i�K��
			if (D3DXVec3Length(&m_pFlail->GetMove()) < 1.0f)
			{
				m_nCounterFlail = -1;
			}
		}
	}
	else if (m_nCounterFlail == 0)
	{
		// �J�E���^�[�A�b�v�J�n
		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_R1, m_nPadID) == TRUE)
		{
			m_nCounterFlail++;
		}

		m_pFlail->SetLengthChain(0.0f);
	}
	else
	{
		// �S���ƃv���C���[�̋�������薢���̎��v���C���[�ʒu�ɓS���Œ�
		if (m_pFlail->GetLengthChain() < 50.0f)
		{
			m_nCounterFlail = 1;
			m_pFlail->SetMove(VEC3_ZERO);
			m_pFlail->SetLengthChain(0.0f);
		}

		// �����߂�
		if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsPress(CInputPad::KEY_R1, m_nPadID) == TRUE)
		{
			m_nCounterFlail -= 1;

			if (m_nCounterFlail < -50)
			{
				m_nCounterFlail = -50;
			}

			// ���߂����Ԃɉ����Ĕ򋗗�����
			D3DXVECTOR3 move = VEC3_ZERO;
			move.x = (sinf(m_pFlail->GetChainRotMove()) * (m_nCounterFlail * -m_nCounterFlail));
			move.z = (cosf(m_pFlail->GetChainRotMove()) * (m_nCounterFlail * -m_nCounterFlail));
			m_pFlail->SetMove(move);

			// �ړ��ʂ��X�V
			m_move.x = 0.0f;
			m_move.z = 0.0f;
		}

		// ����
		if ((CManager::GetInstance()->GetKeyboard()->IsRelease(DIK_SPACE) == TRUE || CManager::GetInstance()->GetPad()->IsRelease(CInputPad::KEY_R1, m_nPadID) == TRUE))
		{
			m_nCounterFlail = -1;
		}
	}

	vecStick = D3DXVECTOR3((float)pPad->GetPressRStickX(m_nPadID), (float)pPad->GetPressRStickY(m_nPadID), 0.0f);	// �X�e�B�b�N�e���̓|����
	fStick = sqrtf(vecStick.x * vecStick.x + vecStick.y * vecStick.y) * 0.5f;	// �X�e�B�b�N�̓|����

	if (DEAD_ZONE < fStick)
	{ // �f�b�h�]�[���ȏ�̏ꍇ
		m_pFlail->SetChainRotMove(CManager::GetInstance()->GetPad()->GetPressRStickRot(m_nPadID) + 1.57f);
	}

	// �ڕW�p�x�ύX����
	if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_A) == TRUE)
	{
		m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove() - 0.015f);
	}
	else if (CManager::GetInstance()->GetKeyboard()->IsPress(DIK_D) == TRUE)
	{
		m_pFlail->SetChainRotMove(m_pFlail->GetChainRotMove() + 0.015f);
	}

	// �ʒu��\��
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�ʒu]�F%f %f %f\n", rPos.x, rPos.y, rPos.z);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "[�J�E���^�[]�F%d\n", m_nCounterFlail);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
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

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(rPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(rPos, m_move, 0.0f))
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

				// �u���b�N�̍ŏ��̑傫����ݒ�
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

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}

	// ���n�󋵂�Ԃ�
	return bLand;
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
