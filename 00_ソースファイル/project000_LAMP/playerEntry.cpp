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
	D3DVIEWPORT9 viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���݂̃r���[�|�[�g���擾
	pDevice->GetViewport(&viewportDef);

	// �J�����̐ݒ�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_ENTRY);

	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw();

	// �J�����̐ݒ�����ɖ߂�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	�G���g���[�̐ݒ菈��
//============================================================
void CPlayerEntry::SetEntry(void)
{
#if 0

	// �ϐ���錾
	D3DXVECTOR3 set = VEC3_ZERO;	// �����ݒ�p

	// ����������
	SetState(STATE_NONE);	// �������Ȃ���Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ʒu��ݒ�
	SetVec3Position(set);

	// ������ݒ�
	SetVec3Rotation(set);
	m_destRot = set;

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �����`���OFF�ɂ���
	SetEnableDraw(false);

#endif
}
