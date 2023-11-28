//============================================================
//
//	�^�C�}�[�}�l�[�W���[�w�b�_�[ [timerManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIMERMANAGER_H_
#define _TIMERMANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_MIN		(2)	// �^�C�}�[�̕��̌���
#define MAX_SEC		(2)	// �^�C�}�[�̕b�̌���
#define MAX_MSEC	(3)	// �^�C�}�[�̃~���b�̌���
#define MAX_PART	(2)	// ��؂�̍ő吔

#define MAX_TIMER	(MAX_MIN + MAX_SEC + MAX_MSEC)	// �^�C�}�[�̌��� (���E�b�E�~���b)

//************************************************************
//	�O���錾
//************************************************************
class CValue;		// �����N���X
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C�}�[�}�l�[�W���[�N���X
class CTimerManager
{
public:
	// �e�N�X�`����
	typedef enum
	{
		TEXTURE_PART = 0,	// ��؂�\���̃e�N�X�`��
		TEXTURE_LOGO,		// ���S�\���̃e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	}TEXTURE;

	// �^�C����
	typedef enum
	{
		TIME_MSEC,	// �~���b
		TIME_SEC,	// �b
		TIME_MIN,	// ��
		TIME_MAX	// ���̗񋓌^�̑���
	}TIME;

	// �v����
	typedef enum
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_MEASURE,	// �v����
		STATE_END,		// �v���I�� 
		STATE_MAX		// ���̗񋓌^�̑���
	}STATE;

	// �R���X�g���N�^
	CTimerManager();

	// �f�X�g���N�^
	~CTimerManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Start(void);	// �v���J�n
	void End(void);		// �v���I��
	void EnableStop(const bool bStop);	// �v����~�ݒ�
	STATE GetState(void);		// �v����Ԏ擾
	bool AddMSec(long nMSec);	// �~���b���Z
	bool AddSec(long nSec);		// �b���Z
	bool AddMin(long nMin);		// �����Z
	bool SetMSec(long nMSec);	// �~���b�ݒ�
	bool SetSec(long nSec);		// �b�ݒ�
	bool SetMin(long nMin);		// ���ݒ�
	int Get(void);				// �^�C���擾
	int GetMSec(void);			// �~���b�擾
	int GetSec(void);			// �b�擾
	int GetMin(void);			// ���擾
	long GetLimit(void);		// �������Ԏ擾
	void SetLimit(const TIME time, const long nTime);	// �������Ԑݒ�
	void SetPosition(const D3DXVECTOR3& rPos);			// �ʒu�ݒ�
	void SetScalingValue(const D3DXVECTOR3& rSize);		// ��؂�̑傫���ݒ�
	void SetScalingPart(const D3DXVECTOR3& rSize);		// �����̑傫���ݒ�
	void SetSpaceValue(const D3DXVECTOR3& rSpace);		// ��؂�̋󔒐ݒ�
	void SetSpacePart(const D3DXVECTOR3& rSpace);		// �����̋󔒐ݒ�
	void SetColor(const D3DXCOLOR& rCol);				// �F�̐ݒ�
	void SetPriority(const int nPriority);				// �D�揇�ʐݒ�
	void SetEnableUpdate(const bool bUpdate);			// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw);				// �`��󋵐ݒ�
	void SetEnableLogoDraw(const bool bDraw);			// ���S�`��󋵐ݒ�
	D3DXVECTOR3 GetPosition(void) const;				// �ʒu�擾
	D3DXVECTOR3 GetScalingValue(void) const;			// ��؂�̑傫���擾
	D3DXVECTOR3 GetScalingPart(void) const;				// �����̑傫���擾
	D3DXVECTOR3 GetSpaceValue(void) const;				// ��؂�̋󔒎擾
	D3DXVECTOR3 GetSpacePart(void) const;				// �����̋󔒎擾

	// �ÓI�����o�֐�
	static CTimerManager *Create	// ����
	( // ����
		const TIME time,				// �ݒ�^�C��
		const long nTime,				// ��������
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSizeValue,	// �����̑傫��
		const D3DXVECTOR3& rSizePart,	// ��؂�̑傫��
		const D3DXVECTOR3& rSpaceValue,	// �����̋�
		const D3DXVECTOR3& rSpacePart	// ��؂�̋�
	);
	static HRESULT Release(CTimerManager *&prTimerManager);	// �j��
	static DWORD GetMinTime(void);	// �ŏ��^�C���擾
	static DWORD GetMaxTime(void);	// �ő�^�C���擾

private:
	// �����o�֐�
	void SetDrawValue(void);	// �����̕\���ݒ�
	void SetTexNum(void);		// �����̃e�N�X�`�����W�ݒ�

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔

	// �����o�ϐ�
	CValue *m_apValue[MAX_TIMER];	// ���l�̏��
	CObject2D *m_apPart[MAX_PART];	// ��؂�̏��
	CObject2D *m_pLogo;				// ���S�̏��
	D3DXVECTOR3 m_pos;				// �ʒu
	D3DXVECTOR3 m_sizeValue;		// �����̑傫��
	D3DXVECTOR3 m_sizePart;			// ��؂�̑傫��
	D3DXVECTOR3 m_spaceValue;		// �����̋�
	D3DXVECTOR3 m_spacePart;		// ��؂�̋�
	DWORD m_dwStartTime;			// �J�n����
	DWORD m_dwTime;					// �o�ߎ���
	DWORD m_dwStopStartTime;		// ��~�J�n����
	DWORD m_dwStopTime;				// ��~����
	DWORD m_dwTempTime;				// �o�ߎ��Ԃ̌v�Z�p
	STATE m_state;					// �v�����
	bool  m_bStop;					// �v����~��
	long  m_nLimit;					// ��������
};

#endif	// _TIMERMANAGER_H_
