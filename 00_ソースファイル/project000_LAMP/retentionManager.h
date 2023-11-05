//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[�w�b�_�[ [retentionManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RETENTION_MANAGER_H_
#define _RETENTION_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �f�[�^�ۑ��}�l�[�W���[�N���X
class CRetentionManager
{
public:
	// ���U���g��
	enum EResult
	{
		RESULT_NONE,	// ����
		RESULT_FAILED,	// �N���A���s
		RESULT_CLEAR,	// �N���A����
		RESULT_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CRetentionManager();

	// �f�X�g���N�^
	~CRetentionManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��

	// �ÓI�����o�֐�
	static CRetentionManager *Create(void);	// ����
	static HRESULT Release(CRetentionManager *&prRetentionManager);	// �j��

	// �����o�֐�
	void SetResult(const EResult result);	// �N���A�󋵐ݒ�
	EResult GetResult(void) const;			// �N���A�󋵎擾
	void SetTime(const long nTime);			// �o�߃^�C���ݒ�
	long GetTime(void) const;				// �o�߃^�C���擾

private:
	// �����o�ϐ�
	EResult m_result;	// �N���A��
	long m_nTime;		// �o�߃^�C��
};

#endif	// _RETENTION_MANAGER_H_
