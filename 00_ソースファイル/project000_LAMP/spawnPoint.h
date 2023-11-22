//============================================================
//
//	�����ʒu�w�b�_�[ [spawnPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SPAWN_POINT_H_
#define _SPAWN_POINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����ʒu�N���X
class CSpawnPoint : public CObject
{
public:
	// �R���X�g���N�^
	CSpawnPoint();

	// �f�X�g���N�^
	~CSpawnPoint();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �O���̈ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �O���̈ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	int GetIndex(void) const override;						// �C���f�b�N�X�擾

	// �ÓI�����o�֐�
	static CSpawnPoint *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot		// ����
	);
	static CObject *GetSavePoint(int Idx);

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	const int m_Idx;		//�X�|�[���ԍ�
	static int m_nNumIdx;	//�X�|�[���ԍ��̌�
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_rot;		//����
};

#endif	// _SPAWN_POINT_H_
