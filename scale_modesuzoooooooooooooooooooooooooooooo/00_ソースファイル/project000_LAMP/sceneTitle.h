//============================================================
//
//	�^�C�g����ʃw�b�_�[ [sceneTitle.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CTitleManager;	// �^�C�g���}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�g����ʃN���X
class CSceneTitle : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneTitle(const EMode mode);

	// �f�X�g���N�^
	~CSceneTitle();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CTitleManager *GetTitleManager(void);	// �^�C�g���}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CTitleManager *m_pTitleManager;	// �^�C�g���}�l�[�W���[
};

#endif	// _SCENE_TITLE_H_
