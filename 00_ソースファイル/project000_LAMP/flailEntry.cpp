//============================================================
//
//	�t���C���G���g���[���� [flailEntry.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "flailEntry.h"
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
//	�q�N���X [CFlailEntry] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlailEntry::CFlailEntry()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFlailEntry::~CFlailEntry()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFlailEntry::Init(void)
{
	// �t���C���̏�����
	if (FAILED(CFlail::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�̕\����OFF�ɂ���
	SetEnableDepthShadow(false);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFlailEntry::Uninit(void)
{
	// �t���C���̏I��
	CFlail::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlailEntry::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CFlailEntry::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 defViewport;		// �J�����̃r���[�|�[�g�ۑ��p
	D3DVIEWPORT9 currentViewport;	// ���݂̃r���[�|�[�g�ۑ��p
	int nPlayerID = GetPlayerID();	// �v���C���[�C���f�b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// �J�����̃|�C���^

	// �J�����̃r���[�|�[�g�̈ʒu��ݒ�
	currentViewport = pCamera->GetViewport(CCamera::TYPE_ENTRY);	// �r���[�|�[�g�擾
	currentViewport.X = (nPlayerID * SPACE_WIDTH);					// ����ʒu��ݒ�
	pCamera->SetViewport(CCamera::TYPE_ENTRY, currentViewport);		// �r���[�|�[�g�ݒ�

	if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nPlayerID))
	{ // ���݂̃t���C�����G���g���[���Ă���ꍇ

		// ���݂̃r���[�|�[�g���擾
		pDevice->GetViewport(&defViewport);

		// �J�����̐ݒ�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_ENTRY);

		// �e�̖_���f���̃}�g���b�N�X���擾
		D3DXMATRIX mtxStick = CScene::GetPlayer(nPlayerID)->GetMultiModel(CPlayer::MODEL_STICK)->GetMtxWorld();

		// �t���C���ʒu��ݒ�
		SetVec3Position(D3DXVECTOR3(mtxStick._41, mtxStick._42, mtxStick._43));

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectModel::Draw();

		// �J�����̐ݒ�����ɖ߂�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �r���[�|�[�g�����ɖ߂�
		pDevice->SetViewport(&defViewport);
	}
}
