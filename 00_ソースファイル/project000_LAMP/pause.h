//============================================================
//
//	�|�[�Y�w�b�_�[ [pause.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �|�[�Y�N���X
class CPause
{
public:
	// �I���
	enum ESelect
	{
		SELECT_RESUME = 0,	// �ĊJ
		SELECT_EXIT,		// �I��
		SELECT_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPause();

	// �f�X�g���N�^
	~CPause();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	bool IsPause(void);	// �|�[�Y�󋵎擾
	void SetEnableDraw(const bool bDraw);	// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CPause *Create(void);				// ����
	static HRESULT Release(CPause *&prPause);	// �j��

private:
	// �����o�֐�
	void Select(void);	// �I�𑀍�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CObject2D *m_apSelect[SELECT_MAX];	// �I�����
	CObject2D *m_pBG;	// �w�i���
	int  m_nSelect;		// ���݂̑I��
	int  m_nOldSelect;	// �O��̑I��
	bool m_bPause;		// �|�[�Y��
};

#endif	// _PAUSE_H_
