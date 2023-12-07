//============================================================
//
//	�p�[�e�B�N��2D�w�b�_�[ [particle2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PARTICLE2D_H_
#define _PARTICLE2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �p�[�e�B�N��2D�N���X
class CParticle2D : public CObject
{
public:
	// �R���X�g���N�^
	CParticle2D();

	// �f�X�g���N�^
	~CParticle2D() override;

	// �p�[�e�B�N��2D��ޗ�
	enum EType
	{
		TYPE_NONE = 0,	// �Ȃ�
		TYPE_DAMAGE,	// �_���[�W
		TYPE_ITEM,		// �A�C�e��
		TYPE_GETITEM,	// �A�C�e���擾
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �I�[�o�[���[�h�����I�[�o�[���C�h�֐�
	HRESULT Init	// ������
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXCOLOR& rCol		// �F
	);

	// �ÓI�����o�֐�
	static CParticle2D *Create	// ����
	( // ����
		const EType type,					// ���
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXCOLOR& rCol = XCOL_WHITE	// �F
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �_���[�W
	void Item(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// �A�C�e��
	void GetItem(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �A�C�e���擾

	// �ÓI�����o�ϐ�
	static const int mc_aLife[TYPE_MAX];	// �����萔

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXCOLOR m_col;	// �F
	EType m_type;		// ���
	int  m_nLife;		// ����
};

#endif	// _PARTICLE2D_H_
