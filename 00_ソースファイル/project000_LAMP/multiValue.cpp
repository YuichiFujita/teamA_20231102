//============================================================
//
//	�}���`�������� [multiValue.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiValue.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�q�N���X [CMultiValue] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMultiValue::CMultiValue() : CObject(CObject::LABEL_NUMBER, DEFAULT_PRIO)
{
	// �����o�ϐ����N���A
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_pos	= VEC3_ZERO;	// �ʒu
	m_space	= VEC3_ZERO;	// ���l�̍s��
	m_nNum		= 0;		// ����
	m_nDigit	= 0;		// ����
	m_nMin		= 0;		// �ŏ��l
	m_nMax		= 0;		// �ő�l
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMultiValue::~CMultiValue()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMultiValue::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// ���l�̏��
	m_pos	= VEC3_ZERO;	// �ʒu
	m_space	= VEC3_ZERO;	// ���l�̍s��
	m_nNum		= 0;		// ����
	m_nDigit	= 0;		// ����
	m_nMin		= 0;		// �ŏ��l
	m_nMax		= 0;		// �ő�l

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMultiValue::Uninit(void)
{
	// �����I�u�W�F�N�g��j��
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // �����̍ő吔���J��Ԃ�

		if (m_apValue[nCntValue] != NULL)
		{ // �g�p����Ă���ꍇ

			// �����̏I��
			m_apValue[nCntValue]->Uninit();
		}
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMultiValue::Update(void)
{
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // �����̍ő吔���J��Ԃ�

		if (m_apValue[nCntValue] != NULL)
		{ // �g�p����Ă���ꍇ

			// �����̍X�V
			m_apValue[nCntValue]->Update();
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CMultiValue::Draw(void)
{

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CMultiValue::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̕`��󋵂�ݒ�
		m_apValue[nCntValue]->SetEnableDraw(bDraw);	// ����
	}
}

//============================================================
//	�D�揇�ʂ̐ݒ菈��
//============================================================
void CMultiValue::SetPriority(const int nPriority)
{
	// �����̗D�揇�ʂ�ݒ�
	CObject::SetPriority(nPriority);	// ���g

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̗D�揇�ʂ�ݒ�
		m_apValue[nCntValue]->SetPriority(nPriority);	// ����
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// �����ʒu�̐ݒ�
	SetNumPosition();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̌�����S�Ă̐����ɐݒ�
		m_apValue[nCntValue]->SetVec3Rotation(rRot);
	}
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Rotation(void) const
{
	// ��O����
	assert(m_apValue[0] != NULL);	// ��g�p�`�F�b�N

	// �擪�����̌�����Ԃ�
	return m_apValue[0]->GetVec3Rotation();
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CMultiValue::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̑傫����S�Ă̐����ɐݒ�
		m_apValue[nCntValue]->SetVec3Sizing(rSize);
	}
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetVec3Sizing(void) const
{
	// ��O����
	assert(m_apValue[0] != NULL);	// ��g�p�`�F�b�N

	// �擪�����̑傫����Ԃ�
	return m_apValue[0]->GetVec3Sizing();
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CMultiValue::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̐F��S�Ă̐����ɐݒ�
		m_apValue[nCntValue]->SetColor(rCol);
	}
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CMultiValue::GetColor(void) const
{
	// ��O����
	assert(m_apValue[0] != NULL);	// ��g�p�`�F�b�N

	// �擪�����̐F��Ԃ�
	return m_apValue[0]->GetColor();
}

//============================================================
//	��������
//============================================================
CMultiValue *CMultiValue::Create
(
	const CValue::ETexture texture,	// �e�N�X�`��
	const int nNum,					// ����
	const int nDigit,				// ����
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSize,		// �傫��
	const D3DXVECTOR3& rSpace,		// �s��
	const D3DXVECTOR3& rRot,		// ����
	const D3DXCOLOR& rCol			// �F
)
{
	// �|�C���^��錾
	CMultiValue *pMultiValue = NULL;	// �}���`���������p

	if (pMultiValue == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMultiValue = new CMultiValue;	// �}���`����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMultiValue != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �}���`�����̏�����
		if (FAILED(pMultiValue->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMultiValue;
			pMultiValue = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ������ݒ�
		pMultiValue->SetDigit(nDigit);

		// ������ݒ�
		pMultiValue->SetNum(nNum);

		// �e�N�X�`����ݒ�
		pMultiValue->SetTexture(texture);

		// ������ݒ�
		pMultiValue->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pMultiValue->SetVec3Sizing(rSize);

		// �F��ݒ�
		pMultiValue->SetColor(rCol);

		// �s�Ԃ�ݒ�
		pMultiValue->SetSpace(rSpace);

		// �ʒu��ݒ�
		pMultiValue->SetVec3Position(rPos);

		// �m�ۂ����A�h���X��Ԃ�
		return pMultiValue;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	���l�̉��Z����
//============================================================
void CMultiValue::AddNum(const int nNum)
{
	// ���������������Z
	m_nNum += nNum;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	���l�̐ݒ菈��
//============================================================
void CMultiValue::SetNum(const int nNum)
{
	// �����������̒l�ɐݒ�
	m_nNum = nNum;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);

	// �����̃e�N�X�`�����W�̐ݒ�
	SetTexNum();
}

//============================================================
//	�����̐ݒ菈��
//============================================================
HRESULT CMultiValue::SetDigit(const int nDigit)
{
	// �ϐ���錾
	D3DXVECTOR3 rot = VEC3_ZERO;	// ����
	D3DXVECTOR3 size = VEC3_ONE;	// �傫��
	D3DXCOLOR col = XCOL_WHITE;		// �F
	int nLimit = 1;					// �ő�l�̌v�Z�p

	// ��O����
	assert(nDigit > 0);	// �����ᔽ

	// �����̌�����ݒ�
	m_nDigit = nDigit;

	if (m_apValue[0] != NULL)
	{ // �擪���g�p����Ă���ꍇ

		// ���݂̐ݒ���擾
		rot = GetVec3Rotation();	// ����
		size = GetVec3Sizing();		// �傫��
		col = GetColor();			// �F
	}

	//--------------------------------------------------------
	//	�����I�u�W�F�N�g�̔j���E����
	//--------------------------------------------------------
	// �����I�u�W�F�N�g�̔j��
	for (int nCntValue = 0; nCntValue < MAX_DIGIT; nCntValue++)
	{ // �����̍ő吔���J��Ԃ�

		if (m_apValue[nCntValue] != NULL)
		{ // �g�p����Ă���ꍇ

			// �����̏I��
			m_apValue[nCntValue]->Uninit();
		}
	}

	// �����I�u�W�F�N�g�̐���
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̐���
		m_apValue[nCntValue] = CValue::Create(CValue::TEXTURE_NORMAL);
		if (m_apValue[nCntValue] == NULL)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	//--------------------------------------------------------
	//	��{���̍Đݒ�
	//--------------------------------------------------------
	// ������ݒ�
	SetVec3Rotation(rot);

	// �傫����ݒ�
	SetVec3Sizing(size);

	// �F��ݒ�
	SetColor(col);

	// �����ʒu�̐ݒ�
	SetNumPosition();

	//--------------------------------------------------------
	//	�����l�̐ݒ�
	//--------------------------------------------------------
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����𑝂₷
		nLimit *= 10;
	}

	// �ŏ��l�̐ݒ�
	SetMin(0);

	// �ő�l�̐ݒ�
	SetMax(nLimit - 1);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�ŏ��l�̐ݒ菈��
//============================================================
void CMultiValue::SetMin(const int nMin)
{
	// ��O����
	assert(nMin >= 0 && nMin <= m_nMax);

	// �����̍ŏ��l��ݒ�
	m_nMin = nMin;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	�ő�l�̐ݒ菈��
//============================================================
void CMultiValue::SetMax(const int nMax)
{
	// �ϐ���錾
	int nLimit = 1;	// ��O�����̍ő�l�v�Z�p

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����𑝂₷
		nLimit *= 10;
	}

	// ��O����
	assert(nMax <= nLimit - 1 && nMax >= m_nMin);

	// �����̍ő�l��ݒ�
	m_nMax = nMax;

	// �����̕␳
	useful::LimitNum(m_nNum, m_nMin, m_nMax);
}

//============================================================
//	���l�擾����
//============================================================
int CMultiValue::GetNum(void) const
{
	// �����̒l��Ԃ�
	return m_nNum;
}

//============================================================
//	�����擾����
//============================================================
int CMultiValue::GetDigit(void) const
{
	// ������Ԃ�
	return m_nDigit;
}

//============================================================
//	�ŏ��l�擾����
//============================================================
int CMultiValue::GetMin(void) const
{
	// �ŏ��l��Ԃ�
	return m_nMin;
}

//============================================================
//	�ő�l�擾����
//============================================================
int CMultiValue::GetMax(void) const
{
	// �ő�l��Ԃ�
	return m_nMax;
}

//============================================================
//	�s�Ԃ̐ݒ菈��
//============================================================
void CMultiValue::SetSpace(const D3DXVECTOR3& rSpace)
{
	// �����̍s�Ԃ�ݒ�
	m_space = rSpace;
}

//============================================================
//	�s�Ԏ擾����
//============================================================
D3DXVECTOR3 CMultiValue::GetSpace(void) const
{
	// �s�Ԃ�Ԃ�
	return m_space;
}

//============================================================
//	�e�N�X�`���̐ݒ菈��
//============================================================
void CMultiValue::SetTexture(const CValue::ETexture texture)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// �����̃e�N�X�`����S�Ă̐����ɐݒ�
		m_apValue[nCntValue]->SetTexture(texture);
	}
}

//============================================================
//	�j������
//============================================================
void CMultiValue::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	�����ʒu�̐ݒ菈��
//============================================================
void CMultiValue::SetNumPosition(void)
{
	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		// ���_�̈ʒu���琔���̈ʒu��ݒ�
		m_apValue[nCntValue]->SetVec3Position(m_pos + (m_space * (float)nCntValue));
	}
}

//============================================================
//	�����̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CMultiValue::SetTexNum(void)
{
	// �ϐ���錾
	int aNumDivide[MAX_DIGIT];	// �������Ƃ̕���p

	// �}���`�������������Ƃɕ���
	useful::DivideDigitNum
	( // ����
		&aNumDivide[0],	// �������ʂ̊i�[�z��
		m_nNum,			// �������鐔�l
		m_nDigit		// �������鐔���̌���
	);

	for (int nCntValue = 0; nCntValue < m_nDigit; nCntValue++)
	{ // �������J��Ԃ�

		if (m_apValue[nCntValue] != NULL)
		{ // �g�p����Ă���ꍇ

			// �����̐ݒ�
			m_apValue[nCntValue]->SetNumber(aNumDivide[nCntValue]);
		}
	}
}
