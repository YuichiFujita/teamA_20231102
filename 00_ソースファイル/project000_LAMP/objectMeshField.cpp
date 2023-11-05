//============================================================
//
//	�I�u�W�F�N�g���b�V���t�B�[���h���� [objectMeshField.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
#define NUM_VTX_TRIANGLE	(3)	// �O�p�`�|���S���̒��_��
#define COLL_RADIUS	(100.0f)	// �����蔻����s������

//************************************************************
//	�q�N���X [CObjectMeshField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshField::CObjectMeshField()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_pPosGapBuff = NULL;	// ���W�̂���o�b�t�@
	m_pNorBuff = NULL;		// �@���o�b�t�@
	m_pNumNorBuff = NULL;	// �@���̎g�p���o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshField, 0, sizeof(m_meshField));	// ���b�V���t�B�[���h�̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshField::CObjectMeshField(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_pIdxBuff = NULL;		// �C���f�b�N�X�o�b�t�@
	m_pPosGapBuff = NULL;	// ���W�̂���o�b�t�@
	m_pNorBuff = NULL;		// �@���o�b�t�@
	m_pNumNorBuff = NULL;	// �@���̎g�p���o�b�t�@
	m_part = GRID2_ZERO;	// ������
	m_nNumVtx = 0;			// �K�v���_��
	m_nNumIdx = 0;			// �K�v�C���f�b�N�X��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_meshField, 0, sizeof(m_meshField));	// ���b�V���t�B�[���h�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshField::~CObjectMeshField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshField::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;			// ���_�o�b�t�@
	m_pIdxBuff = NULL;			// �C���f�b�N�X�o�b�t�@
	m_pPosGapBuff = NULL;		// ���W�̂���o�b�t�@
	m_pNorBuff = NULL;			// �@���o�b�t�@
	m_pNumNorBuff = NULL;		// �@���̎g�p���o�b�t�@
	m_part = GRID2_ZERO;		// ������
	m_nNumVtx = 0;				// �K�v���_��
	m_nNumIdx = 0;				// �K�v�C���f�b�N�X��
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	m_meshField.pos = VEC3_ZERO;	// �ʒu
	m_meshField.rot = VEC3_ZERO;	// ����
	m_meshField.size = VEC2_ZERO;	// �傫��
	m_meshField.col = XCOL_WHITE;	// �F
	m_meshField.cull = D3DCULL_CCW;	// �J�����O��
	m_meshField.bLight = true;		// ���C�e�B���O��

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
void CObjectMeshField::Uninit(void)
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

	// ���W�̂���o�b�t�@�̔j��
	if (m_pPosGapBuff != NULL)
	{ // ���W�̂���o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pPosGapBuff;
		m_pPosGapBuff = NULL;
	}

	// �@���o�b�t�@�̔j��
	if (m_pNorBuff != NULL)
	{ // �@���o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pNorBuff;
		m_pNorBuff = NULL;
	}

	// �@���̎g�p���o�b�t�@�̔j��
	if (m_pNumNorBuff != NULL)
	{ // �C���f�b�N�X�o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pNumNorBuff;
		m_pNumNorBuff = NULL;
	}

	// �I�u�W�F�N�g���b�V���t�B�[���h��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshField::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshField::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshField.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshField.bLight);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshField.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
	D3DXMatrixMultiply(&m_meshField.mtxWorld, &m_meshField.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshField.mtxWorld);

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
void CObjectMeshField::BindTexture(const int nTextureID)
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
void CObjectMeshField::BindTexture(const char *pTexturePass)
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
void CObjectMeshField::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshField.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshField::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshField.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshField::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshField.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshField.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshField::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshField.rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectMeshField::SetVec2Sizing(const D3DXVECTOR2& rSize)
{
	// �����̑傫����ݒ�
	m_meshField.size = rSize;

	// ���_���̐ݒ�
	SetVtx(false);
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR2 CObjectMeshField::GetVec2Sizing(void) const
{
	// �傫����Ԃ�
	return m_meshField.size;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectMeshField::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F��ݒ�
	m_meshField.col = rCol;

	// ���_���̐ݒ�
	SetVtx(false);
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectMeshField::GetColor(void) const
{
	// �F��Ԃ�
	return m_meshField.col;
}

//============================================================
//	��������
//============================================================
CObjectMeshField *CObjectMeshField::Create
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
	CObjectMeshField *pObjectMeshField = NULL;		// �I�u�W�F�N�g���b�V���t�B�[���h�����p

	if (pObjectMeshField == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshField = new CObjectMeshField;	// �I�u�W�F�N�g���b�V���t�B�[���h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectMeshField != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���b�V���t�B�[���h�̏�����
		if (FAILED(pObjectMeshField->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pObjectMeshField->SetVec3Position(rPos);

		// ������ݒ�
		pObjectMeshField->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObjectMeshField->SetVec2Sizing(rSize);

		// �F��ݒ�
		pObjectMeshField->SetColor(rCol);

		// �J�����O��ݒ�
		pObjectMeshField->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectMeshField->SetLighting(bLight);

		// ��������ݒ�
		if (FAILED(pObjectMeshField->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshField;
			pObjectMeshField = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectMeshField;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshField::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshField.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshField::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshField.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshField::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshField.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshField::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshField.bLight;
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CObjectMeshField::SetPattern(const POSGRID2& rPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	//--------------------------------------------------------
	//	�ύX����ݒ�
	//--------------------------------------------------------
	// �����̕�������ݒ�
	m_part = rPart;

	// �K�v���_�E�C���f�b�N�X�������߂�
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // �K�v���_��
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // �K�v�C���f�b�N�X��

	//--------------------------------------------------------
	//	���_�o�b�t�@�̔j���E����
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	�C���f�b�N�X�o�b�t�@�̔j���E����
	//--------------------------------------------------------
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

	//--------------------------------------------------------
	//	���W�̂���o�b�t�@�̔j���E����
	//--------------------------------------------------------
	// ���W�̂���o�b�t�@�̔j��
	if (m_pPosGapBuff != NULL)
	{ // ���W�̂���o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pPosGapBuff;
		m_pPosGapBuff = NULL;
	}

	// ���W�̂���o�b�t�@�̏���ݒ�
	if (m_pPosGapBuff == NULL)
	{ // ��g�p���̏ꍇ

		// ���W�̂���o�b�t�@�̃������m��
		m_pPosGapBuff = new D3DXVECTOR3[m_nNumVtx];

		if (m_pPosGapBuff != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pPosGapBuff, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�@���o�b�t�@�̔j���E����
	//--------------------------------------------------------
	// �@���o�b�t�@�̔j��
	if (m_pNorBuff != NULL)
	{ // �@���o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pNorBuff;
		m_pNorBuff = NULL;
	}

	// �@���o�b�t�@�̏���ݒ�
	if (m_pNorBuff == NULL)
	{ // ��g�p���̏ꍇ

		// �ϐ���錾
		int nNorBuff = 6 + (6 * (m_part.x - 1)) + (6 * (m_part.y - 1)) + (6 * ((m_part.x - 1) * (m_part.y - 1)));	// �@���o�b�t�@�̊m�ې�

		// �@���o�b�t�@�̃������m��
		m_pNorBuff = new D3DXVECTOR3[nNorBuff];

		if (m_pNorBuff != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pNorBuff, 0, sizeof(D3DXVECTOR3) * nNorBuff);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�@���̎g�p���o�b�t�@�̔j���E����
	//--------------------------------------------------------
	// �@���̎g�p���o�b�t�@�̔j��
	if (m_pNumNorBuff != NULL)
	{ // �@���̎g�p���o�b�t�@���g�p���̏ꍇ

		// �������J��
		delete[] m_pNumNorBuff;
		m_pNumNorBuff = NULL;
	}

	// �@���̎g�p���o�b�t�@�̏���ݒ�
	if (m_pNumNorBuff == NULL)
	{ // ��g�p���̏ꍇ

		// �@���̎g�p���o�b�t�@�̃������m��
		m_pNumNorBuff = new int[m_nNumVtx];

		if (m_pNumNorBuff != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pNumNorBuff, 0, sizeof(int) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�������𔽉f
	//--------------------------------------------------------
	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx(true);
	SetIdx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�������擾����
//============================================================
POSGRID2 CObjectMeshField::GetPattern(void) const
{
	// ��������Ԃ�
	return m_part;
}

//============================================================
//	���W�̂���̐ݒ菈��
//============================================================
void CObjectMeshField::SetGapPosition(const int nID, const D3DXVECTOR3& rPos)
{
	if (m_pPosGapBuff != NULL)
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�̂����ݒ�
			m_pPosGapBuff[nID] = rPos;
		}
		else
		{ // �C���f�b�N�X���g�p�s�ȏꍇ

			// ��O����
			assert(false);
		}
	}
}

//============================================================
//	���W�̂���擾����
//============================================================
D3DXVECTOR3 CObjectMeshField::GetGapPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos = VEC3_ZERO;	// ���_�̂���̑���p

	if (m_pPosGapBuff != NULL)
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�̂����ݒ�
			pos = m_pPosGapBuff[nID];
		}
		else
		{ // �C���f�b�N�X���g�p�s�ȏꍇ

			// ��O����
			assert(false);
		}
	}

	// �����̃C���f�b�N�X�̒��_�̂����Ԃ�
	return pos;
}

//============================================================
//	�@���̐��K������
//============================================================
void CObjectMeshField::NormalizeNormal(void)
{
	// �ϐ���錾
	int nNumNor = 0;	// �@���f�[�^�i�[�p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	//--------------------------------------------------------
	//	�@���̐ݒ�
	//--------------------------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
	{ // �c�̕����� +1��J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // ���̕����� +1��J��Ԃ�

			// �ϐ���錾
			int nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));	// ���݂̒��_�ԍ�

			if (nNumVtx == 0)
			{ // ���_�ԍ�������̏ꍇ

				// �@���̎g�p���� 2�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 2;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);

				// �@���f�[�^�� 2���i�߂�
				nNumNor += 2;
			}
			else if (nNumVtx == m_nNumVtx - 1)
			{ // ���_�ԍ����E���̏ꍇ

				// �@���̎g�p���� 2�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 2;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftBottom(pVtx);

				// �@���f�[�^�� 2���i�߂�
				nNumNor += 2;
			}
			else if (nNumVtx == m_part.x)
			{ // ���_�ԍ����E��̏ꍇ

				// �@���̎g�p���� 1�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 1;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);

				// �@���f�[�^�� 1���i�߂�
				nNumNor += 1;
			}
			else if (nNumVtx == (m_nNumVtx - 1) - m_part.x)
			{ // ���_�ԍ��������̏ꍇ

				// �@���̎g�p���� 1�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 1;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);

				// �@���f�[�^�� 1���i�߂�
				nNumNor += 1;
			}
			else if (nNumVtx < m_part.x)
			{ // ���_�ԍ����p�����������s�̒��_�̏ꍇ

				// �@���̎g�p���� 3�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 3;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeft(pVtx);

				// �@���f�[�^�� 3���i�߂�
				nNumNor += 3;
			}
			else if (nNumVtx > (m_part.x + 1) * m_part.y)
			{ // ���_�ԍ����p������������s�̒��_�̏ꍇ

				// �@���̎g�p���� 3�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 3;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// �@���f�[�^�� 3���i�߂�
				nNumNor += 3;
			}
			else if (nCntWidth == 0)
			{ // ���_�ԍ����p������������s�̒��_�̏ꍇ

				// �@���̎g�p���� 3�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 3;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);

				// �@���f�[�^�� 3���i�߂�
				nNumNor += 3;
			}
			else if (nCntWidth == m_part.x)
			{ // ���_�ԍ����p���������E��s�̒��_�̏ꍇ

				// �@���̎g�p���� 3�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 3;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// �@���f�[�^�� 3���i�߂�
				nNumNor += 3;
			}
			else
			{ // ���_�ԍ����O�����̒��_�̏ꍇ

				// �@���̎g�p���� 6�ɐݒ�
				m_pNumNorBuff[nNumVtx] = 6;

				// �@���̐ݒ�
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 3] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 4] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 5] = GetNormalLeftBottom(pVtx);

				// �@���f�[�^�� 6���i�߂�
				nNumNor += 6;
			}

			// ���_�f�[�^�̃|�C���^�� 1���i�߂�
			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//--------------------------------------------------------
	//	�@���̕��ω�
	//--------------------------------------------------------
	// �@���f�[�^�i�[�p�ϐ���������
	nNumNor = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{ // ���_�����J��Ԃ�

		// �ϐ���錾
		D3DXVECTOR3 nor = VEC3_ZERO;	// �@���ݒ�p

		for (int nCntNor = 0; nCntNor < m_pNumNorBuff[nCntVtx]; nCntNor++)
		{ // �ݒ肳��Ă���@�������J��Ԃ�

			// �@�������Z
			nor += m_pNorBuff[nNumNor];

			// �@���f�[�^�� 1���i�߂�
			nNumNor++;
		}

		// �@���𕽋ω�
		nor /= (float)m_pNumNorBuff[nCntVtx];

		// �@����ݒ�
		pVtx->nor = nor;

		// ���_�f�[�^�̃|�C���^�� 1���i�߂�
		pVtx += 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================
//	���b�V���̒��n����
//============================================================
bool CObjectMeshField::LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove)
{
	// �ϐ���錾
	float fLandHeight = GetPositionHeight(rPos);	// ���n�ʒu

	if (rPos.y < fLandHeight)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = fLandHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	���b�V���̒��_�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshField::SetMeshVertexPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W�̐ݒ�
			pVtx[0].pos = rPos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}
}

//============================================================
//	�n�`�̐ݒ菈��
//============================================================
void CObjectMeshField::SetTerrain(const POSGRID2& rPart, D3DXVECTOR3 *pPosGap)
{
	if (rPart != m_part)
	{ // ���������Ⴄ�ꍇ

		// ��������ݒ�
		SetPattern(rPart);
	}

	if (m_pPosGapBuff != NULL && rPart == m_part)
	{ // ���W�̂���o�b�t�@���g�p�����A�����������̏ꍇ

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, pPosGap++)
		{ // ���_�����J��Ԃ�

			// �����̍��W�̂���ʂ�ݒ�
			m_pPosGapBuff[nCntVtx] = *pPosGap;
		}
	}
	else { assert(false); }	// ��g�p��

	// ���_�E�C���f�b�N�X���̐ݒ�
	SetVtx(true);
	SetIdx();
}

//============================================================
//	���_���擾����
//============================================================
int CObjectMeshField::GetNumVertex(void) const
{
	// ���_����Ԃ�
	return m_nNumVtx;
}

//============================================================
//	���b�V���̒��_�ʒu�̐ݒ菈��
//============================================================
D3DXVECTOR3 CObjectMeshField::GetMeshVertexPosition(const int nID)
{
	// �ϐ���錾
	D3DXVECTOR3 pos;	// ���_���W�̑���p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		if (nID < (m_part.x + 1) * (m_part.y + 1))
		{ // �C���f�b�N�X���g�p�\�ȏꍇ

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�f�[�^�̃|�C���^���������i�߂�
			pVtx += nID;

			// ���_���W����
			pos = pVtx[0].pos;

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();
		}
	}

	// �����̃C���f�b�N�X�̒��_���W��Ԃ�
	return pos;
}

//============================================================
//	���b�V���͈͓̔��擾���� (��]�l��)
//============================================================
bool CObjectMeshField::IsPositionRange(const D3DXVECTOR3&rPos)
{
	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[4];		// �|���S���̒��_���W
	D3DXVECTOR3 aVtxMtxPos[4];	// �|���S���̈ʒu�E�������f���s�������_���W

	// ���b�V���̒��_�ʒu��ݒ�
	aVtxPos[0] = D3DXVECTOR3(m_meshField.pos.x - m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z + m_meshField.size.y * 0.5f);	// ����
	aVtxPos[1] = D3DXVECTOR3(m_meshField.pos.x - m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z - m_meshField.size.y * 0.5f);	// ����
	aVtxPos[2] = D3DXVECTOR3(m_meshField.pos.x + m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z - m_meshField.size.y * 0.5f);	// �E��
	aVtxPos[3] = D3DXVECTOR3(m_meshField.pos.x + m_meshField.size.x * 0.5f, 0.0f, m_meshField.pos.z + m_meshField.size.y * 0.5f);	// �E��

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{ // �O�p�`�|���S���̒��_�����J��Ԃ�

		// �ϐ���錾
		D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// ���_�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCntVtx].x, aVtxPos[nCntVtx].y, aVtxPos[nCntVtx].z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �|���S�������𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �|���S���ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �v�Z�����}�g���b�N�X������W��ݒ�
		aVtxMtxPos[nCntVtx] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	// �����Ԃ�
	return collision::BoxOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], aVtxMtxPos[3], rPos);
}

//============================================================
//	���b�V���̒��n�ʒu�̎擾���� (��]��l��)
//============================================================
float CObjectMeshField::GetPositionHeight(const D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	int nWidth  = (int)(( rPos.x + m_meshField.size.x * 0.5f) / (m_meshField.size.x / m_part.x));	// ���̕����ʒu
	int nHeight = (int)((-rPos.z + m_meshField.size.y * 0.5f) / (m_meshField.size.y / m_part.y));	// �c�̕����ʒu
	int nNumVtx = nWidth + (nHeight * (m_part.x + 1));	// �����ʒu�̍��㒸�_�ԍ�
	D3DXVECTOR3 nor;	// �@���x�N�g��

	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[4];	// �|���S���̒��_���W

	for (int nCntHeight = 0; nCntHeight < 3; nCntHeight++)
	{ // �c�̔���m�F�����J��Ԃ�

		// �ϐ���錾
		int nColl = -(m_part.x + 2) + (nCntHeight * (m_part.x + 1));	// ���肷�钸�_�ԍ��̕ύX�p

		for (int nCntWidth = 0; nCntWidth < 3; nCntWidth++)
		{ // ���̔���m�F�����J��Ԃ�

			// �ϐ��z���錾
			int aVtxNum[4] =	// ���_�ԍ�
			{ // ������
				nColl + nNumVtx + m_part.x + 1, // ����
				nColl + nNumVtx,			    // ����
				nColl + nNumVtx + m_part.x + 2, // �E��
				nColl + nNumVtx + 1			    // �E��
			};

			if (aVtxNum[1] >= 0 && aVtxNum[2] <= (m_part.x + 1) * (m_part.y + 1) - 1)
			{ // ���_�ԍ����͈͓��̏ꍇ

				// �|���S���̒��_�ʒu���擾
				aVtxPos[0] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[0]);
				aVtxPos[1] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[1]);
				aVtxPos[2] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[2]);
				aVtxPos[3] = m_meshField.pos + GetMeshVertexPosition(aVtxNum[3]);

				if (collision::TriangleOuterPillar(aVtxPos[0], aVtxPos[2], aVtxPos[1], rPos))
				{ // �|���S���͈͓̔��ɂ���ꍇ

					// �@�������߂�
					useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

					if (nor.y != 0.0f)
					{ // �@�����ݒ肳��Ă���ꍇ

						// �v���C���[�̒��n�_��Ԃ�
						return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
					}
				}

				if (collision::TriangleOuterPillar(aVtxPos[3], aVtxPos[1], aVtxPos[2], rPos))
				{ // �|���S���͈͓̔��ɂ���ꍇ

					// �@�������߂�
					useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

					if (nor.y != 0.0f)
					{ // �@�����ݒ肳��Ă���ꍇ

						// �v���C���[�̒��n�_��Ԃ�
						return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
					}
				}
			}

			// ���肷�钸�_�ԍ���ׂɕύX
			nColl++;
		}
	}

	// ���n�͈͊O�̏ꍇ���݂�y���W��Ԃ�
	return rPos.y;
}

