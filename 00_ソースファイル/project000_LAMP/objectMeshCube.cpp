//============================================================
//
//	�I�u�W�F�N�g���b�V���L���[�u���� [objectMeshCube.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCube.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "ZTexture.h"
#include "DepthShadow.h"
//************************************************************
//	�}�N����`
//************************************************************
#define NUM_SET_CUBE	(2)		// ��̃L���[�u�Ɏg�p����L���[�u�̐�
#define NUM_VTX_CUBE	(8)		// �l�p�`�̒��_��
#define NEED_VTX_CUBE	(24)	// ���b�V���L���[�u�ɕK�v�Ȓ��_�E�C���f�b�N�X��
#define NUM_CUBE_FACE	(6)		// �l�p�`�̖ʂ̐�
#define NUM_VTX_FACE	(4)		// �l�p�`�̈�ʂ̒��_��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const D3DXVECTOR3 CObjectMeshCube::aPosMeshCube[] =	// ���_���W�ݒ�p
{
	D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR3(+1.0f, -1.0f, -1.0f),
	D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR3(+1.0f, -1.0f, +1.0f),
	D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR3(-1.0f, -1.0f, +1.0f),
};

const D3DXVECTOR3 CObjectMeshCube::aNorMeshCube[] =	// �@���x�N�g���ݒ�p
{
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(+1.0f, 0.0f, 0.0f), D3DXVECTOR3(+1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(+1.0f, 0.0f, 0.0f), D3DXVECTOR3(+1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, +1.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f),
	D3DXVECTOR3(0.0f, 0.0f, +1.0f), D3DXVECTOR3(0.0f, 0.0f, +1.0f),
	D3DXVECTOR3(0.0f, 0.0f, +1.0f), D3DXVECTOR3(0.0f, 0.0f, +1.0f),
};

const D3DXVECTOR2 CObjectMeshCube::aTexMeshCube[] =	// �e�N�X�`�����W�ݒ�p
{
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f),
	D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f),
	D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f),
	D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f),
};

const int CObjectMeshCube::aIdxMeshCube[] =			// �C���f�b�N�X�ݒ�p
{
	8, 14, 10, 12, 4, 5, 2, 3, 18, 19, 16, 17, 0, 1, 6, 7, 22, 23, 20, 21, 13, 15, 11, 9,
};

