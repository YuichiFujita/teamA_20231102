//============================================================
//
//	�`���[�g���A����ʃw�b�_�[ [sceneTutorial.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_TUTORIAL_H_
#define _SCENE_TUTORIAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CTutorialManager;	// �`���[�g���A���}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �`���[�g���A����ʃN���X
class CSceneTutorial : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneTutorial(const EMode mode);

	// �f�X�g���N�^
	~CSceneTutorial();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CTutorialManager *GetTutorialManager(void);	// �`���[�g���A���}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CTutorialManager *m_pTutorialManager;	// �`���[�g���A���}�l�[�W���[
};

#endif	// _SCENE_TUTORIAL_H_
