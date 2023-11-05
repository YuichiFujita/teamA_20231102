//============================================================
//
//	����UI�w�b�_�[ [valueUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _VALUE_UI_H_
#define _VALUE_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "value.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CMultiValue;	// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// ����UI�N���X
class CValueUI : public CObject
{
public:
	// �R���X�g���N�^
	CValueUI();

	// �f�X�g���N�^
	~CValueUI();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetEnableDraw(const bool bDraw) override;	// �`��󋵐ݒ�
	void SetPriority(const int nPriority) override;	// �D�揇�ʐݒ�
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾

	// �ÓI�����o�֐�
	static CValueUI *Create	// ����
	( // ����
		const char *pPassTex,			// �^�C�g���e�N�X�`���p�X
		const CValue::ETexture texture,	// �����e�N�X�`��
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rSpaceValue,	// �����s��
		const D3DXVECTOR3& rSizeTitle,	// �^�C�g���傫��
		const D3DXVECTOR3& rSizeValue,	// �����傫��
		const D3DXVECTOR3& rRotTitle = VEC3_ZERO,	// �^�C�g������
		const D3DXVECTOR3& rRotValue = VEC3_ZERO,	// ��������
		const D3DXCOLOR& rColTitle = XCOL_WHITE,	// �^�C�g���F
		const D3DXCOLOR& rColValue = XCOL_WHITE		// �����F
	);

	// �����o�֐�
	void SetSpace(const D3DXVECTOR3& rSpace);		// �s�Ԑݒ�
	D3DXVECTOR3 GetSpace(void) const;				// �s�Ԏ擾
	void SetRotationTitle(const D3DXVECTOR3& rRot);	// �^�C�g�������ݒ�
	D3DXVECTOR3 GetRotationTitle(void) const;		// �^�C�g�������擾
	void SetScalingTitle(const D3DXVECTOR3& rSize);	// �^�C�g���傫���ݒ�
	D3DXVECTOR3 GetScalingTitle(void) const;		// �^�C�g���傫���擾
	void SetColorTitle(const D3DXCOLOR& rCol);		// �^�C�g���F�ݒ�
	D3DXCOLOR GetColorTitle(void) const;			// �^�C�g���F�擾
	void SetTextureTitle(const char *pPassTex);		// �^�C�g���e�N�X�`���ݒ�
	CMultiValue *GetMultiValue(void) const;			// �������擾

private:
	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CObject2D *m_pTitle;	// �^�C�g�����
	CMultiValue *m_pValue;	// �������
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_space;	// �s��
};

#endif	// _VALUE_UI_H_