//************************************************************
//	�q�N���X [CObjectMeshCube] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectMeshCube::CObjectMeshCube()
{
	// �����o�ϐ����N���A
	m_pVtxBuff	= NULL;	// ���_�o�b�t�@
	m_pIdxBuff	= NULL;	// �C���f�b�N�X�o�b�t�@
	m_nNumVtx	= 0;	// �K�v���_��
	m_nNumIdx	= 0;	// �K�v�C���f�b�N�X��
	m_origin	= ORIGIN_CENTER;	// ���_
	memset(&m_meshCube, 0, sizeof(m_meshCube));	// ���b�V���L���[�u�̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectMeshCube::CObjectMeshCube(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff	= NULL;	// ���_�o�b�t�@
	m_pIdxBuff	= NULL;	// �C���f�b�N�X�o�b�t�@
	m_nNumVtx	= 0;	// �K�v���_��
	m_nNumIdx	= 0;	// �K�v�C���f�b�N�X��
	m_origin	= ORIGIN_CENTER;	// ���_
	memset(&m_meshCube, 0, sizeof(m_meshCube));	// ���b�V���L���[�u�̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectMeshCube::~CObjectMeshCube()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectMeshCube::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;	// ���_�o�b�t�@
	m_pIdxBuff = NULL;	// �C���f�b�N�X�o�b�t�@
	m_nNumVtx = 0;		// �K�v���_��
	m_nNumIdx = 0;		// �K�v�C���f�b�N�X��
	m_origin = ORIGIN_CENTER;	// ���_

	// ��{���̏�����
	m_meshCube.pos  = VEC3_ZERO;			// �ʒu
	m_meshCube.rot  = VEC3_ZERO;			// ����
	m_meshCube.size = VEC3_ZERO;			// �傫��
	m_meshCube.aCol[0] = XCOL_WHITE;		// �L���[�u�F
	m_meshCube.aCol[1] = XCOL_BLACK;		// �����F
	m_meshCube.bordState  = BORDER_OFF;		// �����g�p���
	m_meshCube.fBordThick = 0.0f;			// ����葾��
	m_meshCube.texState   = TEXSTATE_ONE;	// �e�N�X�`���̎g�p���

	// �e�N�X�`���C���f�b�N�X�̏�����
	m_meshCube.texID.All	= NONE_IDX;	// �S�ʂ̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Left	= NONE_IDX;	// ���̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Right	= NONE_IDX;	// �E�̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Bottom	= NONE_IDX;	// ���̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Top	= NONE_IDX;	// ��̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Near	= NONE_IDX;	// �O�̃e�N�X�`���C���f�b�N�X
	m_meshCube.texID.Far	= NONE_IDX;	// ��̃e�N�X�`���C���f�b�N�X

	// �e�N�X�`���������̏�����
	m_meshCube.aTexPart[0] = VEC2_ONE;	// ������ x
	m_meshCube.aTexPart[1] = VEC2_ONE;	// ������ y
	m_meshCube.aTexPart[2] = VEC2_ONE;	// ������ z

	// �`����̏�����
	m_meshCube.cull   = D3DCULL_CCW;	// �J�����O��
	m_meshCube.bLight = true;			// ���C�e�B���O��

	SetEnableDepthShadow(true);
	SetEnableZTex(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectMeshCube::Uninit(void)
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

	// �I�u�W�F�N�g���b�V���L���[�u��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectMeshCube::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectMeshCube::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �ϐ��z���錾
	int aTexType[NUM_CUBE_FACE];	// �e�N�X�`���ݒ�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_meshCube.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshCube.rot.y, m_meshCube.rot.x, m_meshCube.rot.z);
	D3DXMatrixMultiply(&m_meshCube.mtxWorld, &m_meshCube.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshCube.pos.x, m_meshCube.pos.y, m_meshCube.pos.z);
	D3DXMatrixMultiply(&m_meshCube.mtxWorld, &m_meshCube.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_meshCube.mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, m_meshCube.cull);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, m_meshCube.bLight);

	// �L���[�u�̕`��
	switch (m_meshCube.texState)
	{ // �e�N�X�`���g�p��Ԃ��Ƃ̏���
	case TEXSTATE_ONE:		// ����e�N�X�`��

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetTexture(m_meshCube.texID.All));

		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetZShader()->BeginPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetAmbient(&m_meshCube.aCol[0]);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->BeginPass();
		}

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive
		( // ����
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			0,
			0,
			NEED_VTX_CUBE,			// �g�p���钸�_��
			0,						// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
			NEED_VTX_CUBE - 2		// �v���~�e�B�u (�|���S��) ��
		);

		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->EndPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->EndPass();
		}
		
		// �����𔲂���
		break;

	case TEXSTATE_SELECT:	// �S�I���e�N�X�`��

		// �e�N�X�`���̎�ނ�ݒ�
		aTexType[0] = m_meshCube.texID.Top;		// ��̃e�N�X�`��
		aTexType[1] = m_meshCube.texID.Right;	// �E�̃e�N�X�`��
		aTexType[2] = m_meshCube.texID.Near;	// �O�̃e�N�X�`��
		aTexType[3] = m_meshCube.texID.Left;	// ���̃e�N�X�`��
		aTexType[4] = m_meshCube.texID.Far;		// ��̃e�N�X�`��
		aTexType[5] = m_meshCube.texID.Bottom;	// ���̃e�N�X�`��
		
		for (int nCntFace = 0; nCntFace < NUM_CUBE_FACE; nCntFace++)
		{ // �ʂ̑������J��Ԃ�

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture->GetTexture(aTexType[nCntFace]));

			if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
			{
				CManager::GetInstance()->GetRenderer()->GetZShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
				CManager::GetInstance()->GetRenderer()->GetZShader()->SetParamToEffect();
				CManager::GetInstance()->GetRenderer()->GetZShader()->BeginPass();
			}
			else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
			{
				CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
				CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetAmbient(&m_meshCube.aCol[0]);
				CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetParamToEffect();
				CManager::GetInstance()->GetRenderer()->GetDepthShader()->BeginPass();
			}

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive
			( // ����
				D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				0,
				0,
				NUM_VTX_FACE,				// �g�p���钸�_��
				nCntFace * NUM_VTX_FACE,	// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
				NUM_VTX_FACE - 2			// �v���~�e�B�u (�|���S��) ��
			);

			if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
			{
				CManager::GetInstance()->GetRenderer()->GetZShader()->EndPass();
			}
			else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
			{
				CManager::GetInstance()->GetRenderer()->GetDepthShader()->EndPass();
			}
		}

		// �����𔲂���
		break;
	}

	// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, true);

	// �����̕`��
	if (m_meshCube.bordState == BORDER_ON)
	{ // ����肪ON�̏ꍇ

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �|���S���̗��ʂ݂̂�\����Ԃɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetZShader()->BeginPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetWorldMatrix(&m_meshCube.mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetAmbient(&m_meshCube.aCol[0]);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->BeginPass();
		}

		// �|���S���̕`��
		pDevice->DrawIndexedPrimitive
		( // ����
			D3DPT_TRIANGLESTRIP,	// �v���~�e�B�u�̎��
			0,
			0,
			NEED_VTX_CUBE,			// �g�p���钸�_��
			NEED_VTX_CUBE,			// �C���f�b�N�X�o�b�t�@�̊J�n�n�_
			NEED_VTX_CUBE - 2		// �v���~�e�B�u (�|���S��) ��
		);

		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->EndPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->EndPass();
		}

		// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, true);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_meshCube.pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_meshCube.pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectMeshCube::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_meshCube.rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_meshCube.rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_meshCube.rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectMeshCube::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �����̑傫����ݒ�
	m_meshCube.size = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObjectMeshCube::GetVec3Sizing(void) const
{
	// �傫����Ԃ�
	return m_meshCube.size;
}

