//============================================================
//
//	Z�e�N�X�`�������w�b�_�[ [DepthShadow.h]
//	Author�F�O�� ���V��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _DepthShadow_H_
#define _DepthShadow_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"


//************************************************************
//	�N���X��`
//************************************************************
// �A�j���[�V����2D�N���X
class CDepthShadow
{
public:
	// �R���X�g���N�^
	CDepthShadow();
	// �f�X�g���N�^
	~CDepthShadow();
	// ���������\�b�h
	bool Init(IDirect3DDevice9 &cpDev);

	// �V���h�E�}�b�v��ݒ�
	bool SetShadowMap(IDirect3DTexture9 **cpShadowMap);

	// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	void SetWorldMatrix(D3DXMATRIX *pMat);

	// �J�����̃r���[�s���ݒ�
	void SetCameraViewMatrix(D3DXMATRIX *pMat);

	// �J�����̎ˉe�ϊ��s���ݒ�
	void SetCameraProjMatrix(D3DXMATRIX *pMat);

	// ���C�g�̃r���[�s���ݒ�
	void SetLightViewMatrix(D3DXMATRIX *pMat);

	// ���C�g�̎ˉe�ϊ��s���ݒ�
	void SetLightProjMatrix(D3DXMATRIX *pMat);

	void SetAmbient(D3DXCOLOR * pCol) { m_AmbientCol = D3DXVECTOR4(pCol->r, pCol->g, pCol->b, pCol->a); }

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

	bool GetbPass(void) { return m_bPass; }
private:
	// �����o�ϐ�
	IDirect3DDevice9 * m_cpDev;			// �`��f�o�C�X
	IDirect3DTexture9 * m_cpShadowMapTex;	// �V���h�E�}�b�v�e�N�X�`��
	ID3DXEffect * m_cpEffect;			// �[�x�o�b�t�@�V���h�E�G�t�F�N�g
	D3DXVECTOR4 m_AmbientCol;
	D3DXMATRIX m_matWorld;						// ���[���h�ϊ��s��
	D3DXMATRIX m_matCameraView;					// �J�����̃r���[�ϊ��s��
	D3DXMATRIX m_matCameraProj;					// �J�����̎ˉe�ϊ��s��
	D3DXMATRIX m_matLightView;					// ���C�g�̃r���[�ϊ��s��
	D3DXMATRIX m_matLightProj;					// ���C�g�̎ˉe�ϊ��s��
	D3DXHANDLE m_hWorldMat;						// ���[���h�ϊ��s��n���h��
	D3DXHANDLE m_hCameraViewMat;				// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_hCameraProjMat;				// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hLightViewMat;					// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_hLightProjMat;					// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hShadowMapTex;					// �V���h�E�}�b�v�e�N�X�`���n���h��
	D3DXHANDLE m_hAmbient;						// �F�n���h��
	D3DXHANDLE m_hTechnique;					// �e�N�j�b�N�ւ̃n���h��
	bool m_bPass;
};

#endif	// _DepthShadow_H_
