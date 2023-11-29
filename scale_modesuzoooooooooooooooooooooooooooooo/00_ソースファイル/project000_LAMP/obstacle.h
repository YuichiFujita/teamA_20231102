//============================================================
//
//	��Q���w�b�_�[ [obstacle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectModel.h"

//************************************************************
//	�N���X��`
//************************************************************
// ��Q���N���X
class CObstacle : public CObjectModel
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_CONIFER = 0,	// �j�t�����f��
		TYPE_HARDWOOD,		// �L�t�����f��
		TYPE_ELDEN_JAR,		// �⃂�f��(�G���f�������O��)
		TYPE_ZELDA_JAR,		// �⃂�f��(�[���_��)
		TYPE_ROCK1,			// �⃂�f��
		TYPE_ROCK2,			// �䂪�񂾊⃂�f��	
		TYPE_CRACKED_ROCK,	// �Ђъ��ꂽ�⃂�f��	
		TYPE_STONEPILLER,	// �΃����K�̒�
		TYPE_WELL,			// ��˃��f��
		TYPE_WALL1,			// �ۉ����̕ǃ��f��
		TYPE_WALL1_FLIMSY,	// ���낢�ۉ����̕ǃ��f��
		TYPE_WALL2,			// �O�p�����̕ǃ��f��
		TYPE_WALL2_FLIMSY,	// ���낢�O�p�����̕ǃ��f��
		TYPE_CASTLE,		// �郂�f��
		TYPE_MONTE_CASTLE,	// �J�X�e���E�f���E�����e���̏郂�f��
		TYPE_CASTLE_FENCE1,	// ��̍򃂃f��1
		TYPE_CASTLE_FENCE2,	// ��̍򃂃f��2
		TYPE_FOUNTAIN,		// �������f��
		TYPE_TOWER,			// �����f��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �j���
	enum EBreak
	{
		BREAK_FALSE = 0,	// �j��OFF
		BREAK_TRUE,			// �j��ON
		BREAK_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObstacle();

	// �f�X�g���N�^
	~CObstacle();

	// �X�e�[�^�X�\����
	struct SStatusInfo
	{
		EBreak state;	// �j���
		int nLife;		// �̗�
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetType(const int nType) override;	// ��ސݒ�
	int GetType(void) const override;		// ��ގ擾

	// �ÓI�����o�֐�
	static CObstacle *Create	// ����
	( // ����
		const EType type,			// ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static void LoadSetup(void);	// �Z�b�g�A�b�v

private:
	// �ÓI�����o�ϐ�
	static SStatusInfo m_aStatusInfo[TYPE_MAX];	// �X�e�[�^�X���
	static const char *mc_apModelFile[];		// ���f���萔

	// �����o�ϐ�
	SStatusInfo m_status;	// �X�e�[�^�X
	EType m_type;	// ���
	int m_nLife;	// �̗�
};

#endif	// _OBSTACLE_H_
