//============================================================
//
//	�����_���[���� [renderer.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "renderState.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "ZTexture.h"
#include "DepthShadow.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
    const D3DFORMAT FORMAT_DEPTH_STENCIL = D3DFMT_D16;					// �[�x�X�e���V���̃t�H�[�}�b�g
    const DWORD		FLAG_CLEAR	= D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;	// �N���A����o�b�t�@�[�t���O
    const D3DCOLOR	COL_CLEAR	= D3DCOLOR_RGBA(255, 255, 255, 255);	// �N���A���̐F
}

//************************************************************
//	�e�N���X [CRenderer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderer::CRenderer()
{
    // �����o�ϐ����N���A
    m_pD3D			= NULL;	// Direct3D�I�u�W�F�N�g
    m_pD3DDevice	= NULL;	// Direct3D�f�o�C�X

    m_nRenderTextureID			= 0;	// �����_�[�e�N�X�`���̃C���f�b�N�X
    m_pDrawScreen				= NULL;	// ��ʕ`��p��2D�|���S��
    m_pRenderTextureSurface		= NULL;	// �`��T�[�t�F�C�X�ւ̃|�C���^
    m_pDepthStencilSurface		= NULL;	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
    m_pDefRenderTextureSurface	= NULL;	// ���̕`��T�[�t�F�C�X�ۑ��p
    m_pDefDepthStencilSurface	= NULL;	// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderer::~CRenderer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
    // �ϐ���錾
    D3DDISPLAYMODE			d3ddm;	// �f�B�X�v���C���[�h
    D3DPRESENT_PARAMETERS	d3dpp;	// �v���[���e�[�V�����p�����[�^

    // �����o�ϐ���������
    m_pD3D			= NULL;	// Direct3D�I�u�W�F�N�g
    m_pD3DDevice	= NULL;	// Direct3D�f�o�C�X

    m_nRenderTextureID			= NONE_IDX;	// �����_�[�e�N�X�`���̃C���f�b�N�X
    m_pDrawScreen				= NULL;		// ��ʕ`��p��2D�|���S��
    m_pRenderTextureSurface		= NULL;		// �`��T�[�t�F�C�X�ւ̃|�C���^
    m_pDepthStencilSurface		= NULL;		// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
    m_pDefRenderTextureSurface	= NULL;		// ���̕`��T�[�t�F�C�X�ۑ��p
    m_pDefDepthStencilSurface	= NULL;		// ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ۑ��p

    // Direct3D�I�u�W�F�N�g�̐���
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3D == NULL)
    { // �I�u�W�F�N�g�̐����Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // ���݂̃f�B�X�v���C���[�h���擾
    if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    { // �f�B�X�v���C���[�h�̎擾�Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
    ZeroMemory(&d3dpp, sizeof(d3dpp));			// �p�����[�^�̃[���N���A
    d3dpp.BackBufferWidth	= SCREEN_WIDTH;		// �Q�[����ʃT�C�Y (��)
    d3dpp.BackBufferHeight	= SCREEN_HEIGHT;	// �Q�[����ʃT�C�Y (����)
    d3dpp.BackBufferFormat	= d3ddm.Format;		// �o�b�N�o�b�t�@�̌`��
    d3dpp.BackBufferCount	= 1;				// �o�b�N�o�b�t�@�̐�
    d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ� (�f���M���ɓ���)
    d3dpp.EnableAutoDepthStencil	= TRUE;					// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
    d3dpp.AutoDepthStencilFormat	= FORMAT_DEPTH_STENCIL;	// �f�v�X�o�b�t�@�Ƃ��� 16bit ���g��
    d3dpp.Windowed					= bWindow;				// �E�C���h�E���[�h
    d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
    d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

    // Direct3D�f�o�C�X�̐���
    if (FAILED(CreateDevice(hWnd, d3dpp)))
    { // �f�o�C�X�̐����Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

	// �����_�[�X�e�[�g���̏�����
	CRenderState::InitRenderState(m_pD3DDevice);

    // �T���v���[�X�e�[�g�̐ݒ� (�e�N�X�`���̊g�k��Ԃ̐ݒ�)
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

    // �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ� (�e�N�X�`���̃A���t�@�u�����h�̐ݒ�)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);



	// �V�F�[�_�[�p�̏������͈ȉ�
	// Z�l�e�N�X�`�������I�u�W�F�N�g�̐����Ə�����
	m_pDev = m_pD3DDevice;
	D3DXCreateSprite(m_pD3DDevice, &m_pSprite);// �X�v���C�g�쐬
	m_pZTex = new CZTexture;
	m_pZTex->Init(*m_pDev, SCREEN_WIDTH, SCREEN_WIDTH, D3DFMT_A16B16G16R16);
	m_pZTex->GetZTex(&m_pZTexture);
	// �[�x�o�b�t�@�V���h�E�I�u�W�F�N�g�̐����Ə�����
	m_pDepthShadow = new CDepthShadow;
	m_pDepthShadow->Init(*m_pDev);
	m_pDepthShadow->SetShadowMap(&m_pZTexture);	// �V���h�E�}�b�v�e�N�X�`����o�^
	D3DXMatrixPerspectiveFovLH(&CameraProj, D3DXToRadian(45), 1.777f, 10.0f, 50000.0f);
	D3DXMatrixPerspectiveFovLH(&LightProj, D3DXToRadian(90), 1.0f, 30.0f, 50000.0f);
	D3DXMatrixLookAtLH(&LightView, &D3DXVECTOR3(300.0f,750.0f,150.0f), &D3DXVECTOR3(0.0f, -10.0f, 0.0f), &D3DXVECTOR3(0, 1, 0));
	// Z�l�e�N�X�`��OBJ�֓o�^
	m_pZTex->SetViewMatrix(&LightView);
	m_pZTex->SetProjMatrix(&LightProj);
	// �[�x�o�b�t�@�V���h�EOBJ�֓o�^
	// �J�����r���[�͖���ς��̂ŕ`�掞�ɓo�^���܂�
	m_pDepthShadow->SetLightViewMatrix(&LightView);
	m_pDepthShadow->SetLightProjMatrix(&LightProj);
	m_pDepthShadow->SetCameraProjMatrix(&CameraProj);
	
    // ������Ԃ�
    return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderer::Uninit(void)
{
    if (m_pDrawScreen != NULL)
    { // ��ʕ`��p��2D�|���S�����g�p���̏ꍇ

        // ��ʕ`��p��2D�|���S���̏I��
        m_pDrawScreen->Uninit();
    }

    // Direct3D�f�o�C�X�̔j��
    if (m_pD3DDevice != NULL)
    { // Direct3D�f�o�C�X���g�p���̏ꍇ

        // �������J��
        m_pD3DDevice->Release();
        m_pD3DDevice = NULL;
    }

    // Direct3D�I�u�W�F�N�g�̔j��
    if (m_pD3D != NULL)
    { // Direct3D�I�u�W�F�N�g���g�p���̏ꍇ

        // �������J��
        m_pD3D->Release();
        m_pD3D = NULL;
    }

    // �`��T�[�t�F�C�X�̔j��
    if (m_pRenderTextureSurface != NULL)
    { // �`��T�[�t�F�C�X���g�p���̏ꍇ

        // �������J��
        m_pRenderTextureSurface->Release();
        m_pRenderTextureSurface = NULL;
    }

    // Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�̔j��
    if (m_pDepthStencilSurface != NULL)
    { // Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X���g�p���̏ꍇ

        // �������J��
        m_pDepthStencilSurface->Release();
        m_pDepthStencilSurface = NULL;
    }

    // ���̕`��T�[�t�F�C�X�̔j��
    if (m_pDefRenderTextureSurface != NULL)
    { // ���̕`��T�[�t�F�C�X���g�p���̏ꍇ

        // �������J��
        m_pDefRenderTextureSurface->Release();
        m_pDefRenderTextureSurface = NULL;
    }

    // ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�j��
    if (m_pDefDepthStencilSurface != NULL)
    { // ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X���g�p���̏ꍇ

        // �������J��
        m_pDefDepthStencilSurface->Release();
        m_pDefDepthStencilSurface = NULL;
    }
	if (m_pDepthShadow != NULL)
	{
		delete m_pDepthShadow;
		m_pDepthShadow = NULL;
	}
	if (m_pZTex != NULL)
	{
		delete m_pZTex;
		m_pZTex = NULL;
	}
}

//============================================================
//	�X�V����
//============================================================
void CRenderer::Update(void)
{
    // �I�u�W�F�N�g�̑S�X�V
    CObject::UpdateAll();
}

//============================================================
//	�`�揈��
//============================================================
void CRenderer::Draw(void)
{
    // �ϐ���錾
    HRESULT			hr;				// �ُ�I���̊m�F�p
    D3DVIEWPORT9	viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p
	//�J�����̃r���[�s��쐬
	D3DXVECTOR3 posR = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN).posR;
	D3DXVECTOR3 posV = CManager::GetInstance()->GetCamera()->GetCamera(CCamera::TYPE_MAIN).posV;
	D3DXVECTOR3 cVec = posR - posV;
	D3DXVec3Normalize(&cVec, &cVec);
	cVec *= 5000.0f;
	posR = cVec + posV;
	D3DXMatrixLookAtLH(&CameraView, &posV, &posR, &D3DXVECTOR3(0, 1, 0));
	m_pDepthShadow->SetCameraViewMatrix(&CameraView);//�o�^
    //--------------------------------------------------------
    //	�e�N�X�`���쐬�p�̕`��
    //--------------------------------------------------------
    // �`��T�[�t�F�C�X���쐬�������̂ɕύX
    hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderTextureSurface);
    if (FAILED(hr)) { assert(false); }

    // Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X���쐬�������̂ɕύX
    hr = m_pD3DDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
    if (FAILED(hr)) { assert(false); }

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear(0, NULL, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	if (FAILED(hr)) { assert(false); }

    // �e�N�X�`���쐬�p�̕`��
    if (SUCCEEDED(m_pD3DDevice->BeginScene()))
    { // �`��J�n�����������ꍇ

        // ���݂̃r���[�|�[�g���擾
        m_pD3DDevice->GetViewport(&viewportDef);

        // �J�����̐ݒ�
        CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	
		m_pZTex->Begin();
		CObject::DrawAll();
		m_pZTex->End();
		CObject::DrawAll();
		m_pDepthShadow->Begin();
		CObject::DrawAll();
		m_pDepthShadow->End();
        // �I�u�W�F�N�g�̑S�`��
		CObject::DrawAll();

        // �r���[�|�[�g�����ɖ߂�
        m_pD3DDevice->SetViewport(&viewportDef);

        // �`��I��
        hr = m_pD3DDevice->EndScene();
        if (FAILED(hr)) { assert(false); }
    }

    //--------------------------------------------------------
    //	��ʗp�̕`��
    //--------------------------------------------------------
    // �`��T�[�t�F�C�X�����ɖ߂�
    hr = m_pD3DDevice->SetRenderTarget(0, m_pDefRenderTextureSurface);
    if (FAILED(hr)) { assert(false); }

    // Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�����ɖ߂�
    hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefDepthStencilSurface);
    if (FAILED(hr)) { assert(false); }

	// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	hr = m_pD3DDevice->Clear(0, NULL, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	if (FAILED(hr)) { assert(false); }

    // ��ʂ̕`��
    if (SUCCEEDED(m_pD3DDevice->BeginScene()))
    { // �`��J�n�����������ꍇ

        // ���݂̃r���[�|�[�g���擾
        m_pD3DDevice->GetViewport(&viewportDef);

        // �J�����̐ݒ�
        CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// �T���v���[�X�e�[�g��ݒ�
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); // U�����̃��b�s���O�𖳌���
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP); // V�����̃��b�s���O�𖳌���
		
		// ��ʕ`��p��2D�|���S���̕`��
		m_pDrawScreen->Draw();

		D3DXMATRIX SpriteScaleMat;
		D3DXMatrixScaling(&SpriteScaleMat, 0.25f, 0.25f, 1.0f);
		m_pSprite->SetTransform(&SpriteScaleMat);
		m_pSprite->Begin(0);
		//m_pSprite->Draw(m_pZTexture, NULL, NULL, NULL, 0xffffffff);
		m_pSprite->End();
		// �f�o�b�O�\���̕`��
        CManager::GetInstance()->GetDebugProc()->Draw();

		// �T���v���[�X�e�[�g��ݒ�
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U�����̃��b�s���O��L����
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V�����̃��b�s���O��L����

        // �r���[�|�[�g�����ɖ߂�
        m_pD3DDevice->SetViewport(&viewportDef);

        // �`��I��
        hr = m_pD3DDevice->EndScene();
        if (FAILED(hr)) { assert(false); }
    }

    // �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
    hr = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
    if (FAILED(hr)) { assert(false); }
}

