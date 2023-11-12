//============================================================
//
//	�I�u�W�F�N�g�������� [objectTornado.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectTornado.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_OFFSET	(2)	// �I�t�Z�b�g�̐�

//************************************************************
//	�q�N���X [CObjectTornado] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectTornado::CObjectTornado()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
	memset(&m_tornado, 0, sizeof(m_tornado));	// �����̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectTornado::CObjectTornado(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
	m_nTextureID = 0;	// �e�N�X�`���C���f�b�N�X
	memset(&m_tornado, 0, sizeof(m_tornado));	// �����̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectTornado::~CObjectTornado()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectTornado::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nNumAround = 0;	// �Q�̎���
	m_nPattern = 0;		// �Q�̕�����
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	// �����̏���������
	m_tornado.pos     = VEC3_ZERO;		// �ʒu
	m_tornado.rot     = VEC3_ZERO;		// ����
	m_tornado.direRot = VEC3_ZERO;		// ��������
	m_tornado.col     = XCOL_WHITE;		// �F
	m_tornado.pMtxParent  = NULL;		// �e�̃}�g���b�N�X
	m_tornado.fMoveRot    = 0.0f;		// �����̕ύX��
	m_tornado.fThickness  = 0.0f;		// �|���S���̑���
	m_tornado.fOuterPlus  = 0.0f;		// �|���S���O���� y���W���Z��
	m_tornado.fSetWidth   = 0.0f;		// �������̉������
	m_tornado.fSetAlpha   = 0.0f;		// �������̓����x
	m_tornado.fAddWidth   = 0.0f;		// ������̉��Z��
	m_tornado.fAddHeight  = 0.0f;		// �c����̉��Z��
	m_tornado.fSubAlpha   = 0.0f;		// �����x�̌��Z��
	m_tornado.fGrowWidth  = 0.0f;		// ������̐�����
	m_tornado.fGrowHeight = 0.0f;		// �c����̐�����
	m_tornado.fGrowAlpha  = 0.0f;		// �����x�̐�����
	m_tornado.cull   = D3DCULL_NONE;	// �J�����O��
	m_tornado.bLight = false;			// ���C�e�B���O��

	// �Q��ݒ�
	if (FAILED(SetVortex(1, 1)))
	{ // �Q�̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectTornado::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g������j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectTornado::Update(void)
{
	// �����̌�����ύX
	m_tornado.rot.y -= m_tornado.fMoveRot;

	// �����𐳋K��
	useful::NormalizeRot(m_tornado.rot.y);

	// �����𐬒�������
	m_tornado.fSetWidth  += m_tornado.fGrowWidth * m_nNumVtx;	// ���_����̉�����ʂ����Z
	m_tornado.fAddWidth  += m_tornado.fGrowWidth;	// �����̉��̍L��������Z
	m_tornado.fAddHeight += m_tornado.fGrowHeight;	// �����̏c�̍L��������Z
	m_tornado.fSetAlpha  -= m_tornado.fGrowAlpha;	// �����̓����x�����Z (�����ɂ��Ă���)

	// ���_���̐ݒ�
	SetVtx();

	if (m_tornado.fSetAlpha <= 0.0f)
	{ // �����������ɂȂ����ꍇ

		// �����I�u�W�F�N�g�̏I��
		Uninit();

		// �����𔲂���
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectTornado::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxOrigin;			// �������̃}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̕\����Ԃ�ݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, m_tornado.cull);

	// ���C�e�B���O��ݒ�
	pDevice->SetRenderState(D3DRS_LIGHTING, m_tornado.bLight);

	// ���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// ���e�X�g�̐ݒ�

	//--------------------------------------------------------
	//	�������̃}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// �������̃}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxOrigin);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.direRot.y, m_tornado.direRot.x, m_tornado.direRot.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxRot);	// ��������

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_tornado.pos.x, m_tornado.pos.y, m_tornado.pos.z);
	D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, &mtxTrans);	// �����ʒu

	//--------------------------------------------------------
	//	���[���h�}�g���b�N�X�����߂�
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_tornado.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_tornado.rot.y, m_tornado.rot.x, m_tornado.rot.z);
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxRot);	// ��]��

	//--------------------------------------------------------
	//	�}�g���b�N�X���|�����킹��
	//--------------------------------------------------------
	if (m_tornado.pMtxParent != NULL)	// TODO�F�e�}�g���b�N�X��NULL����Ȃ��Ȃ����Ƃ��ɔ��f�ł��Ȃ�
	{ // �e�̃}�g���b�N�X�����݂���ꍇ

		// �e�̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&mtxOrigin, &mtxOrigin, m_tornado.pMtxParent);
	}

	// �������̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_tornado.mtxWorld, &m_tornado.mtxWorld, &mtxOrigin);

	//--------------------------------------------------------
	//	������`��
	//--------------------------------------------------------
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_tornado.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// ���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// ���e�X�g�̗L�� / �����̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// ���e�X�g�̐ݒ�
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectTornado::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_tornado.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectTornado::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_tornado.pos;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectTornado::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_tornado.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectTornado::GetColor(void) const
{
	// �F��Ԃ�
	return m_tornado.col;
}

