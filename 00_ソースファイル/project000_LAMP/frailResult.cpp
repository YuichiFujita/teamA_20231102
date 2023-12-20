//============================================================
//
//	�t���C���G���g���[���� [flailEntry.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "frailResult.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "retentionManager.h"

//************************************************************
//	�q�N���X [CFlailEntry] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlailResult::CFlailResult()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CFlailResult::~CFlailResult()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFlailResult::Init(void)
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
void CFlailResult::Uninit(void)
{
	// �t���C���̏I��
	CFlail::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlailResult::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CFlailResult::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 defViewport;		// �J�����̃r���[�|�[�g�ۑ��p
	D3DVIEWPORT9 currentViewport;	// ���݂̃r���[�|�[�g�ۑ��p
	int nPlayerID = CManager::GetInstance()->GetRetentionManager()->GetWinRank1st();	// �v���C���[�C���f�b�N�X

									// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CCamera *pCamera = CManager::GetInstance()->GetCamera();	// �J�����̃|�C���^

																// �J�����̃r���[�|�[�g�̈ʒu��ݒ�
	currentViewport = pCamera->GetViewport(CCamera::TYPE_RESULT);	// �r���[�|�[�g�擾
	currentViewport.X = (nPlayerID);								// ����ʒu��ݒ�
	pCamera->SetViewport(CCamera::TYPE_RESULT, currentViewport);		// �r���[�|�[�g�ݒ�

	if (CManager::GetInstance()->GetRetentionManager()->IsEntry(nPlayerID))
	{ // ���݂̃t���C�����G���g���[���Ă���ꍇ

	  // ���݂̃r���[�|�[�g���擾
		pDevice->GetViewport(&defViewport);

		// �J�����̐ݒ�
		CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_RESULT);

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
