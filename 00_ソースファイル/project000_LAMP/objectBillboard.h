//============================================================
//
//	�I�u�W�F�N�g�r���{�[�h�w�b�_�[ [objectBillboard.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�r���{�[�h�N���X
class CObjectBillboard : public CObject
{
public:
	// ���_��
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// ���S���_
		ORIGIN_DOWN,		// �����_
		ORIGIN_MAX,			// ���̗񋓌^�̑���
	};

	// ��]��
	enum ERotate
	{
		ROTATE_NORMAL = 0,	// �ʏ��]
		ROTATE_LATERAL,		// ����]
		ROTATE_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObjectBillboard();
	CObjectBillboard(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectBillboard();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetVec3Sizing(const D3DXVECTOR3& rSize) override;	// �傫���ݒ�
	D3DXVECTOR3 GetVec3Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾
	D3DXMATRIX *GetPtrMtxWorld(void) override;				// �}�g���b�N�X�|�C���^�擾
	D3DXMATRIX GetMtxWorld(void) const override;			// �}�g���b�N�X�擾

	// �ÓI�����o�֐�
	static CObjectBillboard *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize,	// �傫��
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE,		// �F
		const EOrigin origin = ORIGIN_CENTER,	// ���_
		const ERotate rotate = ROTATE_NORMAL,	// ��]
		const D3DCMPFUNC func = D3DCMP_ALWAYS,	// Z�e�X�g�ݒ�
		const bool bZEnable = false				// Z�o�b�t�@�̎g�p��
	);

	// �����o�֐�
	void SetOrigin(const EOrigin origin);	// ���_�ݒ�
	EOrigin GetOrigin(void) const;			// ���_�擾
	void SetRotate(const ERotate rotate);	// ��]�ݒ�
	ERotate GetRotate(void) const;			// ��]�擾
	void SetFunc(const D3DCMPFUNC func);	// Z�e�X�g�ݒ�
	D3DCMPFUNC GetFunc(void) const;			// Z�e�X�g�擾
	void SetZEnable(const bool bEnable);	// Z�o�b�t�@�̎g�p�󋵐ݒ�
	bool IsZEnable(void) const;				// Z�o�b�t�@�̎g�p�󋵎擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX	m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;		// �ʒu
	D3DXVECTOR3	m_rot;		// ����
	D3DXVECTOR3	m_size;		// �傫��
	D3DXCOLOR	m_col;		// �F
	EOrigin		m_origin;	// ���_
	ERotate		m_rotate;	// ��]
	D3DCMPFUNC	m_func;		// Z�e�X�g�ݒ�
	bool  m_bZEnable;		// Z�o�b�t�@�̎g�p��
	float m_fAngle;			// �Ίp���̊p�x
	float m_fLength;		// �Ίp���̒���
	int   m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECTBILLBOARD_H_
