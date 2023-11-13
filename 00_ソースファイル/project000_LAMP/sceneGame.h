//============================================================
//
//	�Q�[����ʃw�b�_�[ [sceneGame.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"
#include "camera.h"

//************************************************************
//	�O���錾
//************************************************************
class CGameManager;		// �Q�[���}�l�[�W���[�N���X
class CTimerManager;	// �^�C�}�[�}�l�[�W���[�N���X
class CPause;			// �|�[�Y�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneGame(const EMode mode);

	// �f�X�g���N�^
	~CSceneGame();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CGameManager		*GetGameManager(void);		// �Q�[���}�l�[�W���[�擾
	static CTimerManager	*GetTimerManager(void);		// �^�C�}�[�}�l�[�W���[�擾
	static CPause			*GetPause(void);			// �|�[�Y�擾

	static void SetEnableDrawUI(const bool bDraw);		// UI�̕`��󋵐ݒ�
	static bool IsDrawUI(void);							// UI�̕`��󋵎擾
	static void SetEnableDrawPause(const bool bDraw);	// �|�[�Y�̕`��󋵐ݒ�
	static bool IsDrawPause(void);						// �|�[�Y�̕`��󋵎擾
	static void SetEnableControlCamera(const bool bControl);	// �J�����̑���󋵐ݒ�

private:
	// �ÓI�����o�ϐ�
	static CGameManager		*m_pGameManager;	// �Q�[���}�l�[�W���[
	static CTimerManager	*m_pTimerManager;	// �^�C�}�[�}�l�[�W���[
	static CPause			*m_pPause;			// �|�[�Y

	static bool m_bControlCamera;	// �J�����̑����
	static bool m_bDrawUI;			// UI�̕`���
	static bool m_bDrawPause;		// �|�[�Y�̕`���
};

#endif	// _SCENE_GAME_H_
