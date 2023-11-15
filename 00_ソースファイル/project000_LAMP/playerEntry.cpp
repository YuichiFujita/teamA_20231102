//============================================================
//
//	�v���C���[�G���g���[���� [playerEntry.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "playerEntry.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "retentionManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int SPACE_WIDTH = 320;	// ��
}

//************************************************************
//	�q�N���X [CPlayerEntry] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerEntry::CPlayerEntry(const int nPad) : CPlayer(nPad)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerEntry::~CPlayerEntry()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerEntry::Init(void)
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

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerEntry::Uninit(void)
{
	// �v���C���[�̏I��
	CPlayer::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayerEntry::Update(void)
{
	// �v���C���[�̍X�V
	CPlayer::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerEntry::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 defViewport;		// �J�����̃r���[�|�[�g�ۑ��p
	D3DVIEWPORT9 currentViewport;	// ���݂̃r���[�|�[�g�ۑ��p
	int nPadID = GetPadID();		// �p�b�h�C���f�b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// �J�����̃|�C���^

	// �J�����̃r���[�|�[�g�̈ʒu��ݒ�
	currentViewport = pCamera->GetViewport(CCamera::TYPE_ENTRY);	// �r���[�|�[�g�擾
	currentViewport.X = (nPadID * SPACE_WIDTH);						// ����ʒu��ݒ�
	pCamera->SetViewport(CCamera::TYPE_ENTRY, currentViewport);		// �r���[�|�[�g�ݒ�

	if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nPadID))
	{ // ���݂̃v���C���[���G���g���[���Ă���ꍇ

		// ���݂̃r���[�|�[�g���擾
		pDevice->GetViewport(&defViewport);

		// �J�����̐ݒ�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_ENTRY);

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
void CPlayerEntry::SetEntry(void)
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

	// �����x��s�����ɐݒ�
	SetAlpha(1.0f);
}
