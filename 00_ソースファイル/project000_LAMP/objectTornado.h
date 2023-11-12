//============================================================
//
//	�I�u�W�F�N�g�����w�b�_�[ [objectTornado.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_TORNADO_H_
#define _OBJECT_TORNADO_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�����N���X
class CObjectTornado : public CObject
{
public:
	// �R���X�g���N�^
	CObjectTornado();
	explicit CObjectTornado(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectTornado();

	// �����\����
	struct STornado
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXVECTOR3	direRot;	// ��������
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		D3DXMATRIX *pMtxParent;	// �e�̃}�g���b�N�X
		float fMoveRot;			// �����̕ύX��
		float fThickness;		// �|���S���̑���
		float fOuterPlus;		// �|���S���O���� y���W���Z��
		float fSetWidth;		// �������̉������
		float fSetAlpha;		// �������̓����x
		float fAddWidth;		// ������̉��Z��
		float fAddHeight;		// �c����̉��Z��
		float fSubAlpha;		// �����x�̌��Z��
		float fGrowWidth;		// ������̐�����
		float fGrowHeight;		// �c����̐�����
		float fGrowAlpha;		// �����x�̐�����
		D3DCULL cull;			// �J�����O��
		bool bLight;			// ���C�e�B���O��
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	// �ÓI�����o�֐�
	static CObjectTornado *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXVECTOR3& rDireRot,		// ��������
		const D3DXCOLOR& rCol,				// �F
		D3DXMATRIX *pMtxParent,				// �e�̃}�g���b�N�X
		const int   nNumAround,				// �Q�̎���
		const int   nPattern,				// �Q�̕�����
		const float fMoveRot,				// �����̕ύX��
		const float fThickness,				// �|���S���̑���
		const float fOuterPlus,				// �|���S���O���� y���W���Z��
		const float fSetWidth,				// �������̉������
		const float fSetAlpha,				// �������̓����x
		const float fAddWidth,				// ������̉��Z��
		const float fAddHeight,				// �c����̉��Z��
		const float fSubAlpha,				// �����x�̌��Z��
		const float fGrowWidth,				// ������̐�����
		const float fGrowHeight,			// �c����̐�����
		const float fGrowAlpha,				// �����x�̐�����
		const D3DCULL cull = D3DCULL_NONE,	// �J�����O��
		const bool bLight = false			// ���C�e�B���O��
	);

	// �����o�֐�
	void SetGrow	// �����ݒ�
	( // ����
		float fMoveRot,		// �����̕ύX��
		float fGrowWidth,	// ������̐�����
		float fGrowHeight,	// �c����̐�����
		float fGrowAlpha	// �����x�̐�����
	);
	HRESULT SetVortex	// �Q�ݒ�
	( // ����
		const int nNumAround,	// �Q�̎���
		const int nPattern		// �Q�̕�����
	);

	void DeleteMatrixParent(void);						// �e�}�g���b�N�X�폜
	void SetMatrixParent(D3DXMATRIX *pMtxParent);		// �e�}�g���b�N�X�ݒ�
	void SetDirectionRotation(const D3DXVECTOR3& rRot);	// ���������ݒ�
	D3DXVECTOR3 GetDirectionRotation(void) const;		// ���������擾

	void SetThickness(const float fThickness);	// �|���S���̑����ݒ�
	float GetThickness(void) const;				// �|���S���̑����擾
	void SetOuterPlus(const float fOuterPlus);	// �|���S���O���� y���W���Z�ʐݒ�
	float GetOuterPlus(void) const;				// �|���S���O���� y���W���Z�ʎ擾
	void SetWidth(const float fSetWidth);		// �������̉�����ʐݒ�
	float GetWidth(void) const;					// �������̉�����ʎ擾
	void SetAlpha(const float fSetAlpha);		// �������̓����x�ݒ�
	float GetAlpha(void) const;					// �������̓����x�擾
	void SetAddWidth(const float fAddWidth);	// ������̉��Z�ʐݒ�
	float GetAddWidth(void) const;				// ������̉��Z�ʎ擾
	void SetAddHeight(const float fAddHeight);	// �c����̉��Z�ʐݒ�
	float GetAddHeight(void) const;				// �c����̉��Z�ʎ擾
	void SetSubAlpha(const float fSubAlpha);	// �����x�̌��Z�ʐݒ�
	float GetSubAlpha(void) const;				// �����x�̌��Z�ʎ擾
	void SetCulling(const D3DCULL cull);		// �J�����O�ݒ�
	D3DCULL GetCulling(void) const;				// �J�����O�擾
	void SetLighting(const bool bLight);		// ���C�e�B���O�ݒ�
	bool GetLighting(void) const;				// ���C�e�B���O�擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	STornado m_tornado;	// �����̏��
	int m_nNumVtx;		// �K�v���_��
	int m_nNumAround;	// �Q�̎���
	int m_nPattern;		// �Q�̕�����
	int m_nTextureID;	// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_TORNADO_H_