//============================================================
//	�����_�[�e�N�X�`���[��������
//============================================================
HRESULT CRenderer::CreateRenderTexture(void)
{
    // �ϐ���錾
    HRESULT hr;	// �ُ�I���̊m�F�p

    // �|�C���^��錾
    CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
    if (pTexture == NULL) { assert(false); return E_FAIL; }		// ��g�p��

    // ��̃e�N�X�`���𐶐�
    m_nRenderTextureID = pTexture->Regist(CTexture::SInfo
    ( // ����
        SCREEN_WIDTH,			// �e�N�X�`������
        SCREEN_HEIGHT,			// �e�N�X�`���c��
        0,						// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,	// �����E�m�ۃI�v�V����
		D3DFMT_X8R8G8B8,		// �s�N�Z���t�H�[�}�b�g
        D3DPOOL_DEFAULT			// �i�[������
    ));

    // ��ʕ`��p��2D�|���S���̐���
    if (m_pDrawScreen == NULL)
    { // ��ʕ`��p��2D�|���S������g�p���̏ꍇ

        // ��ʕ`��p��2D�|���S���̐���
        m_pDrawScreen = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
        if (m_pDrawScreen == NULL)
        { // ��ʕ`��p��2D�|���S������g�p���̏ꍇ

            // ���s��Ԃ�
            assert(false);
            return E_FAIL;
        }

        // �e�N�X�`��������
        m_pDrawScreen->BindTexture(m_nRenderTextureID);

        // �����`���OFF�ɂ���
        m_pDrawScreen->SetEnableDraw(false);
    }
    else { assert(false); return E_FAIL; }

    // �e�N�X�`���p�T�[�t�F�C�X�̐���
    hr = m_pD3DDevice->CreateDepthStencilSurface
    ( // ����
        SCREEN_WIDTH,				// �[�x�X�e���V���̃T�[�t�F�X����
        SCREEN_HEIGHT,				// �[�x�X�e���V���̃T�[�t�F�X�c��
        FORMAT_DEPTH_STENCIL,		// �[�x�X�e���V���̃T�[�t�F�X�`��
        D3DMULTISAMPLE_NONE,		// �}���`�T���v�����O�̃o�b�t�@�[�^
        0,							// �i�����x��
        FALSE,						// Z�o�b�t�@�j���̗L��/����
        &m_pDepthStencilSurface,	// Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X�ւ̃|�C���^
        NULL						// NULL
    );
    if (FAILED(hr))
    { // �`���̐����Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // �`��T�[�t�F�C�X�̎擾
    hr = pTexture->GetTexture(m_nRenderTextureID)->GetSurfaceLevel
    ( // ����
        0,							// �~�b�v�}�b�v���x��
        &m_pRenderTextureSurface	// �`��T�[�t�F�C�X�ւ̃|�C���^
    );
    if (FAILED(hr))
    { // �T�[�t�F�C�X�̎擾�Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // ���̕`��T�[�t�F�C�X��ۑ�
	hr = m_pD3DDevice->GetRenderTarget(0, &m_pDefRenderTextureSurface);
    if (FAILED(hr))
    { // �`��T�[�t�F�C�X�̎擾�Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // ����Z�o�b�t�@�E�X�e���V���o�b�t�@�̃T�[�t�F�C�X��ۑ�
	hr = m_pD3DDevice->GetDepthStencilSurface(&m_pDefDepthStencilSurface);
    if (FAILED(hr))
    { // Z�o�b�t�@�E�X�e���V���o�b�t�@�̎擾�Ɏ��s�����ꍇ

        // ���s��Ԃ�
        assert(false);
        return E_FAIL;
    }

    // ������Ԃ�
    return S_OK;
}

//============================================================
//	�f�o�C�X�擾����
//============================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) const
{
    // �f�o�C�X�̃|�C���^��Ԃ�
    return m_pD3DDevice;
}

//============================================================
//	��������
//============================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
    // �|�C���^��錾
    CRenderer *pRenderer = NULL;	// �����_���[�����p

    if (pRenderer == NULL)
    { // �g�p����Ă��Ȃ��ꍇ

        // ���������m��
        pRenderer = new CRenderer;	// �����_���[
    }
    else { assert(false); return NULL; }	// �g�p��

    if (pRenderer != NULL)
    { // �m�ۂɐ������Ă���ꍇ

        // �����_���[�̏�����
        if (FAILED(pRenderer->Init(hWnd, bWindow)))
        { // �������Ɏ��s�����ꍇ

            // �������J��
            delete pRenderer;
            pRenderer = NULL;

            // ���s��Ԃ�
            return NULL;
        }

        // �m�ۂ����A�h���X��Ԃ�
        return pRenderer;
    }
    else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CRenderer::Release(CRenderer *&prRenderer)
{
    if (prRenderer != NULL)
    { // �g�p���̏ꍇ

        // �����_���[�̏I��
        prRenderer->Uninit();

        // �������J��
        delete prRenderer;
        prRenderer = NULL;

        // ������Ԃ�
        return S_OK;
    }
    else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�f�o�C�X��������
//============================================================
HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
    // �`�揈���ƒ��_�������n�[�h�E�F�A�ōs���ݒ�ɂ���
    if (FAILED(m_pD3D->CreateDevice
    ( // ����
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &m_pD3DDevice
    )))
    { // CPU�̐��\���Ⴉ�����ꍇ

        // �`�揈���̓n�[�h�E�F�A�A���_������CPU���s���ݒ�ɂ���
        if (FAILED(m_pD3D->CreateDevice
        ( // ����
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &m_pD3DDevice
        )))
        { // �n�[�h�E�F�A�̐��\���Ⴉ�����ꍇ

            // �`�揈���ƒ��_������CPU�ōs���ݒ�ɂ���
            if (FAILED(m_pD3D->CreateDevice
            ( // ����
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF,
                hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &d3dpp,
                &m_pD3DDevice
            )))
            { // CPU�ƃn�[�h�E�F�A�̐��\���g�����ɂȂ�Ȃ������ꍇ

                // ���s��Ԃ�
                assert(false);
                return E_FAIL;
            }
        }
    }

    // ������Ԃ�
    return S_OK;
}
