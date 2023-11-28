//============================================================
//
//	�p�[�e�B�N��3D�w�b�_�[ [particle3D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PARTICLE3D_H_
#define _PARTICLE3D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �p�[�e�B�N��3D�N���X
class CParticle3D : CObject
{
public:
	// �R���X�g���N�^
	CParticle3D();

	// �f�X�g���N�^
	~CParticle3D();

	// �p�[�e�B�N��3D��ޗ�
	enum EType
	{
		TYPE_NONE = 0,			// �Ȃ�
		TYPE_DAMAGE,			// �_���[�W
		TYPE_HEAL,				// ��
		TYPE_STOMP_PLANT,		// �A�����݂Ԃ�
		TYPE_SMALL_EXPLOSION,	// ������
		TYPE_BIG_EXPLOSION,		// �唚��
		TYPE_PLAYER_DAMAGE,		// �v���C���[�_���[�W
		TYPE_MUZZLE_FLASH,		// �}�Y���t���b�V��
		TYPE_TANK_FIRE,			// �^���N�t�@�C�A
		TYPE_BUBBLE_EXPLOSION,	// �o�u������
		TYPE_MAX				// ���̗񋓌^�̑���
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
	static CParticle3D *Create	// ����
	( // ����
		const EType type,					// ���
		const D3DXVECTOR3& rPos,			// �ʒu
		const D3DXCOLOR& rCol = XCOL_WHITE	// �F
	);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�֐�
	void Damage(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);		// �_���[�W
	void Heal(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);			// ��
	void StompPlant(const D3DXVECTOR3& rPos, const D3DXCOLOR& rCol);	// �A�����݂Ԃ�

	void SmallExplosion(const D3DXVECTOR3& rPos);	// ������
	void BigExplosion(const D3DXVECTOR3& rPos);		// �唚��
	void PlayerDamage(const D3DXVECTOR3& rPos);		// �v���C���[�_���[�W
	void MuzzleFlash(const D3DXVECTOR3& rPos);		// �}�Y���t���b�V��
	void TankFire(const D3DXVECTOR3& rPos);			// �^���N�t�@�C�A
	void BubbleExplosion(const D3DXVECTOR3& rPos);	// �o�u������

	// �ÓI�����o�ϐ�
	static const int mc_aLife[TYPE_MAX];	// �����萔

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXCOLOR m_col;	// �F
	EType m_type;		// ���
	int  m_nLife;		// ����
};

#endif	// _PARTICLE3D_H_
