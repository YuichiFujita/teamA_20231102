//============================================================
//
//	����UI���� [valueUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "valueUI.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "object2D.h"
#include "multiValue.h"

//************************************************************
//	�}�N����`
//************************************************************
#define VALUE_UI_PRIO	(13)	// ����UI�\���̗D�揇��

//************************************************************
//	�q�N���X [CValueUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CValueUI::CValueUI() : CObject(CObject::LABEL_NONE, VALUE_UI_PRIO)
{
	// �����o�ϐ����N���A
	m_pTitle = NULL;		// �^�C�g�����
	m_pValue = NULL;		// �������
	m_pos = VEC3_ZERO;		// �ʒu
	m_space = VEC3_ZERO;	// �s��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CValueUI::~CValueUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CValueUI::Init(void)
{
	// �����o�ϐ���������
	m_pTitle = NULL;		// �^�C�g�����
	m_pValue = NULL;		// �������
	m_pos = VEC3_ZERO;		// �ʒu
	m_space = VEC3_ZERO;	// �s��

	// �^�C�g�����̐���
	m_pTitle = CObject2D::Create(VEC3_ZERO);
	if (m_pTitle == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pTitle->SetPriority(VALUE_UI_PRIO);

	// �������̐���
	m_pValue = CMultiValue::Create(CValue::TEXTURE_NORMAL, 0, 1, VEC3_ZERO, VEC3_ONE, VEC3_ZERO);
	if (m_pValue == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pValue->SetPriority(VALUE_UI_PRIO);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CValueUI::Uninit(void)
{
	// �^�C�g�����̏I��
	m_pTitle->Uninit();

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CValueUI::Update(void)
{
	// �^�C�g�����̍X�V
	m_pTitle->Update();

	// �������̍X�V
	m_pValue->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CValueUI::Draw(void)
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CValueUI::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g
	m_pTitle->SetEnableDraw(bDraw);	// �^�C�g�����
	m_pValue->SetEnableDraw(bDraw);	// �������
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CValueUI::SetPriority(const int nPriority)
{
	// �����̗D�揇�ʂ�ݒ�
	CObject::SetPriority(nPriority);	// ���g
	m_pTitle->SetPriority(nPriority);	// �^�C�g�����
	m_pValue->SetPriority(nPriority);	// �������
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CValueUI::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CValueUI::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	��������
//============================================================
CValueUI *CValueUI::Create
(
	const char *pPassTex,			// �^�C�g���e�N�X�`���p�X
	const CValue::ETexture texture,	// �����e�N�X�`��
	const int nDigit,				// ����
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSpace,		// �s��
	const D3DXVECTOR3& rSpaceValue,	// �����s��
	const D3DXVECTOR3& rSizeTitle,	// �^�C�g���傫��
	const D3DXVECTOR3& rSizeValue,	// �����傫��
	const D3DXVECTOR3& rRotTitle,	// �^�C�g������
	const D3DXVECTOR3& rRotValue,	// ��������
	const D3DXCOLOR& rColTitle,		// �^�C�g���F
	const D3DXCOLOR& rColValue		// �����F
)
{
	// �|�C���^��錾
	CValueUI *pValueUI = NULL;	// ����UI�����p

	if (pValueUI == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pValueUI = new CValueUI;	// ����UI
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pValueUI != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ����UI�̏�����
		if (FAILED(pValueUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pValueUI;
			pValueUI = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �����̌�����ݒ�
		pValueUI->GetMultiValue()->SetDigit(nDigit);

		// �^�C�g���e�N�X�`����ݒ�
		pValueUI->SetTextureTitle(pPassTex);

		// �����e�N�X�`����ݒ�
		pValueUI->GetMultiValue()->SetTexture(texture);

		// �^�C�g��������ݒ�
		pValueUI->SetRotationTitle(rRotTitle);

		// ����������ݒ�
		pValueUI->GetMultiValue()->SetVec3Rotation(rRotValue);

		// �^�C�g���傫����ݒ�
		pValueUI->SetScalingTitle(rSizeTitle);

		// �����傫����ݒ�
		pValueUI->GetMultiValue()->SetVec3Sizing(rSizeValue);

		// �^�C�g���F��ݒ�
		pValueUI->SetColorTitle(rColTitle);

		// �����F��ݒ�
		pValueUI->GetMultiValue()->SetColor(rColValue);

		// �����̍s�Ԃ�ݒ�
		pValueUI->GetMultiValue()->SetSpace(rSpaceValue);

		// �s�Ԃ�ݒ�
		pValueUI->SetSpace(rSpace);

		// �ʒu��ݒ�
		pValueUI->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pValueUI;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�s�Ԃ̐ݒ菈��
//============================================================
void CValueUI::SetSpace(const D3DXVECTOR3& rSpace)
{
	// �����̍s�Ԃ�ݒ�
	m_space = rSpace;

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�s�Ԏ擾����
//============================================================
D3DXVECTOR3 CValueUI::GetSpace(void) const
{
	// �s�Ԃ�Ԃ�
	return m_space;
}

//============================================================
//	�^�C�g�������̐ݒ菈��
//============================================================
void CValueUI::SetRotationTitle(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_pTitle->SetVec3Rotation(rRot);
}

//============================================================
//	�^�C�g�������擾����
//============================================================
D3DXVECTOR3 CValueUI::GetRotationTitle(void) const
{
	// �^�C�g��������Ԃ�
	return m_pTitle->GetVec3Rotation();
}

//============================================================
//	�^�C�g���傫���̐ݒ菈��
//============================================================
void CValueUI::SetScalingTitle(const D3DXVECTOR3& rSize)
{
	// �����̑傫����ݒ�
	m_pTitle->SetVec3Sizing(rSize);
}

//============================================================
//	�^�C�g���傫���擾����
//============================================================
D3DXVECTOR3 CValueUI::GetScalingTitle(void) const
{
	// �^�C�g���傫����Ԃ�
	return m_pTitle->GetVec3Sizing();
}

//============================================================
//	�^�C�g���F�̐ݒ菈��
//============================================================
void CValueUI::SetColorTitle(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_pTitle->SetColor(rCol);
}

//============================================================
//	�^�C�g���F�擾����
//============================================================
D3DXCOLOR CValueUI::GetColorTitle(void) const
{
	// �^�C�g���F��Ԃ�
	return m_pTitle->GetColor();
}

//============================================================
//	�^�C�g���e�N�X�`���̐ݒ菈��
//============================================================
void CValueUI::SetTextureTitle(const char *pPassTex)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����̃e�N�X�`��������
	m_pTitle->BindTexture(pTexture->Regist(pPassTex));
}

//============================================================
//	�������擾����
//============================================================
CMultiValue *CValueUI::GetMultiValue(void) const
{
	// ��������Ԃ�
	return m_pValue;
}

//============================================================
//	�j������
//============================================================
void CValueUI::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CValueUI::SetPositionRelative(void)
{
	// �^�C�g���̈ʒu��ݒ�
	m_pTitle->SetVec3Position(m_pos);

	// �����̈ʒu��ݒ�
	m_pValue->SetVec3Position(m_pTitle->GetVec3Position() + m_space);
}
