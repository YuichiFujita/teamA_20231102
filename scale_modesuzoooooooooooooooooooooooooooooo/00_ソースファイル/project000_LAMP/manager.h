//============================================================
//
//	�}�l�[�W���[�w�b�_�[ [manager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CRenderer;			// �����_���[�N���X
class CInputKeyboard;		// �L�[�{�[�h�N���X
class CInputMouse;			// �}�E�X�N���X
class CInputPad;			// �p�b�h�N���X
class CSound;				// �T�E���h�N���X
class CCamera;				// �J�����N���X
class CLight;				// ���C�g�N���X
class CTexture;				// �e�N�X�`���N���X
class CModel;				// ���f���N���X
class CFade;				// �t�F�[�h�N���X
class CRetentionManager;	// �f�[�^�ۑ��}�l�[�W���[�N���X
class CDebugProc;			// �f�o�b�O�\���N���X

#ifdef _DEBUG	// �f�o�b�O����

class CDebug;	// �f�o�b�O�N���X

#endif	// _DEBUG

//************************************************************
//	�N���X��`
//************************************************************
// �}�l�[�W���[�N���X
class CManager
{
public:
	// �R���X�g���N�^
	CManager();

	// �f�X�g���N�^
	~CManager();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ������
	HRESULT Uninit(void);	// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	// �ÓI�����o�֐�
	static CManager *Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ����
	static CManager *GetInstance(void);				// �擾
	static HRESULT Release(CManager *&prManager);	// �j��

	// �����o�֐�
	void SetScene(const CScene::EMode mode, const int nWait = 0);	// �V�[���ݒ�
	HRESULT SetMode(const CScene::EMode mode);		// ���[�h�ݒ�
	CScene::EMode GetMode(void);					// ���[�h�擾

	CRenderer			*GetRenderer(void);			// �����_���[�擾
	CInputKeyboard		*GetKeyboard(void);			// �L�[�{�[�h�擾
	CInputMouse			*GetMouse(void);			// �}�E�X�擾
	CInputPad			*GetPad(void);				// �p�b�h�擾
	CSound				*GetSound(void);			// �T�E���h�擾
	CCamera				*GetCamera(void);			// �J�����擾
	CLight				*GetLight(void);			// ���C�g�擾
	CTexture			*GetTexture(void);			// �e�N�X�`���擾
	CModel				*GetModel(void);			// ���f���擾
	CFade				*GetFade(void);				// �t�F�[�h�擾
	CScene				*GetScene(void);			// �V�[���擾
	CRetentionManager	*GetRetentionManager(void);	// �f�[�^�ۑ��}�l�[�W���[�擾
	CDebugProc			*GetDebugProc(void);		// �f�o�b�O�\���擾

#ifdef _DEBUG	// �f�o�b�O����

	// �����o�֐�
	CDebug *GetDebug(void);	// �f�o�b�O�擾

#endif	// _DEBUG

private:
	// �ÓI�����o�ϐ�
	static CManager *m_pManager;	// �}�l�[�W���[

	// �����o�ϐ�
	CRenderer			*m_pRenderer;			// �����_���[�I�u�W�F�N�g
	CInputKeyboard		*m_pKeyboard;			// �L�[�{�[�h�I�u�W�F�N�g
	CInputMouse			*m_pMouse;				// �}�E�X�I�u�W�F�N�g
	CInputPad			*m_pPad;				// �p�b�h�I�u�W�F�N�g
	CSound				*m_pSound;				// �T�E���h�I�u�W�F�N�g
	CCamera				*m_pCamera;				// �J�����I�u�W�F�N�g
	CLight				*m_pLight;				// ���C�g�I�u�W�F�N�g
	CTexture			*m_pTexture;			// �e�N�X�`���I�u�W�F�N�g
	CModel				*m_pModel;				// ���f���I�u�W�F�N�g
	CFade				*m_pFade;				// �t�F�[�h�I�u�W�F�N�g
	CScene				*m_pScene;				// �V�[���I�u�W�F�N�g
	CRetentionManager	*m_pRetentionManager;	// �f�[�^�ۑ��}�l�[�W���[
	CDebugProc			*m_pDebugProc;			// �f�o�b�O�\��

#ifdef _DEBUG	// �f�o�b�O����

	// �����o�ϐ�
	CDebug *m_pDebug;	// �f�o�b�O

#endif	// _DEBUG

};

#endif	// _MANAGER_H_
