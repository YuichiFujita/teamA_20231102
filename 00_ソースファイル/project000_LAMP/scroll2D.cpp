//============================================================
//
//	�X�N���[��2D���� [scroll2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scroll2D.h"

//************************************************************
//	�q�N���X [CScroll2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScroll2D::CScroll2D()
{
	// �����o�ϐ����N���A
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CScroll2D::CScroll2D(const CObject::ELabel label, const int nPriority) : CObject2D(label, nPriority)
{
	// �����o�ϐ����N���A
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CScroll2D::~CScroll2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScroll2D::Init(void)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = 0.0f;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = 0.0f;	// �e�N�X�`���c���W�̈ړ���

	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	���������� (�I�[�o�[���[�h)
//============================================================
HRESULT CScroll2D::Init(const float fMoveU, const float fMoveV)
{
	// �����o�ϐ���������
	m_fTexU  = 0.0f;	// �e�N�X�`�������W�̊J�n�ʒu
	m_fTexV  = 0.0f;	// �e�N�X�`���c���W�̊J�n�ʒu
	m_fMoveU = fMoveU;	// �e�N�X�`�������W�̈ړ���
	m_fMoveV = fMoveV;	// �e�N�X�`���c���W�̈ړ���

	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CScroll2D::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScroll2D::Update(void)
{
	// �X�N���[�������Z
	m_fTexU += m_fMoveU;
	m_fTexV += m_fMoveV;

	if (m_fTexU > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexU -= 1.0f;
	}

	if (m_fTexV > 1.0f)
	{ // 1.0f���傫���Ȃ����ꍇ

		// �J�n�n�_��␳
		m_fTexV -= 1.0f;
	}

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update();

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�`�揈��
//============================================================
void CScroll2D::Draw(void)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CScroll2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �ʒu�̐ݒ�
	CObject2D::SetVec3Position(rPos);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CScroll2D::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̐ݒ�
	CObject2D::SetVec3Rotation(rRot);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CScroll2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �傫���̐ݒ�
	CObject2D::SetVec3Sizing(rSize);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CScroll2D::SetColor(const D3DXCOLOR& rCol)
{
	// �F�̐ݒ�
	CObject2D::SetColor(rCol);

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	��������
//============================================================
CScroll2D *CScroll2D::Create(const float fMoveU, const float fMoveV, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CScroll2D *pScroll2D = NULL;	// �X�N���[��2D�����p

	if (pScroll2D == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pScroll2D = new CScroll2D;	// �X�N���[��2D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pScroll2D != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �X�N���[��2D�̏�����
		if (FAILED(pScroll2D->Init(fMoveU, fMoveV)))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pScroll2D;
			pScroll2D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pScroll2D->SetVec3Position(rPos);

		// ������ݒ�
		pScroll2D->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pScroll2D->SetVec3Sizing(rSize);

		// �F��ݒ�
		pScroll2D->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pScroll2D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScroll2D::SetTexU(const float fTexU)
{
	// �����̉����W�̊J�n�ʒu����
	m_fTexU = fTexU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̊J�n�ʒu�ݒ菈��
//============================================================
void CScroll2D::SetTexV(const float fTexV)
{
	// �����̏c���W�̊J�n�ʒu����
	m_fTexV = fTexV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�����W�̈ړ��ʐݒ菈��
//============================================================
void CScroll2D::SetMoveU(const float fMoveU)
{
	// �����̉����W�̈ړ��ʂ���
	m_fMoveU = fMoveU;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}

//============================================================
//	�c���W�̈ړ��ʐݒ菈��
//============================================================
void CScroll2D::SetMoveV(const float fMoveV)
{
	// �����̏c���W�̈ړ��ʂ���
	m_fMoveV = fMoveV;

	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	CObject2D::SetScrollTex(m_fTexU, m_fTexV);
}
