//============================================================
//
//	�}���`�����w�b�_�[ [multiValue.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MULTIVALUE_H_
#define _MULTIVALUE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "value.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_DIGIT	(8)	// �����̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CValue;	// �����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �}���`�����N���X
class CMultiValue : public CObject
{
public:
	// �R���X�g���N�^
	CMultiValue();

	// �f�X�g���N�^
	~CMultiValue();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetEnableDraw(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetPriority(const int nPriority) override;			// �D�揇�ʐݒ�
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CMultiValue *Create	// ����
	( // ����
		const CValue::ETexture texture,	// �e�N�X�`��
		const int nNum,					// ����
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSize,		// �傫��
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE		// �F
	);

	// �����o�֐�
	void AddNum(const int nNum);		// ���l���Z
	void SetNum(const int nNum);		// ���l�ݒ�
	HRESULT SetDigit(const int nDigit);	// �����ݒ�
	void SetMin(const int nMin);		// �ŏ��l�ݒ�
	void SetMax(const int nMax);		// �ő�l�ݒ�
	int GetNum(void) const;				// ���l�擾
	int GetDigit(void) const;			// �����擾
	int GetMin(void) const;				// �ŏ��l�擾
	int GetMax(void) const;				// �ő�l�擾

	void SetSpace(const D3DXVECTOR3& rSpace);			// �s�Ԑݒ�
	D3DXVECTOR3 GetSpace(void) const;					// �s�Ԏ擾
	void SetTexture(const CValue::ETexture texture);	// �e�N�X�`���ݒ�


private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void SetNumPosition(void);	// �����̈ʒu�ݒ�
	void SetTexNum(void);		// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CValue *m_apValue[MAX_DIGIT];	// ���l�̏��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_space;	// ���l�̍s��
	int m_nNum;		// ����
	int m_nDigit;	// ����
	int m_nMin;		// �ŏ��l
	int m_nMax;		// �ő�l
};

#endif	// _MULTIVALUE_H_
