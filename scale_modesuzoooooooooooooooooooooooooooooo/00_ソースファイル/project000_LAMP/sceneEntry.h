//============================================================
//
//	�G���g���[��ʃw�b�_�[ [sceneEntry.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_ENTRY_H_
#define _SCENE_ENTRY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CEntryManager;	// �G���g���[�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �G���g���[��ʃN���X
class CSceneEntry : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneEntry(const EMode mode);

	// �f�X�g���N�^
	~CSceneEntry();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CEntryManager *GetEntryManager(void);	// �G���g���[�}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CEntryManager *m_pEntryManager;	// �G���g���[�}�l�[�W���[
};

#endif	// _SCENE_ENTRY_H_