//============================================================
//	�L���[�u�F�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetColor(const D3DXCOLOR& rCol)
{
	// �����̃L���[�u�F��ݒ�
	m_meshCube.aCol[CUBECOL_CUBE] = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�L���[�u�F�擾����
//============================================================
D3DXCOLOR CObjectMeshCube::GetColor(void) const
{
	// �L���[�u�F��Ԃ�
	return m_meshCube.aCol[CUBECOL_CUBE];
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectMeshCube::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_meshCube.mtxWorld;
}

//============================================================
//	�e�N�X�`���̕�����X�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetTexturePatternX(const D3DXVECTOR2& rTexPart)
{
	// �����̃e�N�X�`���̕�����X��ݒ�
	m_meshCube.aTexPart[CUBEPART_X] = rTexPart;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`��������X�擾����
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternX(void) const
{
	// �e�N�X�`���̕�����X��Ԃ�
	return m_meshCube.aTexPart[CUBEPART_X];
}

//============================================================
//	�e�N�X�`���̕�����Y�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetTexturePatternY(const D3DXVECTOR2& rTexPart)
{
	// �����̃e�N�X�`���̕�����Y��ݒ�
	m_meshCube.aTexPart[CUBEPART_Y] = rTexPart;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`��������Y�擾����
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternY(void) const
{
	// �e�N�X�`���̕�����Y��Ԃ�
	return m_meshCube.aTexPart[CUBEPART_Y];
}

//============================================================
//	�e�N�X�`���̕�����Z�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetTexturePatternZ(const D3DXVECTOR2& rTexPart)
{
	// �����̃e�N�X�`���̕�����Z��ݒ�
	m_meshCube.aTexPart[CUBEPART_Z] = rTexPart;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�e�N�X�`��������Z�擾����
//============================================================
D3DXVECTOR2 CObjectMeshCube::GetTexturePatternZ(void) const
{
	// �e�N�X�`���̕�����Z��Ԃ�
	return m_meshCube.aTexPart[CUBEPART_Z];
}

//============================================================
//	��������
//============================================================
CObjectMeshCube *CObjectMeshCube::Create
(
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	const D3DXVECTOR3& rSize,		// �傫��
	const D3DXCOLOR& rCubeCol,		// �L���[�u�F
	const D3DXCOLOR& rBorderCol,	// �����F
	const EBorder bordState,		// �������
	const float fBordThick,			// ����葾��
	const ETexState texState,		// �e�N�X�`�����
	const SFaceTex& rTexID,			// �e�N�X�`�����
	const EOrigin origin,			// ���_
	const D3DXVECTOR2& rTexPartX,	// �e�N�X�`�������� x
	const D3DXVECTOR2& rTexPartY,	// �e�N�X�`�������� y
	const D3DXVECTOR2& rTexPartZ,	// �e�N�X�`�������� z
	const D3DCULL cull,				// �J�����O��
	const bool bLight				// ���C�e�B���O��
)
{
	// �|�C���^��錾
	CObjectMeshCube *pObjectMeshCube = NULL;	// �I�u�W�F�N�g���b�V���L���[�u�����p

	if (pObjectMeshCube == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectMeshCube = new CObjectMeshCube;	// �I�u�W�F�N�g���b�V���L���[�u
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectMeshCube != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g���b�V���L���[�u�̏�����
		if (FAILED(pObjectMeshCube->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshCube;
			pObjectMeshCube = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`��������
		pObjectMeshCube->BindTexture(rTexID);

		// ���_��ݒ�
		pObjectMeshCube->SetOrigin(origin);

		// �ʒu��ݒ�
		pObjectMeshCube->SetVec3Position(rPos);

		// ������ݒ�
		pObjectMeshCube->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObjectMeshCube->SetVec3Sizing(rSize);

		// �L���[�u�F��ݒ�
		pObjectMeshCube->SetColor(rCubeCol);

		// �����F��ݒ�
		pObjectMeshCube->SetBorderColor(rBorderCol);

		// �����̏�Ԃ�ݒ�
		if (FAILED(pObjectMeshCube->SetBorderState(bordState)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectMeshCube;
			pObjectMeshCube = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �����̑�����ݒ�
		pObjectMeshCube->SetBorderThick(fBordThick);

		// �e�N�X�`���̏�Ԃ�ݒ�
		pObjectMeshCube->SetTextureState(texState);

		// �e�N�X�`���̕�����X��ݒ�
		pObjectMeshCube->SetTexturePatternX(rTexPartX);

		// �e�N�X�`���̕�����Y��ݒ�
		pObjectMeshCube->SetTexturePatternY(rTexPartY);

		// �e�N�X�`���̕�����Z��ݒ�
		pObjectMeshCube->SetTexturePatternZ(rTexPartZ);

		// �J�����O��ݒ�
		pObjectMeshCube->SetCulling(cull);

		// ���C�e�B���O��ݒ�
		pObjectMeshCube->SetLighting(bLight);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectMeshCube;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`����������
//============================================================
void CObjectMeshCube::BindTexture(const SFaceTex textureID)
{
	// �e�N�X�`���C���f�b�N�X����
	m_meshCube.texID = textureID;
}

//============================================================
//	�����F�̐ݒ菈��
//============================================================
void CObjectMeshCube::SetBorderColor(const D3DXCOLOR& rCol)
{
	// �����̉����F��ݒ�
	m_meshCube.aCol[CUBECOL_BORDER] = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����F�擾����
//============================================================
D3DXCOLOR CObjectMeshCube::GetBorderColor(void) const
{
	// �����F��Ԃ�
	return m_meshCube.aCol[CUBECOL_BORDER];
}

//============================================================
//	�����̏�Ԃ̐ݒ菈��
//============================================================
HRESULT CObjectMeshCube::SetBorderState(const EBorder bordState)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����̉����̏�Ԃ�ݒ�
	m_meshCube.bordState = bordState;

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
			sizeof(VERTEX_3D) * (NEED_VTX_CUBE + ((int)bordState * NEED_VTX_CUBE)),	// �K�v���_��
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
			sizeof(WORD) * (NEED_VTX_CUBE + ((int)bordState * NEED_VTX_CUBE)),	// �K�v�C���f�b�N�X��
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
//	������Ԏ擾����
//============================================================
CObjectMeshCube::EBorder CObjectMeshCube::GetBorderState(void) const
{
	// �����̏�Ԃ�Ԃ�
	return m_meshCube.bordState;
}

//============================================================
//	�����̑����̐ݒ菈��
//============================================================
void CObjectMeshCube::SetBorderThick(const float fBordThick)
{
	// �����̉����̑�����ݒ�
	m_meshCube.fBordThick = fBordThick;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	����葾���擾����
//============================================================
float CObjectMeshCube::GetBorderThick(void) const
{
	// �����̑�����Ԃ�
	return m_meshCube.fBordThick;
}

//============================================================
//	�e�N�X�`���̏�Ԃ̐ݒ菈��
//============================================================
void CObjectMeshCube::SetTextureState(const ETexState texState)
{
	// �����̃e�N�X�`���̏�Ԃ�ݒ�
	m_meshCube.texState = texState;
}

//============================================================
//	�e�N�X�`����Ԏ擾����
//============================================================
CObjectMeshCube::ETexState CObjectMeshCube::GetTextureState(void) const
{
	// �e�N�X�`���̏�Ԃ�Ԃ�
	return m_meshCube.texState;
}

//============================================================
//	���_�ݒ菈��
//============================================================
void CObjectMeshCube::SetOrigin(const EOrigin origin)
{
	// �����̌��_��ݒ�
	m_origin = origin;
}

//============================================================
//	���_�擾����
//============================================================
CObjectMeshCube::EOrigin CObjectMeshCube::GetOrigin(void) const
{
	// ���_��Ԃ�
	return m_origin;
}

//============================================================
//	�J�����O�ݒ菈��
//============================================================
void CObjectMeshCube::SetCulling(const D3DCULL cull)
{
	// �����̃J�����O�󋵂�ݒ�
	m_meshCube.cull = cull;
}

//============================================================
//	�J�����O�擾����
//============================================================
D3DCULL CObjectMeshCube::GetCulling(void) const
{
	// �J�����O�󋵂�Ԃ�
	return m_meshCube.cull;
}

//============================================================
//	���C�e�B���O�ݒ菈��
//============================================================
void CObjectMeshCube::SetLighting(const bool bLight)
{
	// �����̃��C�e�B���O�󋵂�ݒ�
	m_meshCube.bLight = bLight;
}

//============================================================
//	���C�e�B���O�擾����
//============================================================
bool CObjectMeshCube::GetLighting(void) const
{
	// ���C�e�B���O�󋵂�Ԃ�
	return m_meshCube.bLight;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectMeshCube::SetVtx(void)
{
	// �ϐ���錾
	int nLoop = 0;	// �L���[�u�̕`�搔
	float fSetBord = 0.0f, fUseBord = 0.0f;	// ���_���W�v�Z�p
	D3DXVECTOR2 texPart = VEC2_ZERO;		// �e�N�X�`���������̌v�Z�p

	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �����̗L���ŕ`�搔��ύX
		nLoop = (int)m_meshCube.bordState + 1;

		for (int nCntBorder = 0; nCntBorder < nLoop; nCntBorder++)
		{ // ����肪ON�̏ꍇ���J��Ԃ�

			// ���_���W�̌v�Z�p�̒l��ݒ�
			fSetBord = (float)(nCntBorder - 1) * m_meshCube.fBordThick;
			fUseBord = (float)m_meshCube.bordState;

			for (int nCntSet = 0; nCntSet < NEED_VTX_CUBE; nCntSet++)
			{ // �K�v���_�E�C���f�b�N�X�����J��Ԃ�

				// �ϐ���錾
				int nVtxID = nCntSet % NUM_VTX_CUBE;	// ���݂̔��̒��_�ԍ�
				int nBoxID = nCntSet / NUM_VTX_CUBE;	// ���݂̔��ԍ�

				if (nVtxID == 0)
				{ // ���_�̐ݒ肪�ꏄ�����ꍇ

					// �e�N�X�`����������ύX
					texPart = D3DXVECTOR2
					( // ����
						m_meshCube.aTexPart[nBoxID].x,	// x
						m_meshCube.aTexPart[nBoxID].y	// y
					);
				}

				switch (m_origin)
				{ // ���_���Ƃ̏���
				case ORIGIN_CENTER:	// ����
			
					// ���_���W�̐ݒ�
					pVtx[0].pos.x = (m_meshCube.size.x * aPosMeshCube[nVtxID].x) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].x);
					pVtx[0].pos.y = (m_meshCube.size.y * aPosMeshCube[nVtxID].y) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].y);
					pVtx[0].pos.z = (m_meshCube.size.z * aPosMeshCube[nVtxID].z) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].z);
			
					break;
			
				case ORIGIN_DOWN:	// ��
			
					// ���_���W�̐ݒ�
					pVtx[0].pos.x = (m_meshCube.size.x * aPosMeshCube[nVtxID].x) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].x);
					pVtx[0].pos.y = (m_meshCube.size.y * aPosMeshCube[nVtxID].y * 2.0f) * -((nVtxID % 2) - 1) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].y);
					pVtx[0].pos.z = (m_meshCube.size.z * aPosMeshCube[nVtxID].z) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].z);
			
					break;

				case ORIGIN_UP:		// �㌴�_

					// ���_���W�̐ݒ�
					pVtx[0].pos.x = (m_meshCube.size.x * aPosMeshCube[nVtxID].x) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].x);
					pVtx[0].pos.y = (m_meshCube.size.y * aPosMeshCube[nVtxID].y * 2.0f) * (nVtxID % 2) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].y);
					pVtx[0].pos.z = (m_meshCube.size.z * aPosMeshCube[nVtxID].z) - (fSetBord * fUseBord * -aPosMeshCube[nVtxID].z);

					break;
			
				default:	// ��O����
					assert(false);
					break;
				}

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = aNorMeshCube[nCntSet];

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_meshCube.aCol[nCntBorder];

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex.x = texPart.x * aTexMeshCube[nCntSet].x;
				pVtx[0].tex.y = texPart.y * aTexMeshCube[nCntSet].y;

				// ���_�f�[�^�̃|�C���^���ړ�
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
void CObjectMeshCube::SetIdx(void)
{
	// �ϐ���錾
	int nNumVtx = 0;	// ���_���̌v���p
	int nLoop = 0;		// �L���[�u�̕`�搔

	// �|�C���^��錾
	WORD *pIdx;	// �C���f�b�N�X���ւ̃|�C���^

	if (m_pIdxBuff != NULL)
	{ // �g�p���̏ꍇ

		// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// �����̗L���ŕ`�搔��ύX
		nLoop = (int)m_meshCube.bordState + 1;

		for (int nCntBorder = 0; nCntBorder < nLoop; nCntBorder++)
		{ // ����肪ON�̏ꍇ���J��Ԃ�

			for (int nCntSet = 0; nCntSet < NEED_VTX_CUBE; nCntSet++)
			{ // �K�v���_�E�C���f�b�N�X�����J��Ԃ�

				// �C���f�b�N�X�̐ݒ�
				pIdx[0] = (WORD)(aIdxMeshCube[nCntSet] + nNumVtx);

				// �C���f�b�N�X�f�[�^�̃|�C���^���ړ�
				pIdx += 1;
			}

			// ���_�o�b�t�@�̊J�n�n�_��K�v�������炷
			nNumVtx += NEED_VTX_CUBE;
		}

		// �C���f�b�N�X�o�b�t�@���A�����b�N����
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	�j������
//============================================================
void CObjectMeshCube::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
