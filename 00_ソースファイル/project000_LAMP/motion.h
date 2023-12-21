//============================================================
//
//	���[�V�����w�b�_�[ [motion.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_PARTS	(24)	// �p�[�c�̍ő吔
#define MAX_MOTION	(32)	// ���[�V�����̍ő吔
#define MAX_KEY		(32)		// �L�[�̍ő吔

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���[�V�����N���X
class CMotion
{
public:
	// �R���X�g���N�^
	CMotion();

	// �f�X�g���N�^
	~CMotion();

	// �p�[�c�Ǘ��\����
	struct SKey
	{
		D3DXVECTOR3 pos;	// ���f���ʒu
		D3DXVECTOR3 rot;	// ���f������
	};

	// �|�[�Y�Ǘ��\����
	struct SKeyInfo
	{
		SKey aKey[MAX_PARTS];	// �L�[���f�����
		int  nFrame;	// �L�[�Đ��t���[����
	};

	// �L�[�Ǘ��\����
	struct SMotionInfo
	{
		SKeyInfo aKeyInfo[MAX_KEY];	// �L�[���
		int  nNumKey;	// �L�[����
		bool bLoop;		// ���[�v ON/OFF
	};

	// ���[�V�����Ǘ��\����
	struct SInfo
	{
		SMotionInfo aMotionInfo[MAX_MOTION];	// ���[�V�������
		int  nNumMotion;	// ���[�V��������
		int  nType;			// ���[�V�������
		int  nPose;			// ���[�V�����|�[�Y�ԍ�
		int  nCounter;		// ���[�V�����J�E���^�[
		bool bFinish;		// ���[�V�����I����
	};

	// �����o�֐�
	HRESULT Init(void);			// ������
	void Uninit(void);			// �I��
	void Update(void);			// �X�V
	void Set(const int nType);	// �ݒ�
	void SetInfo(const SMotionInfo info);					// ���[�V�������ݒ�
	void SetEnableUpdate(const bool bUpdate);				// �X�V�󋵐ݒ�
	void SetModel(CMultiModel **ppModel, const int nNum);	// ���f�����ݒ�
	int  GetType(void) const;			// ��ގ擾
	int  GetPose(void) const;			// �|�[�Y�ԍ��擾
	int  GetCounter(void) const;		// ���[�V�����J�E���^�[�擾
	bool IsFinish(void) const;			// �I���擾
	bool IsLoop(const int nType) const;	// ���[�v�擾

	// �ÓI�����o�֐�
	static CMotion *Create(void);				// ����
	static HRESULT Release(CMotion *&prMotion);	// �j��

private:
	// �����o�ϐ�
	SInfo m_info;				// ���[�V�������
	CMultiModel **m_ppModel;	// ���f�����
	int m_nNumModel;			// ���f���̃p�[�c��
	bool m_bUpdate;				// �X�V��
};

#endif	// _MOTION_H_
