//============================================================
//
//	�v���C���[�G���g���[�w�b�_�[ [playerEntry.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_RESULT_H_
#define _PLAYER_RESULT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�G���g���[�N���X
class CPlayerResult : public CPlayer
{
public:
	// �R���X�g���N�^
	CPlayerResult(const CScene::EMode mode, const int nPad);

	// �f�X�g���N�^
	~CPlayerResult();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

private:
	// �����o�֐�
	void SetEntry(void);	// �G���g���[�ݒ�
};

#endif	// _PLAYER_ENTRY_H_

