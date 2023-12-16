//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectChara.h"
#include "scene.h"
#include "object3D.h"
//************************************************************
//	�O���錾
//************************************************************
class CStatusManager;	// �X�e�[�^�X�}�l�[�W���[�N���X
class CFlail;			// �t���C���N���X
class CPlayerAI;		// �t���C���N���X
class CObjectBillboard;
//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// ���f����
	enum EModel
	{
		MODEL_WAIST = 0,	// ��
		MODEL_BODY,			// ��
		MODEL_HEAD,			// ��
		MODEL_SHOULDER_R,	// �E��
		MODEL_ARM_R,		// �E�r
		MODEL_HAND_R,		// �E��
		MODEL_SHOULDER_L,	// ����
		MODEL_ARM_L,		// ���r
		MODEL_HAND_L,		// ����
		MODEL_THIGH_R,		// �E����
		MODEL_LEG_R,		// �E��
		MODEL_THIGH_L,		// ������
		MODEL_LEG_L,		// ����
		MODEL_STICK,		// �_
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_MOVE,		// �ړ����[�V����
		MOTION_ATTACK,		// �U�����[�V����
		MOTION_CHARGE,		// �`���[�W���[�V����
		MOTION_DASH,		// �_�b�V�����[�V����
		MOTION_PULL,		// �������胂�[�V����
		MOTION_KNOCK,		// ������у��[�V����
		MOTION_LAND,		// ���n���[�V����
		MOTION_DROWN,		// �M�ꃂ�[�V����
		MOTION_DEATH,		// ���S���[�V����
		MOTION_EMOTE_PROUD,	// ���𒣂�G���[�g���[�V����
		MOTION_EMOTE_SLEEP,	// �Q��G���[�g���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �v���C���[��
	enum EPlayer
	{
		PLAYER_RED = 0,	// �v���C���[1�F��
		PLAYER_BLUE,	// �v���C���[2�F��
		PLAYER_YELLOW,	// �v���C���[3�F��
		PLAYER_GREEN,	// �v���C���[4�F��
		PLAYER_MAX		// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_SPAWN,	// �X�|�[�����
		STATE_NORMAL,	// �ʏ���
		STATE_KNOCK,	// �m�b�N���
		STATE_INVULN,	// ���G���
		STATE_DROWN,	// �M����
		STATE_DEATH,	// ���S���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// ����
	enum EAxis
	{
		AXIS_X = 0,	// X��
		AXIS_Y,		// Y��
		AXIS_Z,		// Z��
		AXIS_MAX	// ���̗񋓌^�̑���
	};
	//�A�C�e������
	enum EItem
	{
		ITEM_EMPTY = 0,
		ITEM_HEAL ,// ��
		ITEM_BOOST_ATTACK,
		ITEM_BOOST_KNOCKBACK,
		ITEM_SUPERARMOR,
		ITEM_BIGFLAIL,
		ITEM_LONGFLAIL,
		ITEM_GHOSTFLAIL,
		ITEM_MULTIFLAIL,
		ITEM_BURNINGFLAIL,
		ITEM_MAX
	};

	struct SItemEffect
	{
		EItem type;	//���
		int nLife;	//���ʎ���
	};
	// �R���X�g���N�^
	explicit CPlayer(const int nPad);

	// �f�X�g���N�^
	~CPlayer();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void Hit(void) override;		// �A�C�e���q�b�g

	void SetState(const int nState) override;	// ��Ԑݒ�
	int GetState(void) const override;			// ��Ԏ擾
	float GetRadius(void) const override;		// ���a�擾
	float GetHeight(void) const override;		// �c���擾

	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�
	D3DXMATRIX GetMtxWorld(void) const override;		// �}�g���b�N�X�擾

	// �ÓI�����o�֐�
	static CPlayer *Create(CScene::EMode mode, const int nPad, const bool bAI = false);	// ����

	// �����o�֐�
	void HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock, CPlayer *pAttack);	// �m�b�N�o�b�N�q�b�g
	void HitKillY(const int nDmg);	// �L��Y���W�q�b�g
	void SetSpawn(void);	// �o���ݒ�
	void SetInvuln(void);	// ���G�ݒ�

	void SetEnableDrawUI(const bool bDraw);		// UI�`��󋵐ݒ�
	int GetPadID(void) const;					// �p�b�h�C���f�b�N�X�擾
	CPlayer *GetLastAttackPlayer(void) const;	// �Ō�ɍU�������v���C���[�擾
	int GetCounterFlail(void) const;			// �t���C���J�E���^�[�擾
	void SetCounterFlail(const int nCounterFlail);		// �t���C���J�E���^�[�擾

	CFlail *GetFlail(void) const;		// �t���C���J�E���^�[�擾

	void SetItemPermanent(EItem Item);
	SItemEffect GetTemporaryItem() { return m_SItemTemporary; }
protected:
	// �����o�֐�
	void UpdateMotion(int nMotion);			// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	void SetMainMaterial(void);				// ���g�̃��C���J���[�}�e���A���ݒ�
	D3DXMATERIAL GetMainMaterial(void);		// ���g�̃��C���J���[�}�e���A���擾

private:
	// �����o�֐�
	void LoadSetup(void);		// �Z�b�g�A�b�v
	EMotion UpdateSpawn(void);	// �X�|�[����Ԏ��̍X�V
	EMotion UpdateNormal(void);	// �ʏ��Ԏ��̍X�V
	EMotion UpdateKnock(void);	// �m�b�N��Ԏ��̍X�V
	EMotion UpdateInvuln(void);	// ���G��Ԏ��̍X�V
	EMotion UpdateDrown(void);	// �M���Ԏ��̍X�V
	void UpdateDeath(void);		// ���S��Ԏ��̍X�V

	void PlayEmote(EMotion& rAnim);		// �G���[�g����

	void UpdateOldPosition(void);			// �ߋ��ʒu�̍X�V
	EMotion UpdateMove(D3DXVECTOR3& rPos);	// �ړ��ʁE�ڕW�����̍X�V
	void UpdateDash(void);					// �_�b�V���̍X�V
	void UpdateGravity(void);				// �d�͂̍X�V
	bool UpdateLanding(D3DXVECTOR3& rPos);	// ���n�󋵂̍X�V
	void UpdateRotation(D3DXVECTOR3& rRot);	// �����̍X�V
	bool UpdateFadeOut(const float fAdd);	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	bool UpdateFadeIn(const float fSub);	// �t�F�[�h�C����Ԏ��̍X�V

	bool ResponseSingleGround(const EAxis axis, D3DXVECTOR3& rPos);	// �n�ՂƂ̈ꎲ���Ƃ̓����蔻��
	bool ResponseSingleBlock(const EAxis axis, D3DXVECTOR3& rPos);	// �u���b�N�Ƃ̈ꎲ���Ƃ̓����蔻��
	bool CollisionGroundBlock(D3DXVECTOR3& rPos);	// �n�ՁE�u���b�N�Ƃ̓����蔻��
	bool CollisionObstacle(D3DXVECTOR3& rPos);		// ��Q���Ƃ̓����蔻��
	bool CollisionPlayer(D3DXVECTOR3& rPos);		// ��Q���Ƃ̓����蔻��

	// �ÓI�����o�ϐ�
	static const char *mc_apModelFile[];	// ���f���萔

	// �����o�ϐ�
	SItemEffect m_SItemPermanent[2];//�i���I�ȃA�C�e���̌���
	SItemEffect m_SItemTemporary;	//�ꎞ�I�ȃA�C�e���̌���
	CStatusManager *m_pStatus;		// �X�e�[�^�X�̏��
	CFlail		*m_pFlail;			// �t���C���̏��
	CPlayer		*m_pFinalAttack;	// �U���҂̏��
	CPlayerAI	*m_pAI;				// AI���
	D3DXVECTOR3	m_oldPos;			// �ߋ��ʒu
	D3DXVECTOR3	m_move;				// �ړ���
	D3DXVECTOR3	m_destRot;			// �ڕW����
	D3DXVECTOR3	m_dashRot;			// �_�b�V������
	EState		m_state;			// ���
	CObject3D * m_pGuide;			// ��������
	CObjectBillboard * m_pPlayerGuide;
	CObjectBillboard * m_pClown;
	int			m_motionOld;		// �ߋ����[�V����
	int			m_nCounterState;	// ��ԊǗ��J�E���^�[
	int			m_nCounterFlail;	// �t���C���Ǘ��J�E���^�[
	int			m_nCounterAttack;	// �ŏI�U���J�E���^�[
	float		m_fPlusMove;		// �v���X�ړ���
	float		m_fSinAlpha;		// ��������
	bool		m_bDash;			// �_�b�V����
	bool		m_bJump;			// �W�����v��
	bool		m_bAI;				// AI��
	bool		m_bHook;			// AI��
	const int	m_nPadID;			// �p�b�h�C���f�b�N�X
};

#endif	// _PLAYER_H_