//============================================================
//	���b�V���̒��n�ʒu�̎擾���� (��]�l��)
//============================================================
float CObjectMeshField::GetPositionRotateHeight(const D3DXVECTOR3&rPos)
{
	// �ϐ���錾
	int nNumCul;		// �@���x�N�g���p�̒��_�v�Z�p
	int nNumVtx;		// �@�������߂钸�_�ԍ�
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DXVECTOR3 pos;	// �^�[�Q�b�g�ʒu

	// �ϐ��z���錾
	D3DXVECTOR3 aVtxPos[NUM_VTX_TRIANGLE];		// �|���S���̒��_���W
	D3DXVECTOR3 aVtxMtxPos[NUM_VTX_TRIANGLE];	// �|���S���̈ʒu�E�������f���s�������_���W

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{ // �l�p�|���S���Ɏg�p����O�p�̐����J��Ԃ�

		// �@���x�N�g���p�̒��_�v�Z�p�̒l��ݒ�
		nNumCul = (nCntVtx == 0) ? 1: -1;

		for (int nCntHeight = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // �c�̕��������J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < m_part.x; nCntWidth++)
			{ // ���̕��������J��Ԃ�

				// �@�������߂钸�_�ԍ���ݒ�
				nNumVtx = ((nCntWidth + 1) + (nCntHeight * (m_part.x + 1))) + (nCntVtx * m_part.x);

				// �^�[�Q�b�g�ʒu��ݒ�
				pos = rPos;

				if (collision::CirclePillar(pos, GetMeshVertexPosition(nNumVtx), COLL_RADIUS, 0.0f))
				{ // ���_���W���߂��ɂ���ꍇ

					// �|���S���̒��_�ʒu���擾
					aVtxPos[0] = GetMeshVertexPosition(nNumVtx);
					aVtxPos[1] = GetMeshVertexPosition(nNumVtx - (1 * nNumCul));
					aVtxPos[2] = GetMeshVertexPosition(nNumVtx + ((m_part.x + 1) * nNumCul));

					for (int nCntTriangle = 0; nCntTriangle < NUM_VTX_TRIANGLE; nCntTriangle++)
					{ // �O�p�`�|���S���̒��_�����J��Ԃ�

						// �ϐ���錾
						D3DXMATRIX mtxWorld, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

						// ���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&mtxWorld);

						// ���_�ʒu�𔽉f
						D3DXMatrixTranslation(&mtxTrans, aVtxPos[nCntTriangle].x, aVtxPos[nCntTriangle].y, aVtxPos[nCntTriangle].z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

						// �|���S�������𔽉f
						D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshField.rot.y, m_meshField.rot.x, m_meshField.rot.z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

						// �|���S���ʒu�𔽉f
						D3DXMatrixTranslation(&mtxTrans, m_meshField.pos.x, m_meshField.pos.y, m_meshField.pos.z);
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

						// �v�Z�����}�g���b�N�X������W��ݒ�
						aVtxMtxPos[nCntTriangle] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
					}

					if (collision::TriangleOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], rPos))
					{ // �|���S���͈͓̔��ɂ���ꍇ

						// �@�������߂�
						useful::NormalizeNormal(aVtxMtxPos[1], aVtxMtxPos[0], aVtxMtxPos[2], nor);

						if (nor.y != 0.0f)
						{ // �@�����ݒ肳��Ă���ꍇ

							// �v���C���[�̒��n�_��Ԃ�
							return (((rPos.x - aVtxMtxPos[0].x) * nor.x + (-aVtxMtxPos[0].y) * nor.y + (rPos.z - aVtxMtxPos[0].z) * nor.z) * -1.0f) / nor.y;
						}
					}
				}
			}
		}
	}

	// ���n�͈͊O�̏ꍇ���݂�y���W��Ԃ�
	return rPos.y;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshField::SetVtx(bool bNor)
{
	// �ϐ���錾
	int nNumVtx = 0;	// ���݂̒��_�ԍ�

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

				// ���݂̒��_�ԍ���ݒ�
				nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3
				( // ����
					nCntWidth * (m_meshField.size.x / (float)m_part.x) - (m_meshField.size.x * 0.5f),		// x
					0.0f,																					// y
					-(nCntHeight * (m_meshField.size.y / (float)m_part.y) - (m_meshField.size.y * 0.5f))	// z
				);
				pVtx[0].pos += m_pPosGapBuff[nNumVtx];	// ���_����̂���ʂ����Z

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshField.col;

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(1.0f * nCntWidth, 1.0f * nCntHeight);

				// ���_�f�[�^�̃|�C���^�� 1���i�߂�
				pVtx += 1;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	if (bNor)
	{ // �@���̐��K�����s���ꍇ

		// �@���̐ݒ�E���K��
		NormalizeNormal();
	}
}

