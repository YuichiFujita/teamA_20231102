//============================================================
//
//	�}���`���f������ [multiModel.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "multiModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "ZTexture.h"
#include "DepthShadow.h"

//************************************************************
//	�q�N���X [CMultiModel] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMultiModel::CMultiModel()
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pParent = NULL;		// �e���f���ւ̃|�C���^
	m_pMat	= NULL;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ZERO;	// �g�嗦
	m_nModelID = 0;			// ���f���C���f�b�N�X
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CMultiModel::CMultiModel(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pParent = NULL;		// �e���f���ւ̃|�C���^
	m_pMat	= NULL;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ZERO;	// �g�嗦
	m_nModelID = 0;			// ���f���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMultiModel::~CMultiModel()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMultiModel::Init(void)
{
	// �����o�ϐ���������
	memset(&m_modelData, 0, sizeof(m_modelData));	// ���f�����
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));		// ���[���h�}�g���b�N�X
	m_pParent = NULL;		// �e���f���ւ̃|�C���^
	m_pMat	= NULL;			// �}�e���A���ւ̃|�C���^
	m_pos	= VEC3_ZERO;	// �ʒu
	m_rot	= VEC3_ZERO;	// ����
	m_scale	= VEC3_ONE;		// �g�嗦
	m_nModelID = NONE_IDX;	// ���f���C���f�b�N�X

	// �����X�V�Ǝ����`���OFF�ɂ���
	SetEnableUpdate(false);
	SetEnableDraw(false);

	SetEnableZTex(true);
	SetEnableDepthShadow(true);
	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMultiModel::Uninit(void)
{
	// �}�e���A���ւ̃|�C���^��j��
	if (m_pMat != NULL)
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = NULL;
	}

	// �}���`���f����j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CMultiModel::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CMultiModel::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX   mtxParent;	// �e�̃}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�}�g���b�N�X��ݒ�
	if (m_pParent == NULL)
	{ // �e�����݂��Ȃ��ꍇ

		// ���݂̃}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// �ݒ肳�ꂽ�ŐV�̃}�g���b�N�X (���̂̃}�g���b�N�X)
	}
	else
	{ // �e�����݂���ꍇ

		// �e�̃}�g���b�N�X��ݒ�
		mtxParent = *m_pParent->GetPtrMtxWorld();
	}

	// ���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetTexture(m_modelData.pTextureID[nCntMat]));

		if (m_scale != VEC3_ONE)
		{ // �g�嗦���ύX����Ă���ꍇ

			// ���_�@���̎������K����L���ɂ���
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		}
		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetWorldMatrix(&m_mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetZShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetZShader()->BeginPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetWorldMatrix(&m_mtxWorld);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetAmbient(&(D3DXCOLOR)GetMaterial(nCntMat).MatD3D.Diffuse);
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->SetParamToEffect();
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->BeginPass();
		}
		// ���f���̕`��
		m_modelData.pMesh->DrawSubset(nCntMat);
		if (CManager::GetInstance()->GetRenderer()->GetZShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetZShader()->EndPass();
		}
		else if (CManager::GetInstance()->GetRenderer()->GetDepthShader()->GetbPass())
		{
			CManager::GetInstance()->GetRenderer()->GetDepthShader()->EndPass();
		}

		// ���_�@���̎������K���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//============================================================
//	���f���������� (�C���f�b�N�X)
//============================================================
void CMultiModel::BindModel(const int nModelID)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetInstance()->GetModel();	// ���f���ւ̃|�C���^
	if (pModel == NULL)
	{ // ���f���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (nModelID > NONE_IDX)
	{ // ���f���C���f�b�N�X���g�p�\�ȏꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = nModelID;

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(nModelID);

		// ���}�e���A���̐ݒ�
		SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat);
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	���f���������� (�p�X)
//============================================================
void CMultiModel::BindModel(const char *pModelPass)
{
	// �|�C���^��錾
	CModel *pModel = CManager::GetInstance()->GetModel();	// ���f���ւ̃|�C���^
	if (pModel == NULL)
	{ // ���f���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (pModelPass != NULL)
	{ // ���蓖�Ă郂�f���p�X�����݂���ꍇ

		// ���f���C���f�b�N�X����
		m_nModelID = pModel->Regist(pModelPass);

		// ���f�������蓖��
		m_modelData = *pModel->GetModel(m_nModelID);

		// ���}�e���A���̐ݒ�
		SetOriginMaterial(m_modelData.pBuffMat, (int)m_modelData.dwNumMat);
	}
	else { assert(false); }	// ���f���p�X����
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CMultiModel::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CMultiModel::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�g�嗦�̐ݒ菈��
//============================================================
void CMultiModel::SetVec3Scaling(const D3DXVECTOR3& rScale)
{
	// �����̊g�嗦����
	m_scale = rScale;
}

//============================================================
//	�g�嗦�擾����
//============================================================
D3DXVECTOR3 CMultiModel::GetVec3Scaling(void) const
{
	// �g�嗦��Ԃ�
	return m_scale;
}

//============================================================
//	�}�e���A���̑S�ݒ菈��
//============================================================
void CMultiModel::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	// �����̃}�e���A����S�}�e���A���ɐݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = rMat;
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CMultiModel::ResetMaterial(void)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	// �S�}�e���A���ɏ����}�e���A�����Đݒ�
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CMultiModel::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CMultiModel::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	��������
//============================================================
CMultiModel *CMultiModel::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, const D3DXVECTOR3& rScale)
{
	// �|�C���^��錾
	CMultiModel *pMultiModel = NULL;	// �}���`���f�������p

	if (pMultiModel == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMultiModel = new CMultiModel;	// �}���`���f��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMultiModel != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �}���`���f���̏�����
		if (FAILED(pMultiModel->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMultiModel;
			pMultiModel = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pMultiModel->SetVec3Position(rPos);

		// ������ݒ�
		pMultiModel->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pMultiModel->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pMultiModel;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�}�e���A���ݒ菈��
//============================================================
void CMultiModel::SetMaterial(const D3DXMATERIAL& rMat, const int nID)
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����ݒ�
		m_pMat[nID] = rMat;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�}�e���A���擾����
//============================================================
D3DXMATERIAL CMultiModel::GetMaterial(const int nID) const
{
	if (nID > NONE_IDX && nID < (int)m_modelData.dwNumMat)
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă��Ȃ��ꍇ

		// �����C���f�b�N�X�̃}�e���A����Ԃ�
		return m_pMat[nID];
	}
	else
	{ // �����C���f�b�N�X���}�e���A���̍ő吔�𒴂��Ă���ꍇ

		// �ϐ���錾
		D3DXMATERIAL mat;	// ��O���̃}�e���A��

		// �}�e���A���̃��������N���A
		ZeroMemory(&mat, sizeof(mat));

		// ��O����
		assert(false);

		// ��O���̃}�e���A����Ԃ�
		return mat;
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CMultiModel::SetAlpha(const float fAlpha)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �ϐ���錾
		float fSetAlpha = fAlpha;	// �ݒ肷�铧���x

		// �����x�̕␳
		useful::LimitNum(fSetAlpha, 0.0f, pOriginMat[nCntMat].MatD3D.Diffuse.a);

		// �����x��ݒ�
		m_pMat[nCntMat].MatD3D.Diffuse.a = fSetAlpha;
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CMultiModel::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (m_pMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_pMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CMultiModel::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)m_modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_modelData.dwNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		if (pOriginMat[nCntMat].MatD3D.Diffuse.a > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = pOriginMat[nCntMat].MatD3D.Diffuse.a;
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�}�g���b�N�X�̐ݒ菈��
//============================================================
void CMultiModel::SetMtxWorld(const D3DXMATRIX& rMtxWorld)
{
	// �����̃}�g���b�N�X��ݒ�
	m_mtxWorld = rMtxWorld;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CMultiModel::SetParentObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g�|�C���^��e�ɐݒ�
	m_pParent = pObject;
}

//============================================================
//	�e�I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CMultiModel::SetParentModel(CMultiModel *pModel)
{
	// �����̃��f���|�C���^��e�ɐݒ�
	m_pParent = (CObject*)pModel;
}

//============================================================
//	�e�I�u�W�F�N�g�폜����
//============================================================
void CMultiModel::DeleteParentObject(void)
{
	// �e�I�u�W�F�N�g��NULL�ɂ���
	m_pParent = NULL;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CMultiModel::SetModelData(const CModel::SModel& rModel)
{
	// �����̃��f��������
	m_modelData = rModel;
}

//============================================================
//	���f�����擾����
//============================================================
CModel::SModel CMultiModel::GetModelData(void) const
{
	// ���f������Ԃ�
	return m_modelData;
}

//============================================================
//	�j������
//============================================================
void CMultiModel::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���}�e���A���̐ݒ菈��
//============================================================
HRESULT CMultiModel::SetOriginMaterial(const LPD3DXBUFFER pBuffMat, const int nNumMat)
{
	// �|�C���^��錾
	D3DXMATERIAL *pOriginMat;	// �}�e���A���f�[�^�ւ̃|�C���^

	//--------------------------------------------------------
	//	�������J���E�m��
	//--------------------------------------------------------
	if (m_pMat != NULL)
	{ // �|�C���^���g�p����Ă����ꍇ

		// �������J��
		delete[] m_pMat;
		m_pMat = NULL;
	}

	if (m_pMat == NULL)
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// �}�e���A�������̃������m��
		m_pMat = new D3DXMATERIAL[nNumMat];

		if (m_pMat != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_pMat, 0, sizeof(D3DXMATERIAL) * nNumMat);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�}�e���A������ݒ�
	//--------------------------------------------------------
	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pOriginMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < nNumMat; nCntMat++)
	{ // �}�e���A���̐����J��Ԃ�

		// �}�e���A���f�[�^���R�s�[
		m_pMat[nCntMat] = pOriginMat[nCntMat];
	}

	// ������Ԃ�
	return S_OK;
}
