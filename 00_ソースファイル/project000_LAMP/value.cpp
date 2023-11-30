//============================================================
//
//	�������� [value.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "value.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define VAL_PRIO	(13)	// �����̗D�揇��
#define PTRN_WIDTH	(10)	// �e�N�X�`���̉��̕�����
#define PTRN_HEIGHT	(1)		// �e�N�X�`���̏c�̕�����

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CValue::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\number000.png",	// �ʏ�e�N�X�`��
	"data\\TEXTURE\\number001.png",	// �ʏ�e�N�X�`��
};

//************************************************************
//	�q�N���X [CValue] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CValue::CValue() : CAnim2D(CObject::LABEL_NONE, VAL_PRIO)
{
	// �����o�ϐ����N���A
	m_nNum = 0;	// �\�����l
}

//============================================================
//	�f�X�g���N�^
//============================================================
CValue::~CValue()
{

}

//============================================================
//	����������
//============================================================
HRESULT CValue::Init(void)
{
	// �����o�ϐ���������
	m_nNum = 0;	// �\�����l

	// �A�j���[�V����2D�̏�����
	if (FAILED(CAnim2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`�����������̐ݒ�
	CAnim2D::SetWidthPattern(PTRN_WIDTH);

	// �e�N�X�`���c�������̐ݒ�
	CAnim2D::SetHeightPattern(PTRN_HEIGHT);

	// ��~�󋵂̐ݒ�
	CAnim2D::SetEnableStop(true);	// �A�j���[�V�������Ȃ�

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CValue::Uninit(void)
{
	// �A�j���[�V����2D�̏I��
	CAnim2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CValue::Update(void)
{
	// �A�j���[�V����2D�̍X�V
	CAnim2D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CValue::Draw(void)
{
	// �A�j���[�V����2D�̕`��
	CAnim2D::Draw();
}

//============================================================
//	��������
//============================================================
CValue *CValue::Create(const ETexture texture)
{
	// �|�C���^��錾
	CValue *pValue = NULL;		// ���������p

	if (pValue == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pValue = new CValue;	// ����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pValue != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �����̏�����
		if (FAILED(pValue->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pValue;
			pValue = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����ݒ�
		pValue->SetTexture(texture);

		// �m�ۂ����A�h���X��Ԃ�
		return pValue;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	��������
//============================================================
CValue *CValue::Create(const ETexture texture, const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	// �|�C���^��錾
	CValue *pValue = NULL;		// ���������p

	if (pValue == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pValue = new CValue;	// ����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pValue != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �����̏�����
		if (FAILED(pValue->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pValue;
			pValue = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����ݒ�
		pValue->SetTexture(texture);

		// �ʒu��ݒ�
		pValue->SetVec3Position(rPos);

		// ������ݒ�
		pValue->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pValue->SetVec3Sizing(rSize);

		// �F��ݒ�
		pValue->SetColor(rCol);

		// �m�ۂ����A�h���X��Ԃ�
		return pValue;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`���̐ݒ菈��
//============================================================
void CValue::SetTexture(const ETexture texture)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �e�N�X�`����o�^�E����
	BindTexture(pTexture->Regist(mc_apTextureFile[texture]));
}

//============================================================
//	�l�̐ݒ菈��
//============================================================
void CValue::SetNumber(const int nNum)
{
	// �����̒l����
	m_nNum = nNum;

	// �p�^�[���̐ݒ�
	CAnim2D::SetPattern(m_nNum);
}
