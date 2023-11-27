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
class CObjectGauge2D;	// �I�u�W�F�N�g�Q�[�W2D�N���X
class CValueUI;			// ����UI�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�^�X�}�l�[�W���[�N���X
class CStatusManager
{
public:
	// �R���X�g���N�^
	CStatusManager();

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
	void SetEnableDrawLife(const bool bDraw);	// �̗͂̕`��ݒ�
	void AddNumRate(const int nAdd);			// ������ї����Z
	void SetNumRate(const int nSet);			// ������ї��ݒ�
	int  GetNumRate(void) const;				// ������ї��擾
	void SetEnableDrawRate(const bool bDraw);	// ������ї��̕`��ݒ�

	// �ÓI�����o�֐�
	static CStatusManager *Create(void);	// ����
	static HRESULT Release(CStatusManager *&prStatusManager);	// �j��

private:
	// �����o�ϐ�
	CObjectGauge2D	*m_pLife;		// �̗͂̏��
	CValueUI		*m_pKnockRate;	// ������ї��̏��
};

#endif	// _STATUS_MANAGER_H_
