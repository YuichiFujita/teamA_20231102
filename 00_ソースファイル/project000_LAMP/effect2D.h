//============================================================
//
//	�G�t�F�N�g2D�w�b�_�[ [effect2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�t�F�N�g2D�N���X
class CEffect2D : public CObject2D
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CEffect2D();
	CEffect2D(const EType type, const CObject::ELabel label);

	// �f�X�g���N�^
	~CEffect2D();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CEffect2D *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const float fRadius,		// ���a
		const EType type = TYPE_NORMAL,			// �e�N�X�`��
		const int nLife = 1,					// ����
		const D3DXVECTOR3& rMove = VEC3_ZERO,	// �ړ���
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXCOLOR& rCol = XCOL_WHITE,		// �F
		const float fSubSize = 0.0f,			// ���a�̌��Z��
		const bool bAdd = true,					// ���Z������
		const CObject::ELabel label = LABEL_EFFECT	// �I�u�W�F�N�g���x��
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	D3DXVECTOR3 m_move;		// �ړ���
	const EType m_type;		// ��ޒ萔
	int		m_nLife;		// ����
	float	m_fSubSize;		// �傫���̌��Z��
	float	m_fSubAlpha;	// �����x�̌��Z��
	bool	m_bAdd;			// ���Z������
};

#endif	// _EFFECT2D_H_
