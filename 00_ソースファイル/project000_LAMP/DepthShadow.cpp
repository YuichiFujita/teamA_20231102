//============================================================
//
//	�[�x�o�b�t�@�ɂ��e [DepthShadow.cpp]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "DepthShadow.h"

#include <tchar.h>



// �R���X�g���N�^
CDepthShadow::CDepthShadow()
{
}


// �f�X�g���N�^
CDepthShadow::~CDepthShadow()
{
}


// ���������\�b�h
bool CDepthShadow::Init(IDirect3DDevice9 &cpDev )
{
	// �f�o�C�X�̓o�^�ƃG�t�F�N�g�n���h���̎擾
	if( &cpDev == NULL ) return false;
	ID3DXBuffer* pError = NULL;

	if(FAILED( D3DXCreateEffectFromFile(
		&cpDev,
		("data\\SHADER\\DepthBufShadowEffect.fx"),
		NULL,
		NULL,
		0,
		NULL,
		&m_cpEffect,
		&pError)))
		if (pError)
		{
			OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
			//�f�o�b�O�R���\�[���ɕ\������
			MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			return false;
		}


	m_hWorldMat		 = m_cpEffect->GetParameterByName( NULL, "matWorld" );
	m_hCameraViewMat = m_cpEffect->GetParameterByName( NULL, "matCameraView" );
	m_hCameraProjMat = m_cpEffect->GetParameterByName( NULL, "matCameraProj" );
	m_hLightViewMat  = m_cpEffect->GetParameterByName( NULL, "matLightView" );
	m_hLightProjMat  = m_cpEffect->GetParameterByName( NULL, "matLightProj" );
	m_hShadowMapTex  = m_cpEffect->GetParameterByName( NULL, "texShadowMap" );
	m_hTechnique = m_cpEffect->GetTechniqueByName( "DepthBufShadowTec" );

	if( !m_hWorldMat || !m_hCameraViewMat || !m_hCameraProjMat
		|| !m_hLightViewMat || !m_hLightProjMat || !m_hShadowMapTex || !m_hTechnique )
		return false;
	
	m_cpDev = &cpDev;
	return true;
}


// �V���h�E�}�b�v��ݒ�
bool CDepthShadow::SetShadowMap(IDirect3DTexture9 &cpShadowMap )
{
	if( &cpShadowMap == NULL )
		return false;

	m_cpShadowMapTex = &cpShadowMap;
	return true;
}


// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
void CDepthShadow::SetWorldMatrix( D3DXMATRIX *pMat )
{
	m_matWorld = *pMat;
}

// �J�����̃r���[�s���ݒ�
void CDepthShadow::SetCameraViewMatrix( D3DXMATRIX *pMat )
{
	m_matCameraView = *pMat;
}


// �J�����̎ˉe�ϊ��s���ݒ�
void CDepthShadow::SetCameraProjMatrix( D3DXMATRIX *pMat )
{
	m_matCameraProj = *pMat;
}


// ���C�g�̃r���[�s���ݒ�
void CDepthShadow::SetLightViewMatrix( D3DXMATRIX *pMat )
{
	m_matLightView = *pMat;
}


// ���C�g�̎ˉe�ϊ��s���ݒ�
void CDepthShadow::SetLightProjMatrix( D3DXMATRIX *pMat )
{
	m_matLightProj = *pMat;
}


// �`��̊J�n��錾����
HRESULT CDepthShadow::Begin()
{
	// �e�T�[�t�F�C�X��������
	m_cpDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );

	// �v���O���}�u���V�F�[�_�ɐ؂�ւ�
	m_cpEffect->SetTechnique( m_hTechnique );

	// �J�n�錾
	UINT Pass;
	m_cpEffect->Begin(&Pass, 0);

	return S_OK;
}


// �`��̏I����錾����
HRESULT CDepthShadow::End()
{
	m_cpEffect->End();

	// �Œ�@�\�ɖ߂�
	m_cpDev->SetVertexShader( NULL );
	m_cpDev->SetPixelShader( NULL );

	return S_OK;
}


// �p�X�̊J�n��錾����
HRESULT CDepthShadow::BeginPass()
{
	m_cpEffect->BeginPass(0);	// �p�X��1�̂�
	return S_OK;
}


// �p�X�̏I����錾����
HRESULT CDepthShadow::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
bool CDepthShadow::SetParamToEffect()
{
	m_cpEffect->SetMatrix( m_hWorldMat, &m_matWorld );
	m_cpEffect->SetMatrix( m_hCameraViewMat, &m_matCameraView );
	m_cpEffect->SetMatrix( m_hCameraProjMat, &m_matCameraProj );
	m_cpEffect->SetMatrix( m_hLightViewMat, &m_matLightView );
	m_cpEffect->SetMatrix( m_hLightProjMat, &m_matLightProj );
	HRESULT hr = m_cpEffect->SetTexture( m_hShadowMapTex, m_cpShadowMapTex);

	return true;
}