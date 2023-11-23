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
class CZTexture;	//Z�e�N�X�`��
class CDepthShadow; //�[�x�V���h�E
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
	IDirect3DDevice9 * m_pDev; //�`��f�o�C�X

	int m_nRenderTextureID;		// �����_�[�e�N�X�`���̃C���f�b�N�X
	CObject2D *m_pDrawScreen;	// ��ʕ`��p��2D�|���S��
	ID3DXSprite * m_pSprite;	 //�V���h�E�p�X�v���C�g
	CZTexture * m_pZTex;		//Z�e�N�X�`�������p
	CDepthShadow * m_pDepthShadow;					//�[�x�e
	D3DXMATRIX CameraView, CameraProj;	// �J�����r���[�ϊ��E�ˉe�ϊ�
	D3DXMATRIX LightView, LightProj;	// ���C�g�r���[�ϊ��E�ˉe�ϊ�

	IDirect3DTexture9 * m_pZTexture;				//Z�e�N�X�`���{��
	LPDIRECT3DSURFACE9	m_pRenderTextureSurface;	// �`��T�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DSURFACE9	m_pDepthStencilSurface;		// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
	LPDIRECT3DSURFACE9	m_pDefRenderTextureSurface;	// ���̕`��T�[�t�F�C�X�ۑ��p
	LPDIRECT3DSURFACE9	m_pDefDepthStencilSurface;	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p
};

#endif	// _RENDERER_H_
