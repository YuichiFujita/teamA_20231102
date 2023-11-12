//============================================================
//
//	�I�u�W�F�N�g�O�Ճw�b�_�[ [objectOrbit.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_ORBIT_H_
#define _OBJECT_ORBIT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_OFFSET		(2)		// �I�t�Z�b�g�̐�
#define DEFAULT_PART	(10)	// �f�t�H���g�̕�����
#define DEFAULT_TEXPART	(1)		// �f�t�H���g�̃e�N�X�`��������

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�O�ՃN���X
class CObjectOrbit : public CObject
{
public:
	// �I�t�Z�b�g��
	enum EOffset
	{
		OFFSET_ARMDL = 0,	// �����r�I�t�Z�b�g
		OFFSET_ARMDR,		// �E���r�I�t�Z�b�g
		OFFSET_LEGDL,		// �����I�t�Z�b�g
		OFFSET_LEGDR,		// �E���I�t�Z�b�g
		OFFSET_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_VANISH,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObjectOrbit();
	explicit CObjectOrbit(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectOrbit();

	// �O�Ս\����
	struct SOrbit
	{
		D3DXVECTOR3	aOffset[MAX_OFFSET];		// ���[�̃I�t�Z�b�g
		D3DXCOLOR	aCol[MAX_OFFSET];			// ���[�̊�F
		D3DXMATRIX	aMtxWorldPoint[MAX_OFFSET];	// ���[�̃��[���h�}�g���b�N�X
		D3DXVECTOR3	*pPosPoint;		// �e���_���W
		D3DXCOLOR	*pColPoint;		// �e���_�J���[
		D3DXMATRIX	*pMtxParent;	// �e�̃}�g���b�N�X
		D3DXMATRIX	mtxVanish;		// �����J�n���̐e�̃}�g���b�N�X
		int nPart;		// ������
		int nTexPart;	// �e�N�X�`��������
		bool bAlpha;	// ��������
		bool bInit;		// ��������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	int GetState(void) const override;	// ��Ԏ擾

	// �ÓI�����o�֐�
	static CObjectOrbit *Create	// ����
	( // ����
		D3DXMATRIX *pMtxParent,	// �e�}�g���b�N�X
		const D3DXCOLOR& rCol,	// �F
		const EOffset offset,	// �I�t�Z�b�g
		const int nPart = DEFAULT_PART,			// ������
		const int nTexPart = DEFAULT_TEXPART,	// �e�N�X�`��������
		const bool bAlpha = true				// ��������
	);

	// �����o�֐�
	void DeleteMatrixParent(void);					// �e�}�g���b�N�X�폜
	void SetState(const EState state);				// ��Ԑݒ�
	void SetMatrixParent(D3DXMATRIX *pMtxParent);	// �e�̃}�g���b�N�X�ݒ�
	void SetOffset(const EOffset offset);			// �I�t�Z�b�g�ݒ�
	void SetTexPart(const int nTexPart);			// �e�N�X�`���������ݒ�
	void SetEnableAlpha(const bool bAlpha);			// �������󋵐ݒ�
	void SetEnableInit(const bool bInit);			// �������󋵐ݒ�
	HRESULT SetLength(const int nPart);				// �����ݒ�

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�

private:
	// �ÓI�����o�ϐ�
	static const D3DXVECTOR3 mc_aOffset[][MAX_OFFSET];	// �I�t�Z�b�g�̈ʒu������

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	SOrbit m_orbit;			// �O�Ղ̏��
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nNumVtx;			// �K�v���_��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_ORBIT_H_
