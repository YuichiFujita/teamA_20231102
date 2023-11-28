//============================================================
//
//	�I�u�W�F�N�g���b�V���E�H�[������ [objectMeshWall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshWall.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CObjectMeshWall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshWall::CObjectMeshWall()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshWall, 0, sizeof(m_meshWall));	// ���b�V���E�H�[���̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshWall::CObjectMeshWall(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshWall, 0, sizeof(m_meshWall));	// ���b�V���E�H�[���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshWall::~CObjectMeshWall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshWall::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = -1;		// �e�N�X�`���C���f�b�N�X

	m_meshWall.pos = VEC3_ZERO;		// �ʒu
	m_meshWall.rot = VEC3_ZERO;		// ����
	m_meshWall.size = VEC2_ZERO;	// �傫��
	m_meshWall.col = XCOL_WHITE;	// �F
	m_meshWall.cull = D3DCULL_CCW;	// �J�����O��
	m_meshWall.bLight = true;		// ���C�e�B���O��

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
void CObjectMeshWall::Uninit(void)
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

	// �I�u�W�F�N�g���b�V���E�H�[����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshWall::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshWall::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshWall.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshWall.bLight);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshWall.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshWall.rot.y, m_meshWall.rot.x, m_meshWall.rot.z);
	D3DXMatrixMultiply(&m_meshWall.mtxWorld, &m_meshWall.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshWall.pos.x, m_meshWall.pos.y, m_meshWall.pos.z);
	D3DXMatrixMultiply(&m_meshWall.mtxWorld, &m_meshWall.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshWall.mtxWorld);

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
void CObjectMeshWall::BindTexture(const int nTextureID)
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
void CObjectMeshWall::BindTexture(const char *pTexturePass)
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
void CObjectMeshWall::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshWall.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshWall::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshWall.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshWall::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshWall.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshWall.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshWall::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshWall.rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectMeshWall::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// �����̑傫����ݒ�
	m_meshWall.size = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR2 CObjectMeshWall::GetVec2Sizing(void) const
{
	// �傫����Ԃ�
	return m_meshWall.size;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshWall::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshWall.col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshWall::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshWall.col;
}

//============================================================
//	��������
//============================================================
CObjectMeshWall *CObjectMeshWall::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR2& rSize,	// �傫��
	const D3DXCOLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const D3DCULL cull,			// �J�����O��
	const bool bLight			// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectMeshWall *pObjectMeshWall = NULL;		// �I�u�W�F�N�g���b�V���E�H�[�������p

	if (pObjectMeshWall == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshWall = new CObjectMeshWall;	// �I�u�W�F�N�g���b�V���E�H�[��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectMeshWall != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���b�V���E�H�[���̏�����
		if (FAILED(pObjectMeshWall->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshWall;
			pObjectMeshWall = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectMeshWall->SetVec3Position(rPos);

		// ������ݒ�
		pObjectMeshWall->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObjectMeshWall->SetVec2Sizing(rSize);

		// �F��ݒ�
		pObjectMeshWall->SetColor(rCol);

		// �J�����O��ݒ�
		pObjectMeshWall->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectMeshWall->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pObjectMeshWall->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshWall;
			pObjectMeshWall = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectMeshWall;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshWall::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshWall.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshWall::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshWall.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshWall::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshWall.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshWall::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshWall.bLight;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshWall::SetPattern(const POSGRID2& rPart)
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
POSGRID2 CObjectMeshWall::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshWall::SetVtx(void)
{
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

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				( // ����
					nCntWidth * (m_meshWall.size.x / (float)m_part.x) - (m_meshWall.size.x * 0.5f),	// x
					-(nCntHeight * (m_meshWall.size.y / (float)m_part.y)) + m_meshWall.size.y,		// y
					0.0f																			// z
				);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshWall.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

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
void CObjectMeshWall::SetIdx(void)
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
void CObjectMeshWall::SetScrollTex(const float fTexU, const float fTexV)
{
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
				pVtx[0].tex = D3DXVECTOR2((float)nCntWidth + fTexU, (float)nCntHeight + fTexV);

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
void CObjectMeshWall::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
