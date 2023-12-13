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
class CItem : public CObjectModel
{
public:
	// �R���X�g���N�^
	CItem();

	// �f�X�g���N�^
	~CItem();

	//<*********************************
	//�A�C�e���^�C�v�̗񋓌^
	//<*********************************
	enum TYPE
	{
		TYPE_HEAL = 0,		//�񕜃A�C�e��
		TYPE_ATTACKUP,		//�U���̓A�b�v
		TYPE_KNOCKBACKUP,	//�m�b�N�o�b�N�A�b�v
		TYPE_SUPER_ARMOR,	//�X�[�p�[�A�[�}�[
		TYPE_BIG_IRON,		//����S��
		TYPE_SLIP_FRAIL,	//���蔲����
		TYPE_DOUBLE_FRAIL,	//�_�u����
		TYPE_BURNING_FRAIL,	//�R���鍽
		TYPE_MAX
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	//�����o�֐�
	void Collision();

	static CItem *Create(D3DXVECTOR3 pos);

private:

	//�ÓI�����o�ϐ�
	static const char *mc_apModelFile[TYPE::TYPE_MAX];	//�e�N�X�`���萔

	int m_nType;

	// �����o�ϐ�
};

#endif
