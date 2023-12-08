//============================================================
//
//	�A�C�e���X�|�[���w�b�_�[ [itemspawn.h]
//	Author�F�X��x��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ITEMSPAWN_H_
#define _ITEMSPAWN_H_
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectmodel.h"

//************************************************************
//	�N���X��`
//************************************************************
// �A�C�e���N���X
class CItemSpawn : public CObjectModel
{
public:
	// �A�C�e����
	enum ItemSpawn
	{
		ITEM_HEAL = 0,		// ��
		ITEM_ATTACKUP,		// �U���̓A�b�v
		ITEM_KNOCKBACK,		// �m�b�N�o�b�N�A�b�v
		ITEM_SUPERARMOR,	// �X�[�p�[�A�[�}�[
		ITEM_BIGIRONBALL,	// ����S��
		ITEM_TPIRONBALL,	// �����S��
		ITEM_LONGCHAIN,		// ������
		ITEM_DOUBLEFLAIL,	// �����t���C��
		ITEM_FILEFLAIL,		// �R���Ă�S��
		ITEM_MAX
	};

	// �R���X�g���N�^
	CItemSpawn();

	// �f�X�g���N�^
	~CItemSpawn();

	// �����o�֐�

private:
	// �ÓI�����o�ϐ�

	// �����o�ϐ�
};

#endif
