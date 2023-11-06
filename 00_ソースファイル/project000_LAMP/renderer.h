//============================================================
//
//	�����_���[�w�b�_�[ [renderer.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����_���[�N���X
class CRenderer
{
public:
	// �R���X�g���N�^
	CRenderer();

	// �f�X�g���N�^
	~CRenderer();

	// �����o�֐�
	HRESULT Init(HWND hWnd, BOOL bWindow);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��

	HRESULT CreateRenderTexture(void);			// �����_�[�e�N�X�`���[����
	LPDIRECT3DDEVICE9 GetDevice(void) const;	// �f�o�C�X�擾

	// �ÓI�����o�֐�
	static CRenderer *Create(HWND hWnd, BOOL bWindow);	// ����
	static HRESULT Release(CRenderer *&prRenderer);		// �j��

private:
	// �����o�֐�
	HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// �f�o�C�X����

	// �����o�ϐ�
	LPDIRECT3D9			m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Direct3D�f�o�C�X

	int m_nRenderTextureID;		// �����_�[�e�N�X�`���̃C���f�b�N�X
	CObject2D *m_pDrawScreen;	// ��ʕ`��p��2D�|���S��
	LPDIRECT3DSURFACE9	m_pRenderTextureSurface;	// �`��T�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DSURFACE9	m_pDepthStencilSurface;		// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DSURFACE9	m_pDefRenderTextureSurface;	// ���̕`��T�[�t�F�C�X�ۑ��p
	LPDIRECT3DSURFACE9	m_pDefDepthStencilSurface;	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p
};

#endif	// _RENDERER_H_
