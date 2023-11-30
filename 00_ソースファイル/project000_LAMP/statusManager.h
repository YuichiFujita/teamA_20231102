//============================================================
//
//	�X�e�[�^�X�}�l�[�W���[�w�b�_�[ [statusManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STATUS_MANAGER_H_
#define _STATUS_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiValue;	// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�^�X�}�l�[�W���[�N���X
class CStatusManager
{
public:
	// �R���X�g���N�^
	explicit CStatusManager(const int nPadID);

	// �f�X�g���N�^
	~CStatusManager();

	// �����o�֐�
	HRESULT Init(void);		// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	void AddNumLife(const int nAdd);			// �̗͉��Z
	void SetNumLife(const int nSet);			// �̗͐ݒ�
	int  GetNumLife(void) const;				// �̗͎擾
	int  GetNumMaxLife(void) const;				// �ő�̗͎擾
	void SetEnableDrawLife(const bool bDraw);	// �̗͂̕`��ݒ�
	void AddNumRate(const int nAdd);			// ������ї����Z
	void SetNumRate(const int nSet);			// ������ї��ݒ�
	int  GetNumRate(void) const;				// ������ї��擾
	int  GetNumMaxRate(void) const;				// �ő吁����ї��擾
	void SetEnableDrawRate(const bool bDraw);	// ������ї��̕`��ݒ�

	// �ÓI�����o�֐�
	static CStatusManager *Create(const int nPadID);	// ����
	static HRESULT Release(CStatusManager *&prStatusManager);	// �j��

private:
	// �����o�ϐ�
	CMultiValue *m_pLife;		// �̗͂̏��
	CMultiValue *m_pKnockRate;	// ������ї��̏��

	const int m_nPadID;	// �p�b�hID�萔
};

#endif	// _STATUS_MANAGER_H_
