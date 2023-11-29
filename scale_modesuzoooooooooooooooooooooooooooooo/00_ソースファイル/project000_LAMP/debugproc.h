//==========================================================
//
//�f�o�b�O�\������ [debugproc.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//���̃}�N������`����Ă��Ȃ��ꍇ
#define _DEBUGPROC_H_

#include "main.h"

//**********************************************************
//�}�N����`
//**********************************************************
#define MAX_DEBUGSTRING	(2048)	//�f�o�b�O�\���̍ő啶����

//**********************************************************
//�N���X�̒�`
//**********************************************************
class CDebugProc
{
public:
	// �f�o�b�O�ʒu��
	enum EPoint
	{
		POINT_CENTER = 0,	// ����
		POINT_LEFT,			// ��
		POINT_RIGHT,		// �E
		POINT_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CDebugProc();

	// �f�X�g���N�^
	~CDebugProc();

	// �����o�֐�
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Print(const EPoint point, const char *fmt, ...);

	// �ÓI�����o�֐�
	static CDebugProc *Create(void);	// ����
	static HRESULT Release(CDebugProc *&pDebugProc);	// �j��

private:
	static LPD3DXFONT m_pFont;	// �t�H���g�ւ̃|�C���^
	bool m_bDisp;				// �f�o�b�O�\����ON/OFF
	char m_aStr[POINT_MAX][MAX_DEBUGSTRING];	// �f�o�b�O�\���p�̕�����
};

#endif	// _DEBUGPROC_H_
