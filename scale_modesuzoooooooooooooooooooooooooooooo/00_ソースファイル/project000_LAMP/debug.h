//============================================================
//
//	�f�o�b�O�w�b�_�[ [debug.h]
//	Author�F���c�E��
//
//============================================================
#ifdef _DEBUG	// �f�o�b�O����
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _DEBUG_H_
#define _DEBUG_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �f�o�b�O�N���X
class CDebug
{
public:
	// �R���X�g���N�^
	CDebug();

	// �f�X�g���N�^
	~CDebug();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void MeasureFps(const DWORD dwCurrentTime);	// FPS����
	void AddFrameCount(void);	// �t���[���J�E���g���Z
	int  GetFps(void) const;	// FPS�擾

	// �ÓI�����o�֐�
	static CDebug *Create(void);				// ����
	static HRESULT Release(CDebug *&pDebug);	// �j��

private:
	// �����o�ϐ�
	int		m_nFps;				// FPS�J�E���^
	DWORD	m_dwFrameCount;		// �t���[���J�E���g
	DWORD	m_dwFPSLastTime;	// �Ō��FPS���v����������
};

#endif	// _DEBUG_H_
#endif	// _DEBUG
