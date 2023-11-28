//============================================================
//
//	�v���C���[�G���g���[�w�b�_�[ [playerEntry.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_ENTRY_H_
#define _PLAYER_ENTRY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "player.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�G���g���[�N���X
class CPlayerEntry : public CPlayer
{
public:
	// �R���X�g���N�^
	explicit CPlayerEntry(const int nPad);

	// �f�X�g���N�^
	~CPlayerEntry();

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
