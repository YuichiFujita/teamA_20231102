//============================================================
//
//	�����L���O��ʃw�b�_�[ [sceneRanking.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_RANKING_H_
#define _SCENE_RANKING_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CRankingManager;	// �����L���O�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����L���O��ʃN���X
class CSceneRanking : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneRanking(const EMode mode);

	// �f�X�g���N�^
	~CSceneRanking();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	HRESULT Uninit(void) override;	// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CRankingManager *GetRankingManager(void);	// �����L���O�}�l�[�W���[�擾

private:
	// �ÓI�����o�ϐ�
	static CRankingManager *m_pRankingManager;	// �����L���O�}�l�[�W���[
};

#endif	// _SCENE_RANKING_H_
