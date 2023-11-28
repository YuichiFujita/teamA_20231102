//============================================================
//
//	�t���C���w�b�_�[ [flail.h]
//	Author�F������
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
#include "multiModel.h"
#include "player.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// ���f���N���X

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

	// ���\����
	struct SChain
	{
		CMultiModel *multiModel;
		D3DXVECTOR3 rotOld;
	};
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetVec3PosOrg(const D3DXVECTOR3& rPosOrg);		// ��]���_�ݒ�
	D3DXVECTOR3 GetVec3PosOrg(void);					// ��]���_�擾

	void SetPlayerID(const int& rPlayerID);				// �v���C���[�ԍ��ݒ�
	int GetPlayerID(void);								// �v���C���[�ԍ��擾

	void SetMove(const D3DXVECTOR3& rMove);				// �ʒu�ݒ�
	D3DXVECTOR3 GetMove(void);							// �ʒu�擾

	void SetChainRot(const float& rChainRot);			// �p�x�ݒ�
	float GetChainRot(void);							// �p�x�擾

	void SetChainRotMove(const float& rChainRotMove);	// �ڕW�p�x�ݒ�
	float GetChainRotMove(void);						// �ڕW�p�x�擾

	void SetLengthChain(const float& rLengthChain);		// �����ݒ�
	float GetLengthChain(void);							// �����擾

	// �ÓI�����o�֐�
	static CFlail *Create	// ����
	( // ����
		const CPlayer& rPlayer,					// �v���C���[
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

private:
	//�����o�֐�
	void Collision(D3DXVECTOR3& rPos);		// �����ݒ�
	void CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	void CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	void BindParent(const CPlayer& rPlayer);
	void UpdateChain(void);		// �X�V

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	SChain		m_chain[10];		// ��
	D3DXVECTOR3 m_posOrg;			// ��]���_
	D3DXVECTOR3 m_oldPos;			// �ߋ��̈ʒu
	D3DXVECTOR3	m_move;				// �ړ���
	int			m_nPlayerID;		// �v���C���[ID
	float		m_fChainRot;		// �p�x
	float		m_fChainRotMove;	// �ڕW�p�x
	float		m_fLengthChain;		// ���̒���

};

#endif	// _FLAIL_H_
