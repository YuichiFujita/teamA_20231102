//============================================================
//
//	���C�g���� [light.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�e�N���X [CLight] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLight::CLight()
{
	// �����o�ϐ����N���A
	memset(&m_aLight[0], 0, sizeof(m_aLight));	// ���C�g�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLight::~CLight()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLight::Init(void)
{
	// �ϐ��z���錾
	D3DXCOLOR	aDiffuse[MAX_LIGHT];	// �ݒ�p�g�U���J���[
	D3DXVECTOR3	aVecDir[MAX_LIGHT];		// �ݒ�p�����x�N�g��

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �g�U���J���[��ݒ�
	aDiffuse[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	aDiffuse[1] = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	aDiffuse[2] = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	aDiffuse[3] = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);

	// �����x�N�g����ݒ�
	aVecDir[0] = D3DXVECTOR3( 0.22f, -0.97f,  0.54f);
	aVecDir[1] = D3DXVECTOR3(-0.38f,  0.78f, -0.54f);
	aVecDir[2] = D3DXVECTOR3( 0.89f, -0.21f,  0.44f);
	aVecDir[3] = D3DXVECTOR3(-0.96f,  0.15f, -0.44f);

	// �����o�ϐ���������
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// �\���̗̂v�f���N���A
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		m_aLight[nCntLight].Diffuse = aDiffuse[nCntLight];

		// ���C�g�̕�����ݒ�
		D3DXVec3Normalize(&aVecDir[nCntLight], &aVecDir[nCntLight]);	// �x�N�g���𐳋K��
		m_aLight[nCntLight].Direction = aVecDir[nCntLight];

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLight::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CLight::Update(void)
{

}

//============================================================
//	��������
//============================================================
CLight *CLight::Create(void)
{
	// �|�C���^��錾
	CLight *pLight = NULL;	// ���C�g�����p

	if (pLight == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pLight = new CLight;	// ���C�g
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pLight != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ���C�g�̏�����
		if (FAILED(pLight->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pLight;
			pLight = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLight;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CLight::Release(CLight *&prLight)
{
	if (prLight != NULL)
	{ // �g�p���̏ꍇ

		// ���C�g�̏I��
		prLight->Uninit();

		// �������J��
		delete prLight;
		prLight = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
