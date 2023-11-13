//============================================================
//
//	���f��UI���� [flail.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "flail.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MODEL_UI_PRIO	(14)	// ���f��UI�\���̗D�揇��

//************************************************************
//	�q�N���X [CFlail] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, MODEL_UI_PRIO)
{
	m_move = VEC3_ZERO;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFlail::~CFlail()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFlail::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	BindModel("data\\MODEL\\OBSTACLE\\obstacle017.x");

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFlail::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlail::Update(void)
{
	SetVec3Position(GetVec3Position() + m_move);

	m_move.x += (0.0f - m_move.x) * 0.15f;
	m_move.z += (0.0f - m_move.z) * 0.15f;

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CFlail::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���݂̃r���[�|�[�g���擾
	pDevice->GetViewport(&viewportDef);

	// �J�����̐ݒ�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();

	// �J�����̐ݒ�����ɖ߂�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	��������
//============================================================
CFlail *CFlail::Create
( // ����
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �傫��
)
{
	// �|�C���^��錾
	CFlail *pModelUI = NULL;	// ���f��UI�����p

	if (pModelUI == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pModelUI = new CFlail;	// ���f��UI
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pModelUI != NULL)
	{ // �g�p����Ă���ꍇ
		
		// ���f��UI�̏�����
		if (FAILED(pModelUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pModelUI;
			pModelUI = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pModelUI->SetVec3Position(rPos);

		// ������ݒ�
		pModelUI->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pModelUI->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pModelUI;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CFlail::SetVec3Move(const D3DXVECTOR3& rMove)
{
	// �����̈ʒu��ݒ�
	m_move = rMove;
}

//============================================================
//	�ړ��ʂ̎擾����
//============================================================
D3DXVECTOR3 CFlail::GetVec3Move(void)
{
	// �ʒu��Ԃ�
	return m_move;
}