//============================================================
//	�C���f�b�N�X���̐ݒ菈��
//============================================================
void CObjectMeshField::SetIdx(void)
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
void CObjectMeshField::SetScrollTex(const float fTexU, const float fTexV)
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
//	�@���̎擾���� (��)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeft(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[m_part.x + 1].pos,	// ���ʒu
		pVtx[0].pos,			// ���S�ʒu
		pVtx[-1].pos,			// �E�ʒu
		nor						// �@��
	);

	// �@����Ԃ�
	return nor;
}

//============================================================
//	�@���̎擾���� (����)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeftTop(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[-(m_part.x + 2)].pos,	// ���ʒu
		pVtx[0].pos,				// ���S�ʒu
		pVtx[-(m_part.x + 1)].pos,	// �E�ʒu
		nor							// �@��
	);

	// �@����Ԃ�
	return nor;
}

//============================================================
//	�@���̎擾���� (����)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalLeftBottom(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[-1].pos,				// ���ʒu
		pVtx[0].pos,				// ���S�ʒu
		pVtx[-(m_part.x + 2)].pos,	// �E�ʒu
		nor							// �@��
	);

	// �@����Ԃ�
	return nor;
}

//============================================================
//	�@���̎擾���� (�E)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRight(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[-(m_part.x + 1)].pos,	// ���ʒu
		pVtx[0].pos,				// ���S�ʒu
		pVtx[1].pos,				// �E�ʒu
		nor							// �@��
	);

	// �@����Ԃ�
	return nor;
}

//============================================================
//	�@���̎擾���� (�E��)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRightTop(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[1].pos,			// ���ʒu
		pVtx[0].pos,			// ���S�ʒu
		pVtx[m_part.x + 2].pos,	// �E�ʒu
		nor						// �@��
	);

	// �@����Ԃ�
	return nor;
}

//============================================================
//	�@���̎擾���� (�E��)
//============================================================
D3DXVECTOR3 CObjectMeshField::GetNormalRightBottom(VERTEX_3D *pVtx)
{
	// �ϐ���錾
	D3DXVECTOR3 nor;	// �@������p

	// �@���̐��K��
	useful::NormalizeNormal
	( // ����
		pVtx[m_part.x + 2].pos,	// ���ʒu
		pVtx[0].pos,			// ���S�ʒu
		pVtx[m_part.x + 1].pos,	// �E�ʒu
		nor						// �@��
	);

	// �@����Ԃ�
	return nor;
}
