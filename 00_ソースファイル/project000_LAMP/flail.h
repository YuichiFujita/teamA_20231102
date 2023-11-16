//============================================================
//
//	���f��UI�w�b�_�[ [flail.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FLAIL_H_
#define _FLAIL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���f��UI�N���X
class CFlail : public CObjectModel
{
public:
	// ���f����
	enum EModel
	{
		MODEL_FLAIL = 0,	// �S��
		MODEL_PREFABHUT,	// �v���n�u����
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CFlail();

	// �f�X�g���N�^
	~CFlail();
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetVec3PosOrg(const D3DXVECTOR3& rPosOrg);		// ��]���_�ݒ�
	D3DXVECTOR3 GetVec3PosOrg(void);					// ��]���_�擾

	void SetMove(const float& rMove);					// �ʒu�ݒ�
	float GetMove(void);								// �ʒu�擾

	void SetChainRot(const float& rChainRot);			// �p�x�ݒ�
	float GetChainRot(void);							// �p�x�擾

	void SetChainRotMove(const float& rChainRotMove);	// �ڕW�p�x�ݒ�
	float GetChainRotMove(void);						// �ڕW�p�x�擾

	void SetLengthChain(const float& rLengthChain);		// �����ݒ�
	float GetLengthChain(void);							// �����擾

	// �ÓI�����o�֐�
	static CFlail *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

private:
	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	D3DXVECTOR3 m_posOrg;			// ��]���_
	float		m_move;				// �ړ���
	float		m_fChainRot;		// �p�x
	float		m_fChainRotMove;	// �ڕW�p�x
	float		m_fLengthChain;		// ���̒���

};

#endif	// _FLAIL_H_
