//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[���� [objectChara.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	�q�N���X [CObjectChara] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara()
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_pMotion = NULL;	// ���[�V�����̏��
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_apMultiModel[0], 0, sizeof(m_apMultiModel));	// ���f���̏��
	m_pMotion = NULL;	// ���[�V�����̏��
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectChara::Init(void)
{
	// �����o�ϐ���������
	m_pos = VEC3_ZERO;	// �ʒu
	m_rot = VEC3_ZERO;	// ����
	m_nNumModel = 0;	// �p�[�c�̑���

	// ���[�V�����I�u�W�F�N�g�̐���
	m_pMotion = CMotion::Create();
	if (m_pMotion == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectChara::Uninit(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < MAX_PARTS; nCntObjectChara++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (m_apMultiModel[nCntObjectChara] != NULL)
		{ // �p�[�c���g�p���̏ꍇ

			// �}���`���f���̏I��
			m_apMultiModel[nCntObjectChara]->Uninit();
		}
	}

	if (m_pMotion != NULL)
	{ // ���[�V�������g���Ă���ꍇ

		// ���[�V������j��
		if (FAILED(m_pMotion->Release(m_pMotion)))
		{ // �j���Ɏ��s�����ꍇ

			// ��O����
			assert(false);
		}
	}
	else { assert(false); }	// ��g�p��

	// �I�u�W�F�N�g�L�����N�^�[��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectChara::Update(void)
{
	// ���[�V�����̍X�V
	m_pMotion->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CObjectChara::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �p�[�c�̕`��
		m_apMultiModel[nCntObjectChara]->Draw();
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectChara::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectChara::GetPtrMtxWorld(void)
{
	// �}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�p�[�c���̐ݒ菈��
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nID,				// �p�[�c�C���f�b�N�X
	const int nParentID,		// �e�C���f�b�N�X
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const char *pFileName		// �t�@�C����
)
{
	if (nID < MAX_PARTS && nParentID < MAX_PARTS && pFileName != NULL)
	{ // �p�[�c�E�e�C���f�b�N�X���z��͈͓����A�t�@�C���������݂���ꍇ

		// ���f���̐���
		m_apMultiModel[nID] = CMultiModel::Create(rPos, rRot);

		// ���f��������
		m_apMultiModel[nID]->BindModel(pFileName);

		// �e���f���̐ݒ�
		if (nParentID == NONE_IDX)
		{ // �e���Ȃ��ꍇ

			// NULL��ݒ�
			m_apMultiModel[nID]->SetParentModel(NULL);
		}
		else
		{ // �e������ꍇ

			// �e�̃A�h���X��ݒ�
			m_apMultiModel[nID]->SetParentModel(m_apMultiModel[nParentID]);
		}

		// �p�[�c�̑��������Z
		m_nNumModel++;
	}
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CObjectChara::SetModelInfo(void)
{
	// ���f�����̐ݒ�
	m_pMotion->SetModel(&m_apMultiModel[0], m_nNumModel);
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CObjectChara::SetMotionInfo(CMotion::SMotionInfo info)
{
	// ���[�V�������̐ݒ�
	m_pMotion->SetInfo(info);
}

//============================================================
//	���[�V�����̐ݒ菈��
//============================================================
void CObjectChara::SetMotion(const int nType)
{
	// ���[�V�����̐ݒ�
	m_pMotion->Set(nType);
}

//============================================================
//	���[�V�����̍X�V�󋵂̐ݒ菈��
//============================================================
void CObjectChara::SetEnableMotionUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂����[�V�����̍X�V�󋵂ɐݒ�
	m_pMotion->SetEnableUpdate(bUpdate);
}

//============================================================
//	�p�[�c�ʒu�̐ݒ菈��
//============================================================
void CObjectChara::SetPartsPosition(const int nID, const D3DXVECTOR3& rPos)
{
	// �����̃C���f�b�N�X�̈ʒu��ݒ�
	m_apMultiModel[nID]->SetVec3Position(rPos);
}

//============================================================
//	�p�[�c�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsPosition(const int nID) const
{
	// �����C���f�b�N�X�̃p�[�c�̈ʒu��Ԃ�
	return m_apMultiModel[nID]->GetVec3Position();
}

//============================================================
//	�p�[�c�����̐ݒ菈��
//============================================================
void CObjectChara::SetPartsRotation(const int nID, const D3DXVECTOR3& rRot)
{
	// �����̃C���f�b�N�X�̌�����ݒ�
	m_apMultiModel[nID]->SetVec3Rotation(rRot);
}

//============================================================
//	�p�[�c�����擾����
//============================================================
D3DXVECTOR3 CObjectChara::GetPartsRotation(const int nID) const
{
	// �����C���f�b�N�X�̃p�[�c�̌�����Ԃ�
	return m_apMultiModel[nID]->GetVec3Rotation();
}

//============================================================
//	�}���`���f���擾����
//============================================================
CMultiModel *CObjectChara::GetMultiModel(const int nID) const
{
	if (nID < m_nNumModel)
	{ // �g�p�\�ȃC���f�b�N�X�̏ꍇ

		// �}���`���f���̏���Ԃ�
		return m_apMultiModel[nID];
	}
	else { assert(false); return m_apMultiModel[0]; }
}

//============================================================
//	���[�V������ގ擾����
//============================================================
int CObjectChara::GetMotionType(void) const
{
	// ���[�V�����̎�ނ�Ԃ�
	return m_pMotion->GetType();
}

//============================================================
//	���[�V�����|�[�Y�ԍ��擾����
//============================================================
int CObjectChara::GetMotionPose(void) const
{
	// ���[�V�����̃|�[�Y�ԍ���Ԃ�
	return m_pMotion->GetPose();
}

//============================================================
//	���[�V�����J�E���^�[�擾����
//============================================================
int CObjectChara::GetMotionCounter(void) const
{
	// ���[�V�����̃J�E���^�[��Ԃ�
	return m_pMotion->GetCounter();
}

//============================================================
//	���[�V�����I���擾����
//============================================================
bool CObjectChara::IsMotionFinish(void) const
{
	// ���[�V�����̏I���󋵂�Ԃ�
	return m_pMotion->IsFinish();
}

//============================================================
//	���[�V�������[�v�擾����
//============================================================
bool CObjectChara::IsMotionLoop(const int nType) const
{
	// �����̎�ނ̃��[�V�������[�v�󋵂�Ԃ�
	return m_pMotion->IsLoop(nType);
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �����̃}�e���A����S�}�e���A���ɐݒ�
		m_apMultiModel[nCntObjectChara]->SetAllMaterial(rMat);
	}
}

//============================================================
//	�}�e���A���̍Đݒ菈��
//============================================================
void CObjectChara::ResetMaterial(void)
{
	for (int nCntObjectChara = 0; nCntObjectChara < m_nNumModel; nCntObjectChara++)
	{ // �p�[�c�̑������J��Ԃ�

		// �S�}�e���A���ɏ����}�e���A�����Đݒ�
		m_apMultiModel[nCntObjectChara]->ResetMaterial();
	}
}

//============================================================
//	�����x�̐ݒ菈��
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		// �����̓����x��ݒ�
		m_apMultiModel[nCntParts]->SetAlpha(fAlpha);
	}
}

//============================================================
//	�����x�擾����
//============================================================
float CObjectChara::GetAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (m_apMultiModel[nCntParts]->GetAlpha() > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_apMultiModel[nCntParts]->GetAlpha();
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}

//============================================================
//	�ő哧���x�擾����
//============================================================
float CObjectChara::GetMaxAlpha(void) const
{
	// �ϐ���錾
	float fAlpha = 0.0f;	// �ł��s�����ȃ}�e���A���̓����x

	// �ł��s�����ȓ����x��T��
	for (int nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{ // �p�[�c�̍ő吔���J��Ԃ�

		if (m_apMultiModel[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // �}�e���A���̓����x�����s�����������ꍇ

			// ���݂̃}�e���A���̓����x��ۑ�
			fAlpha = m_apMultiModel[nCntParts]->GetMaxAlpha();
		}
	}

	// �S�p�[�c���ōł��s�����������}�e���A���̓����x��Ԃ�
	return fAlpha;
}
