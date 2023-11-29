//============================================================
//
//	�I�u�W�F�N�g���b�V���V�����_�[���� [objectMeshCylinder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectMeshCylinder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshCylinder::CObjectMeshCylinder()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshCylinder, 0, sizeof(m_meshCylinder));	// ���b�V���V�����_�[�̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshCylinder::CObjectMeshCylinder(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshCylinder, 0, sizeof(m_meshCylinder));	// ���b�V���V�����_�[�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshCylinder::~CObjectMeshCylinder()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshCylinder::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = -1;		// �e�N�X�`���C���f�b�N�X

	m_meshCylinder.pos = VEC3_ZERO;		// �ʒu
	m_meshCylinder.rot = VEC3_ZERO;		// ����
	m_meshCylinder.col = XCOL_WHITE;	// �F
	m_meshCylinder.fRadius = 0.0f;		// ���a
	m_meshCylinder.fHeight = 0.0f;		// �c��
	m_meshCylinder.cull = D3DCULL_CCW;	// �J�����O��
	m_meshCylinder.bLight = true;		// ���C�e�B���O��

	// ��������ݒ�
	if (FAILED(SetPattern(GRID2_ONE)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectMeshCylinder::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{ // �C���f�b�N�X�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g���b�V���V�����_�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshCylinder::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshCylinder::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshCylinder.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshCylinder.bLight);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshCylinder.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshCylinder.rot.y, m_meshCylinder.rot.x, m_meshCylinder.rot.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshCylinder.pos.x, m_meshCylinder.pos.y, m_meshCylinder.pos.z);
	D3DXMatrixMultiply(&m_meshCylinder.mtxWorld, &m_meshCylinder.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCylinder.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx,		// �g�p���钸�_��
		0,				// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		m_nNumIdx - 2	// �v���~�e�B�u (�|���S��) ��
	);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectMeshCylinder::BindTexture(const int nTextureID)
{
	if (nTextureID > NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObjectMeshCylinder::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	if (pTexture == NULL)
	{ // �e�N�X�`���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (pTexturePass != NULL)
	{ // ���蓖�Ă�e�N�X�`���p�X�����݂���ꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else { assert(false); }	// �e�N�X�`���p�X����
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshCylinder.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshCylinder::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshCylinder.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshCylinder.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshCylinder.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshCylinder::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshCylinder.rot;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshCylinder.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshCylinder::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshCylinder.col;
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetRadius(const float fRadius)
{
	// �����̔��a��ݒ�
	m_meshCylinder.fRadius = fRadius;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���a�擾����
//============================================================
float CObjectMeshCylinder::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_meshCylinder.fRadius;
}

//============================================================
//	�c���̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetHeight(const float fHeight)
{
	// �����̏c����ݒ�
	m_meshCylinder.fHeight = fHeight;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�c���擾����
//============================================================
float CObjectMeshCylinder::GetHeight(void) const
{
	// �c����Ԃ�
	return m_meshCylinder.fHeight;
}

//============================================================
//	��������
//============================================================
CObjectMeshCylinder *CObjectMeshCylinder::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const float fRadius,		// ���a
	const float fHeight,		// �c��
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectMeshCylinder *pObjectMeshCylinder = NULL;	// �I�u�W�F�N�g���b�V���V�����_�[�����p

	if (pObjectMeshCylinder == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshCylinder = new CObjectMeshCylinder;	// �I�u�W�F�N�g���b�V���V�����_�[
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectMeshCylinder != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���b�V���V�����_�[�̏�����
		if (FAILED(pObjectMeshCylinder->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshCylinder;
			pObjectMeshCylinder = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectMeshCylinder->SetVec3Position(rPos);

		// ������ݒ�
		pObjectMeshCylinder->SetVec3Rotation(rRot);

		// �F��ݒ�
		pObjectMeshCylinder->SetColor(rCol);

		// ���a��ݒ�
		pObjectMeshCylinder->SetRadius(fRadius);

		// �c����ݒ�
		pObjectMeshCylinder->SetHeight(fHeight);

		// �J�����O��ݒ�
		pObjectMeshCylinder->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectMeshCylinder->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pObjectMeshCylinder->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshCylinder;
			pObjectMeshCylinder = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectMeshCylinder;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshCylinder::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshCylinder.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshCylinder::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshCylinder.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshCylinder::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshCylinder.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshCylinder::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshCylinder.bLight;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshCylinder::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // �K�v���_��
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // �K�v�C���f�b�N�X��

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

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{ // �C���f�b�N�X�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̏���ݒ�
	if (m_pIdxBuff == NULL)
	{ // ��g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�̐���
		if (FAILED(pDevice->CreateIndexBuffer
		( // ����
			sizeof(WORD) * m_nNumIdx,	// �K�v�C���f�b�N�X��
			D3DUSAGE_WRITEONLY,	// �g�p���@
			D3DFMT_INDEX16,		// �C���f�b�N�X�o�b�t�@�̃t�H�[�}�b�g
			D3DPOOL_MANAGED,	// �������̎w��
			&m_pIdxBuff,		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // �C���f�b�N�X�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx();
	SetIdx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�������擾����
//============================================================
POSGRID2 CObjectMeshCylinder::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;	// ���_�ʒu�̌v�Z�p
	D3DXVECTOR3 vecNor;	// �@���x�N�g���̌v�Z�p
	float fMaxTexU = m_part.x * 1.0f;	// �e�N�X�`�����W�� u�ő�l
	float fMaxTexV = m_part.y * 1.0f;	// �e�N�X�`�����W�� v�ő�l

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// ���_���W�̕�����ݒ�
				vecPos = D3DXVECTOR3
				( // ����
					sinf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * m_meshCylinder.fRadius,	// x
					(nCntHeight * (m_meshCylinder.fHeight / (float)m_part.y)) - m_meshCylinder.pos.y,	// y
					cosf(D3DXToRadian(nCntWidth * (360 / (float)m_part.x))) * m_meshCylinder.fRadius	// z
				);

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + vecPos;

				// �@���x�N�g���̕�����ݒ�
				vecNor = D3DXVECTOR3(vecPos.x, 0.0f, vecPos.z);

				// �x�N�g���𐳋K��
				D3DXVec3Normalize(&vecNor, &vecNor);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = vecNor;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshCylinder.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(fMaxTexU - (1.0f * nCntWidth), fMaxTexV - (1.0f * nCntHeight));

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetIdx(void)
{
	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (m_pIdxBuff != NULL)
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	�X�N���[���̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObjectMeshCylinder::SetScrollTex(const float fTexU, const float fTexV)
{
	// �ϐ���錾
	float fMaxTexU = m_part.x * 1.0f;	// �e�N�X�`�����W�� u�ő�l
	float fMaxTexV = m_part.y * 1.0f;	// �e�N�X�`�����W�� v�ő�l

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fMaxTexU - fTexU - (1.0f * nCntWidth),	// u
					fMaxTexV - fTexV - (1.0f * nCntHeight)	// v
				);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	�j������
//============================================================
void CObjectMeshCylinder::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
