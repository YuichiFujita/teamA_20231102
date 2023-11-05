//============================================================
//
//	�I�u�W�F�N�g���b�V���h�[������ [objectMeshDome.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshDome.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectMeshDome] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshDome::CObjectMeshDome()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_texPart = GRID2_ZERO;	// �e�N�X�`��������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshDome, 0, sizeof(m_meshDome));	// ���b�V���h�[���̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshDome::CObjectMeshDome(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_texPart = GRID2_ZERO;	// �e�N�X�`��������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshDome, 0, sizeof(m_meshDome));	// ���b�V���h�[���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshDome::~CObjectMeshDome()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshDome::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_texPart = GRID2_ONE;	// �e�N�X�`��������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = -1;		// �e�N�X�`���C���f�b�N�X

	m_meshDome.pos = VEC3_ZERO;		// �ʒu
	m_meshDome.rot = VEC3_ZERO;		// ����
	m_meshDome.col = XCOL_WHITE;	// �F
	m_meshDome.fRadius = 0.0f;		// ���a
	m_meshDome.cull = D3DCULL_CCW;	// �J�����O��
	m_meshDome.bLight = true;		// ���C�e�B���O��

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
void CObjectMeshDome::Uninit(void)
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

	// �I�u�W�F�N�g���b�V���h�[����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshDome::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshDome::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshDome.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshDome.bLight);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshDome.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshDome.rot.y, m_meshDome.rot.x, m_meshDome.rot.z);
	D3DXMatrixMultiply(&m_meshDome.mtxWorld, &m_meshDome.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshDome.pos.x, m_meshDome.pos.y, m_meshDome.pos.z);
	D3DXMatrixMultiply(&m_meshDome.mtxWorld, &m_meshDome.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshDome.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �����|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
		0,
		0,
		m_nNumVtx - 1,	// �g�p���钸�_��
		0,				// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		(m_nNumIdx - (m_part.x + 1)) - 3	// �v���~�e�B�u (�|���S��) ��
	);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// ��W�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	( // ����
		D3DPT_TRIANGLEFAN,	// �v���~�e�B�u�̎��
		0,
		0,
		m_part.x + 1,				// �g�p���钸�_��
		m_nNumIdx - (m_part.x + 2),	// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
		m_part.x					// �v���~�e�B�u (�|���S��) ��
	);

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectMeshDome::BindTexture(const int nTextureID)
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
void CObjectMeshDome::BindTexture(const char *pTexturePass)
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
void CObjectMeshDome::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshDome.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshDome::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshDome.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshDome::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshDome.rot = rRot;

	// �����̐��K��
	useful::NormalizeRot(m_meshDome.rot.z);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshDome::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshDome.rot;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshDome::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshDome.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshDome::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshDome.col;
}

