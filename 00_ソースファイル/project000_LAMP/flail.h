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
#include "objectOrbit.h"

namespace flail
{
	const int	FLAIL_CHARGE = 120;		// �t���C�����ߏ��
	const int	FLAIL_THROW = 240;		// �t���C���������
	const int	FLAIL_DROP = -1;		// �t���C���������
	const int	FLAIL_DEF = 0;			// �t���C���擾���
	const int	FLAIL_NUM_MAX = 80;		// �t���C���̍ő吔
	const int	FLAIL_NUM_MIN = 40;		// �t���C���̍ŏ���
	const float	FLAIL_RADIUS = 20.0f;	// �t���C���̔��a
}

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// ���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// �t���C���N���X
class CFlail : public CObjectModel
{
public:
	// ���f����
	enum EModelFlail
	{
		FLAIL_NORMAL = 0,	// �S��
		FLAIL_PREFABHUT,	// �v���n�u����
		FLAIL_MAX			// ���̗񋓌^�̑���
	};

	enum EModelChain
	{
		CHAIN_NORMAL = 0,	// �S��
		CHAIN_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CFlail();

	// �f�X�g���N�^
	~CFlail();

	// ���\����
	struct SChain
	{
		CMultiModel *multiModel;
		D3DXVECTOR3 posOld;
		D3DXVECTOR3 rotOld;
		bool collision;
	};
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	void SetVec3PosOrg(const D3DXVECTOR3& rPosOrg);			// ��]���_�ݒ�
	D3DXVECTOR3 GetVec3PosOrg(void);						// ��]���_�擾

	void SetPlayerID(const int& rPlayerID);					// �v���C���[�ԍ��ݒ�
	int GetPlayerID(void);									// �v���C���[�ԍ��擾

	void SetNumChain(const int& rNumChain);					// �v���C���[�ԍ��ݒ�
	int GetNumChain(void);									// �v���C���[�ԍ��擾

	void SetMove(const D3DXVECTOR3& rMove);					// �ʒu�ݒ�
	D3DXVECTOR3 GetMove(void);								// �ʒu�擾

	void SetChainRot(const float& rChainRot);				// �p�x�ݒ�
	float GetChainRot(void);								// �p�x�擾

	void SetChainRotTarget(const float& rChainRotTarget);	// �ڕW�p�x�ݒ�
	float GetChainRotTarget(void);							// �ڕW�p�x�擾

	void SetChainRotMove(const float& rChainRotMove);		// �ڕW�p�x�ݒ�
	float GetChainRotMove(void);							// �ڕW�p�x�擾

	void SetLengthChain(const float& rLengthChain);			// �����ݒ�
	float GetLengthChain(void);								// �����擾

	void SetLengthTarget(const float& rLengthTarget);		// �ڕW�����ݒ�
	float GetLengthTarget(void);							// �ڕW�����擾

	void SetHit(const bool& Hit);						// �ڕW�����ݒ�
	bool GetHit(void);									// �ڕW�����擾

	void CatchFlail(void);
	void ShotFlail(const float rot);

	void InitOrbit(void);	// �O�Տ�����
	void VanishOrbit(void);	// �O�Տ���

	// �ÓI�����o�֐�
	static CFlail *Create	// ����
	( // ����
		CScene::EMode mode,						// ���[�h
		const CPlayer& rPlayer,					// �v���C���[
		const D3DXVECTOR3& rPos,				// �ʒu
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ����
		const D3DXVECTOR3& rScale = VEC3_ONE	// �傫��
	);

private:
	//�����o�֐�
	void Collision(D3DXVECTOR3& rPos);		// �����ݒ�
	bool CollisionChain(D3DXVECTOR3& rPos);		// �����ݒ�
	bool CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos);
	bool CollisionObstacle(D3DXVECTOR3& rPos);		// ��Q���Ƃ̓����蔻��
	void BindParent(const CPlayer& rPlayer);
	void UpdateChain(void);		// �X�V
	void UpdateFlailPos(void);	// �X�V
	void UpdateDropFlailPos(float& rRot);	// �X�V

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFileFlail[];	// ���f���萔
	static const char *mc_apModelFileChain[];	// ���f���萔

	// �����o�ϐ�
	SChain		m_chain[flail::FLAIL_NUM_MAX];	// ��
	CObjectOrbit * m_pOrbit;
	D3DXVECTOR3 m_posOrg;						// ��]���_
	D3DXVECTOR3 m_oldPos;						// �ߋ��̈ʒu
	D3DXVECTOR3	m_move;							// �ړ���
	int			m_nPlayerID;					// �v���C���[ID
	int			m_nNumChain;					// ���̐�
	int			m_nDamage;						// �_���[�W
	int			m_nHitCount;					// �_���[�W
	int			m_nfulChainF;					// ���̐�
	int			m_nfulChainP;					// ���̐�
	float		m_fChainRot;					// �p�x
	float		m_fChainRotOld;					// �p�x
	float		m_fChainRotTarget;				// �ڕW�p�x
	float		m_fChainRotMove;				// �p���x
	float		m_fLengthChain;					// ���̒���
	float		m_fLengthTarget;				// ���̒����ڕW
	bool		m_bHit;							// ���̏Փ˃`�F�b�N

};

#endif	// _FLAIL_H_
