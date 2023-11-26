//============================================================
//
//	Z�e�N�X�`�������w�b�_�[ [ZTexture.h]
//	Author�F�O�� ���V��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ZTexture_H_
#define _ZTexture_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"


//************************************************************
//	�N���X��`
//************************************************************
// �A�j���[�V����2D�N���X
class CZTexture
{
public:
	// �R���X�g���N�^
	CZTexture();
	// �f�X�g���N�^
	~CZTexture();
	// ���������\�b�h
	bool Init(IDirect3DDevice9 &cpDev, UINT ZTexWidth, UINT ZTexHeight, D3DFORMAT ZTexFormat = D3DFMT_A8R8G8B8);

	// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// �r���[�s���ݒ�
	void SetViewMatrix(D3DXMATRIX *pMat);

	// �ˉe�ϊ��s���ݒ�
	void SetProjMatrix(D3DXMATRIX *pMat);

	// �`��̊J�n��錾����
	HRESULT Begin();

	// �`��̏I����錾����
	HRESULT End();

	// �p�X�̊J�n��錾����
	HRESULT BeginPass();

	// �p�X�̏I����錾����
	HRESULT EndPass();

	// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
	bool SetParamToEffect();

	// Z�l�e�N�X�`�����擾����
	bool GetZTex(IDirect3DTexture9 **cpTex);

	bool GetbPass(void) { return m_bPass; }
private:
	// �����o�ϐ�
	IDirect3DDevice9 * m_cpDev;			// �`��f�o�C�X
	IDirect3DTexture9 * m_cpZTex;		// Z�l�e�N�X�`��
	IDirect3DSurface9 * m_cpZTexSurf;	// Z�l�e�N�X�`���T�[�t�F�C�X
	IDirect3DSurface9 * m_cpDepthBuff;	// �[�x�o�b�t�@
	IDirect3DSurface9 * m_cpDevBuffer;	// �f�o�C�X�o�b�N�o�b�t�@
	IDirect3DSurface9 * m_cpDevDepth;	// �f�o�C�X�[�x�o�b�t�@
	ID3DXEffect * m_cpEffect;			// Z�l�v���b�g�G�t�F�N�g
	D3DXMATRIX m_matWorld;						// ���[���h�ϊ��s��
	D3DXMATRIX m_matView;						// �r���[�ϊ��s��
	D3DXMATRIX m_matProj;						// �ˉe�ϊ��s��
	D3DXHANDLE m_hWorldMat;						// ���[���h�ϊ��s��n���h��
	D3DXHANDLE m_hViewMat;						// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_hProjMat;						// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hTechnique;					// �e�N�j�b�N�ւ̃n���h��
	bool m_bPass;
};

#endif	// _ZTexture_H_