//============================================================
//	��������
//============================================================
CObjectTornado *CObjectTornado::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rDireRot,	// ��������
	const D3DXCOLOR& rCol,			// �F
	D3DXMATRIX *pMtxParent,			// �e�̃}�g���b�N�X
	const int   nNumAround,			// �Q�̎���
	const int   nPattern,			// �Q�̕�����
	const float fMoveRot,			// �����̕ύX��
	const float fThickness,			// �|���S���̑���
	const float fOuterPlus,			// �|���S���O���� y���W���Z��
	const float fSetWidth,			// �������̉������
	const float fSetAlpha,			// �������̓����x
	const float fAddWidth,			// ������̉��Z��
	const float fAddHeight,			// �c����̉��Z��
	const float fSubAlpha,			// �����x�̌��Z��
	const float fGrowWidth,			// ������̐�����
	const float fGrowHeight,		// �c����̐�����
	const float fGrowAlpha,			// �����x�̐�����
	const D3DCULL cull,				// �J�����O��
	const bool bLight				// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectTornado *pObjectTornado = NULL;	// �I�u�W�F�N�g���������p

	if (pObjectTornado == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectTornado = new CObjectTornado;	// �I�u�W�F�N�g����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectTornado != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�����̏�����
		if (FAILED(pObjectTornado->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectTornado;
			pObjectTornado = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectTornado->SetVec3Position(rPos);

		// ������ݒ�
		pObjectTornado->SetDirectionRotation(rDireRot);

		// �F��ݒ�
		pObjectTornado->SetColor(rCol);

		// �e�̃}�g���b�N�X��ݒ�
		pObjectTornado->SetMatrixParent(pMtxParent);

		// �|���S���̑�����ݒ�
		pObjectTornado->SetThickness(fThickness);

		// �|���S���O����y���W���Z�ʂ�ݒ�
		pObjectTornado->SetOuterPlus(fOuterPlus);

		// �������̉�����ʂ�ݒ�
		pObjectTornado->SetWidth(fSetWidth);

		// �������̓����x��ݒ�
		pObjectTornado->SetAlpha(fSetAlpha);

		// ������̉��Z�ʂ�ݒ�
		pObjectTornado->SetAddWidth(fAddWidth);

		// �c����̉��Z�ʂ�ݒ�
		pObjectTornado->SetAddHeight(fAddHeight);

		// �����x�̌��Z�ʂ�ݒ�
		pObjectTornado->SetSubAlpha(fSubAlpha);

		// �J�����O��ݒ�
		pObjectTornado->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectTornado->SetLighting(bLight);

		// ��������ݒ�
		pObjectTornado->SetGrow(fMoveRot, fGrowWidth, fGrowHeight, fGrowAlpha);

		// �Q��ݒ�
		if (FAILED(pObjectTornado->SetVortex(nNumAround, nPattern)))
		{ // �Q�̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectTornado;
			pObjectTornado = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectTornado;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectTornado::SetGrow(float fMoveRot, float fGrowWidth, float fGrowHeight, float fGrowAlpha)
{
	// �����̏���ݒ�
	m_tornado.fMoveRot		= fMoveRot;		// �����̕ύX��
	m_tornado.fGrowWidth	= fGrowWidth;	// ������̐�����
	m_tornado.fGrowHeight	= fGrowHeight;	// �c����̐�����
	m_tornado.fGrowAlpha	= fGrowAlpha;	// �����x�̐�����
}

//============================================================
//	�Q�̐ݒ菈��
//============================================================
HRESULT CObjectTornado::SetVortex(const int nNumAround, const int nPattern)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̉Q��ݒ�
	m_nNumAround = nNumAround;
	m_nPattern = nPattern;

	// �K�v���_�������߂�
	m_nNumVtx = m_nNumAround * m_nPattern * MAX_OFFSET;

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���_�o�b�t�@�̏���ݒ�
	if (m_pVtxBuff == NULL)
	{ // ��g�p���̏ꍇ

		// ���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer
		( // ����
			sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
			D3DUSAGE_WRITEONLY,	// �g�p���@
			FVF_VERTEX_3D,		// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,	// �������̎w��
			&m_pVtxBuff,		// ���_�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�}�g���b�N�X�폜����
//============================================================
void CObjectTornado::DeleteMatrixParent(void)
{
	// �e�}�g���b�N�X��NULL�ɂ���
	m_tornado.pMtxParent = NULL;
}

//============================================================
//	�e�̃}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectTornado::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// �����̐e�}�g���b�N�X��ݒ�
	m_tornado.pMtxParent = pMtxParent;
}

//============================================================
//	���������̐ݒ菈��
//============================================================
void CObjectTornado::SetDirectionRotation(const D3DXVECTOR3& rRot)
{
	// �����̐���������ݒ�
	m_tornado.direRot = rRot;

	// ���������̐��K��
	useful::Vec3NormalizeRot(m_tornado.direRot);
}

//============================================================
//	���������擾����
//============================================================
D3DXVECTOR3 CObjectTornado::GetDirectionRotation(void) const
{
	// ����������Ԃ�
	return m_tornado.direRot;
}

//============================================================
//	�|���S���̑����̐ݒ菈��
//============================================================
void CObjectTornado::SetThickness(const float fThickness)
{
	// �����̃|���S���̑�����ݒ�
	m_tornado.fThickness = fThickness;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���̑����擾����
//============================================================
float CObjectTornado::GetThickness(void) const
{
	// �|���S���̑�����Ԃ�
	return m_tornado.fThickness;
}

//============================================================
//	�|���S���O����y���W���Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetOuterPlus(const float fOuterPlus)
{
	// �����̃|���S���O����y���W���Z�ʂ�ݒ�
	m_tornado.fOuterPlus = fOuterPlus;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�|���S���O����y���W���Z�ʂ̎擾����
//============================================================
float CObjectTornado::GetOuterPlus(void) const
{
	// �|���S���O����y���W���Z�ʂ�Ԃ�
	return m_tornado.fOuterPlus;
}

//============================================================
//	�������̉�����ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetWidth(const float fSetWidth)
{
	// �����̐������̉�����ʂ�ݒ�
	m_tornado.fSetWidth = fSetWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̉�����ʎ擾����
//============================================================
float CObjectTornado::GetWidth(void) const
{
	// �������̉�����ʂ�Ԃ�
	return m_tornado.fSetWidth;
}

//============================================================
//	�������̓����x�̐ݒ菈��
//============================================================
void CObjectTornado::SetAlpha(const float fSetAlpha)
{
	// �����̐������̓����x��ݒ�
	m_tornado.fSetAlpha = fSetAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�������̓����x�擾����
//============================================================
float CObjectTornado::GetAlpha(void) const
{
	// �������̓����x��Ԃ�
	return m_tornado.fSetAlpha;
}

//============================================================
//	������̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetAddWidth(const float fAddWidth)
{
	// �����̉�����̉��Z�ʂ�ݒ�
	m_tornado.fAddWidth = fAddWidth;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	������̉��Z�ʎ擾����
//============================================================
float CObjectTornado::GetAddWidth(void) const
{
	// ������̉��Z�ʂ�Ԃ�
	return m_tornado.fAddWidth;
}

//============================================================
//	�c����̉��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetAddHeight(const float fAddHeight)
{
	// �����̏c����̉��Z�ʂ�ݒ�
	m_tornado.fAddHeight = fAddHeight;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�c����̉��Z�ʎ擾����
//============================================================
float CObjectTornado::GetAddHeight(void) const
{
	// �c����̉��Z�ʂ�Ԃ�
	return m_tornado.fAddHeight;
}

//============================================================
//	�����x�̌��Z�ʂ̐ݒ菈��
//============================================================
void CObjectTornado::SetSubAlpha(const float fSubAlpha)
{
	// �����̓����x�̌��Z�ʂ�ݒ�
	m_tornado.fSubAlpha = fSubAlpha;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����x�̌��Z�ʎ擾����
//============================================================
float CObjectTornado::GetSubAlpha(void) const
{
	// �����x�̌��Z�ʂ�Ԃ�
	return m_tornado.fSubAlpha;
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectTornado::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_tornado.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectTornado::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_tornado.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectTornado::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_tornado.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectTornado::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_tornado.bLight;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectTornado::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;	// �����̒��_�����x�N�g��
	float fWidth  = m_tornado.fSetWidth;	// ���_�����̉������
	float fHeight = 0.0f;					// ���_�����̏c�����
	float fAlpha  = m_tornado.fSetAlpha;	// ���_�J���[�̓����x

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �����̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < m_nNumAround; nCntWidth++)
		{ // �Q�̎���̑������J��Ԃ�

			for (int nCntHeight = 0; nCntHeight < m_nPattern; nCntHeight++)
			{ // �Q�̕��������J��Ԃ�

				// ���_�����x�N�g�������߂�
				vecPos.x = sinf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;
				vecPos.y = 1.0f;
				vecPos.z = cosf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern)) * 1.0f;

				for (int nCntSet = 0; nCntSet < MAX_OFFSET; nCntSet++)
				{ // �I�t�Z�b�g�̑������J��Ԃ�

					// ���_���W�̐ݒ�
					pVtx[0].pos.x = 0.0f + vecPos.x * (fWidth + (nCntSet * m_tornado.fThickness));	// x
					pVtx[0].pos.y = 0.0f + vecPos.y * (fHeight + (nCntSet * m_tornado.fOuterPlus));	// y
					pVtx[0].pos.z = 0.0f + vecPos.z * (fWidth + (nCntSet * m_tornado.fThickness));	// z

					// �@���x�N�g���̐ݒ�
					pVtx[0].nor = VEC3_ZERO;

					if (fAlpha < 0.0f)
					{ // �����x���͈͊O�̏ꍇ

						// �����x��␳
						fAlpha = 0.0f;
					}

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(m_tornado.col.r, m_tornado.col.g, m_tornado.col.b, fAlpha);

					// �e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2
					( // ����
						1.0f,			// u
						1.0f * nCntSet	// v
					);

					// ���_�f�[�^�̃|�C���^��i�߂�
					pVtx += 1;
				}

				// �c�����L����
				fWidth  += m_tornado.fAddWidth;
				fHeight += m_tornado.fAddHeight;

				// ���l�����Z
				fAlpha -= m_tornado.fSubAlpha;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�j������
//============================================================
void CObjectTornado::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