//============================================================
//	���a�̐ݒ菈��
//============================================================
void CObjectMeshDome::SetRadius(const float fRadius)
{
	// �����̔��a��ݒ�
	m_meshDome.fRadius = fRadius;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���a�擾����
//============================================================
float CObjectMeshDome::GetRadius(void) const
{
	// ���a��Ԃ�
	return m_meshDome.fRadius;
}

//============================================================
//	��������
//============================================================
CObjectMeshDome *CObjectMeshDome::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const float fRadius,		// ���a
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectMeshDome *pObjectMeshDome = NULL;	// �I�u�W�F�N�g���b�V���h�[�������p

	if (pObjectMeshDome == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshDome = new CObjectMeshDome;	// �I�u�W�F�N�g���b�V���h�[��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectMeshDome != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���b�V���h�[���̏�����
		if (FAILED(pObjectMeshDome->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshDome;
			pObjectMeshDome = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectMeshDome->SetVec3Position(rPos);

		// ������ݒ�
		pObjectMeshDome->SetVec3Rotation(rRot);

		// �F��ݒ�
		pObjectMeshDome->SetColor(rCol);

		// ���a��ݒ�
		pObjectMeshDome->SetRadius(fRadius);

		// �J�����O��ݒ�
		pObjectMeshDome->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectMeshDome->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pObjectMeshDome->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshDome;
			pObjectMeshDome = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectMeshDome;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshDome::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshDome.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshDome::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshDome.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshDome::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshDome.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshDome::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshDome.bLight;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshDome::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = ((m_part.x + 1) * (m_part.y + 1)) - m_part.x; // �K�v���_��
	m_nNumIdx = ((m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2) - (m_part.x + 2); // �K�v�C���f�b�N�X��

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
POSGRID2 CObjectMeshDome::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObjectMeshDome::SetTexPattern(const POSGRID2& rTexPart)
{
	// �����̃e�N�X�`����������ݒ�
	m_texPart = rTexPart;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`���������擾����
//============================================================
POSGRID2 CObjectMeshDome::GetTexPattern(void) const
{
	// �e�N�X�`����������Ԃ�
	return m_texPart;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshDome::SetVtx(void)
{
	// �ϐ���錾
	D3DXVECTOR3 vecPos;					// ���_�ʒu�̌v�Z�p
	D3DXVECTOR3 vecNor;					// �@���x�N�g���̌v�Z�p
	float fRotWidth, fRotHeight;		// ���_�ʒu�̕����̌v�Z�p
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

			for (int nCntWidth = 0; nCntWidth < (m_part.x * (1 - (nCntHeight / m_part.y))) + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ� (�V�ӂ̒��_���͌J��Ԃ��� 1��ɂ���)

				// ���_�̌������v�Z
				fRotWidth  = nCntWidth  * ((D3DX_PI * 2.0f) / m_part.x);
				fRotHeight = (m_part.y - nCntHeight) * ((D3DX_PI * 0.5f) / m_part.y);

				// ���_���W�̕�����ݒ�
				vecPos = D3DXVECTOR3
				( // ����
					m_meshDome.fRadius * sinf(fRotHeight) * sinf(fRotWidth),	// x
					m_meshDome.fRadius * cosf(fRotHeight),						// y
					m_meshDome.fRadius * sinf(fRotHeight) * cosf(fRotWidth)		// z
				);

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + vecPos;

				// �@���x�N�g���̕�����ݒ�
				vecNor = vecPos;

				// �x�N�g���𐳋K��
				D3DXVec3Normalize(&vecNor, &vecNor);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = vecNor;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshDome.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fMaxTexU - (nCntWidth  * 1.0f / m_texPart.x),	// u
					fMaxTexV - (nCntHeight * 1.0f / m_texPart.y)	// v
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
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectMeshDome::SetIdx(void)
{
	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (m_pIdxBuff != NULL)
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y - 1; nCntHeight++)
		{ // �c�̕����� +1��J��Ԃ�

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ�

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 2)
			{ // ��Ԏ�O�̕����ꏊ�ł͂Ȃ��ꍇ

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// �C���f�b�N�X�f�[�^�̃|�C���^�� 2���i�߂�
				pIdx += 2;
			}
		}

		// �����̓V�ӂ̒��_���w��
		pIdx[0] = (WORD)(m_nNumVtx - 1);

		// �C���f�b�N�X�f�[�^�̃|�C���^�� 1���i�߂�
		pIdx += 1;

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // ���̕����� +1��J��Ԃ�

			pIdx[0] = (WORD)(((m_part.x + 1) * (m_part.y - 1)) + nCntWidth);

			// �C���f�b�N�X�f�[�^�̃|�C���^�� 1���i�߂�
			pIdx += 1;
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	�X�N���[���̃e�N�X�`�����W�̐ݒ菈��
//============================================================
void CObjectMeshDome::SetScrollTex(const float fTexU, const float fTexV)
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

			for (int nCntWidth = 0; nCntWidth < (m_part.x * (1 - (nCntHeight / m_part.y))) + 1; nCntWidth++)
			{ // ���̕����� +1��J��Ԃ� (�V�ӂ̒��_���͌J��Ԃ��� 1��ɂ���)

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				( // ����
					fMaxTexU - fTexU - (nCntWidth  * 1.0f / m_texPart.x),	// u
					fMaxTexV - fTexV - (nCntHeight * 1.0f / m_texPart.y)	// v
				);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

// TODO�F�e�N�X�`�����W�����������
