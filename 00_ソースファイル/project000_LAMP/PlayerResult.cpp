//============================================================
//
//	�v���C���[�G���g���[���� [playerEntry.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "PlayerResult.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "flail.h"
#include "retentionManager.h"

//************************************************************
//	�q�N���X [CPlayerEntry] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerResult::CPlayerResult(const CScene::EMode mode, const int nPad) : CPlayer(mode, nPad)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerResult::~CPlayerResult()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerResult::Init(void)
{
	// �v���C���[�̏�����
	if (FAILED(CPlayer::Init()))
	{ // �������Ɏ��s�����ꍇ

	  // ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �G���g���[�̐ݒ�
	SetEntry();

	// �e�̕\����OFF�ɂ���
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	//SetPriority(14);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerResult::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerResult::Update(void)
{
	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(CPlayer::MOTION_IDOL);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerResult::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 defViewport;		// �J�����̃r���[�|�[�g�ۑ��p
	D3DVIEWPORT9 currentViewport;	// ���݂̃r���[�|�[�g�ۑ��p

	int nPadID = CManager::GetInstance()->GetRetentionManager()->GetWinRank1st();

									// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// �J�����̃|�C���^

	// �J�����̃r���[�|�[�g�̈ʒu��ݒ�
	currentViewport = pCamera->GetViewport(CCamera::TYPE_RESULT);	// �r���[�|�[�g�擾
	currentViewport.X = (nPadID);						// ����ʒu��ݒ�
	pCamera->SetViewport(CCamera::TYPE_RESULT, currentViewport);		// �r���[�|�[�g�ݒ�

	if (true)
	{ // ���݂̃v���C���[���G���g���[���Ă���ꍇ

	  // ���݂̃r���[�|�[�g���擾
		pDevice->GetViewport(&defViewport);

		// �J�����̐ݒ�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_RESULT);

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw();

		// �J�����̐ݒ�����ɖ߂�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �r���[�|�[�g�����ɖ߂�
		pDevice->SetViewport(&defViewport);
	}
}

//============================================================
//	�G���g���[�̐ݒ菈��
//============================================================
void CPlayerResult::SetEntry(void)
{
	// �ϐ���錾
	D3DXVECTOR3 set = VEC3_ZERO;	// �����ݒ�p

									// ����������
	SetState(STATE_NONE);	// �������Ȃ���Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�

							// �ʒu��ݒ�
	SetVec3Position(set);

	// ������ݒ�
	SetVec3Rotation(set);

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// ���C���J���[��ݒ�
	SetMainMaterial();

	// �����x��s�����ɐݒ�
	SetAlpha(1.0f);

	// �v���C���[�̕t���i�̎����`����~
	CPlayer::SetEnableDrawUI(false);
	CPlayer::SetEnableDraw(false);
	CObject::SetEnableDraw(true);		// �v���C���[�͕`��ON
	GetFlail()->SetEnableDraw(true);	// �t���C�����`��ON
}