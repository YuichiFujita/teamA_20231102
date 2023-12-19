//============================================================
//
//�A�C�e���X�|�[���|�C���g���� [ItemSpawnPoint.h]
//Author�Fkazuki watanebe
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ITEMSPAWNPOINT_H_
#define _ITEMSPAWNPOINT_H_

#include "main.h"
#include "object.h"
#include "Item.h"



//<**************************************
//�N���X�錾
//<**************************************
class CItemSpawnPoint : public CObject
{
public:

	//�R���X�g���N�^�E�f�X�g���N�^
	CItemSpawnPoint();
	~CItemSpawnPoint();

	//�I�[�o�[���C�h�֐�
	HRESULT Init(void) override;			//������
	void Uninit(void) override;				//�I��
	void Update(void) override;				//�X�V
	void Draw(void) override {};		// �`��

	//<****************
	//�ʒu�n
	//<****************
	void SetVec3Position(const D3DXVECTOR3& rPos) override { m_pos = rPos; }	// �O���̈ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }			// �O���̈ʒu�擾
	int GetIndex(void) const override { return m_nIdx; }						// �C���f�b�N�X�擾

	// �ÓI�����o�֐�
	static CItemSpawnPoint *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos	// �ʒu
	);

	static CObject *GetPoint(int Idx);

private:

	//�X�|�[���|�C���g�̍ő�l
#define MAX_SPAWNPOINT (20)

	// �I�[�o�[���C�h�֐�
	void Release(void) override {CObject::Release();}	// �j��

	// �����o�ϐ�
	const int m_nIdx;		//�X�|�[���ԍ�
	static int m_nNumIdx;	//�X�|�[���ԍ��̌�
	D3DXVECTOR3 m_pos;
	static D3DXVECTOR3 m_rPos[MAX_SPAWNPOINT];		//�ʒu
	static CItem *m_pItem;
	static int m_nCnt;
	int m_nRand;
};

#endif

