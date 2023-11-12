//============================================================
//
//	���U���g��ʃw�b�_�[ [sceneResult.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_RESULT_H_
#define _SCENE_RESULT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CResultManager;	// ���U���g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g��ʃN���X
class CSceneResult : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneResult(const EMode mode);

	// �f�X�g���N�^
	~CSceneResult();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CResultManager *GetResultManager(void);	// ���U���g�}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CResultManager *m_pResultManager;	// ���U���g�}�l�[�W���[
};

#endif	// _SCENE_RESULT_H_
