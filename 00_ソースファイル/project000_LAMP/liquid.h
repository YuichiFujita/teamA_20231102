//============================================================
//
//	�t�̃w�b�_�[ [liquid.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIQUID_H_
#define _LIQUID_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CScrollMeshField;	// �X�N���[�����b�V���t�B�[���h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �t�̃N���X
class CLiquid : public CObject
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SEA = 0,	// �t��
		TYPE_LAVA,
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �㉺��
	enum ELiquid
	{
		LIQUID_LOW = 0,	// ��
		LIQUID_HIGH,	// ��
		LIQUID_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLiquid();

	// �f�X�g���N�^
	~CLiquid();

	// �e�N�X�`���ړ��ʍ\����
	struct STexMove
	{
	public:
		// �R���X�g���N�^
		STexMove() {};
		STexMove(const D3DXVECTOR2 TexMoveLow, const D3DXVECTOR2 TexMoveHigh)
		{ texMoveLow = TexMoveLow; texMoveHigh = TexMoveHigh; };

		// �f�X�g���N�^
		~STexMove() {};

		// �����o�ϐ�
		union
		{
			struct
			{
				D3DXVECTOR2 texMoveLow;		// ���t�̂̃e�N�X�`���ړ���
				D3DXVECTOR2 texMoveHigh;	// ��t�̂̃e�N�X�`���ړ���
			};
			D3DXVECTOR2 aTexMove[2];	// �e�N�X�`���ړ���
		};
	};

	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// �傫���ݒ�
	D3DXVECTOR2 GetVec2Sizing(void) const override;			// �傫���擾
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;				// �F�擾

	void SetType(const int nType) override;	// ��ސݒ�
	int GetType(void) const override;		// ��ގ擾

	// �ÓI�����o�֐�
	static CLiquid *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const STexMove& rTexMove,	// �e�N�X�`���ړ���
		const float fMaxUp,			// �g�̍ō��㏸��
		const float fAddSinRot,		// �g�ł��������Z��
		const float fAddVtxRot		// �הg�̌������Z��
	);

	// �����o�֐�
	HRESULT SetPattern(const POSGRID2& rPart);			// �������ݒ�
	POSGRID2 GetPattern(void) const;					// �������擾
	void SetTexMove(const STexMove texMove);			// �e�N�X�`���ړ��ʐݒ�
	STexMove GetTexMove(const ELiquid liquid) const;	// �e�N�X�`���ړ��ʎ擾

	void SetMaxUp(const float fMaxUp);			// �g�̍ō��㏸�ʐݒ�
	float GetMaxUp(void) const;					// �g�̍ō��㏸�ʎ擾
	void SetAddSinRot(const float fAddSinRot);	// �g�ł��������Z�ʐݒ�
	float GetAddSinRot(void) const;				// �g�ł��������Z�ʎ擾
	void SetAddVtxRot(const float fAddVtxRot);	// �הg�̌������Z�ʐݒ�
	float GetAddVtxRot(void) const;				// �הg�̌������Z�ʎ擾

	CScrollMeshField *GetScrollMeshField(int num) const;	// �g�̃��b�V���t�B�[���h�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[][LIQUID_MAX];	// �e�N�X�`���萔

	// �����o�ϐ�
	CScrollMeshField *m_apLiquid[LIQUID_MAX];	// �t�̂̏��
	EType m_type;		// ���
	float m_fMaxUp;		// �g�̍ō��㏸��
	float m_fSinRot;	// �g�ł�����
	float m_fAddSinRot;	// �g�ł��������Z��
	float m_fAddVtxRot;	// �הg�̌������Z��
};

#endif	// _LIQUID_H_
