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
		TYPE_WOOD = 0,	// �؃��f��
		TYPE_MAX		// ���̗񋓌^�̑���
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
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR3& rScale	// �g�嗦
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
