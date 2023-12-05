//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "ready.h"
//************************************************************
//	�O���錾
//************************************************************
class CMiddleResultManager;	// ���ԃ��U���g�}�l�[�W���[�N���X
class CEditStageManager;	// �G�f�B�b�g�X�e�[�W�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_READY,	// �ʏ���
		STATE_NORMAL,	// �ʏ���
		STATE_RESULT,	// ���U���g���
		STATE_END,		// �I�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetState(const EState state);	// ��Ԑݒ�
	EState GetState(void) const;		// ��Ԏ擾

	// �ÓI�����o�֐�
	static CGameManager *Create(void);	// ����
	static HRESULT Release(CGameManager *&prGameManager);	// �j��

	static CMiddleResultManager	*GetMiddleResult(void);	// ���ԃ��U���g�擾
	static CEditStageManager	*GetEditStage(void);	// �G�f�B�b�g�X�e�[�W�擾

private:
	// �ÓI�����o�ϐ�
	static CMiddleResultManager	*m_pMiddleResult;	// ���ԃ��U���g�̏��
	static CEditStageManager	*m_pEditStage;		// �G�f�B�b�g�X�e�[�W�̏��

	// �����o�ϐ�
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	CReady * m_pReady;
};

#endif	// _GAMEMANAGER_H_
