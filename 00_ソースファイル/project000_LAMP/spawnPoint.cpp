//============================================================
//
//	�����ʒu���� [spawnPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "spawnPoint.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 0;	// �����ʒu�̗D�揇��
}
//************************************************************
//	�ÓI�����o�ϐ�
//************************************************************
int CSpawnPoint::m_nNumIdx = 0;		// �X�|�[���ԍ��̌�

//************************************************************
//	�q�N���X [CSpawnPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSpawnPoint::CSpawnPoint() : CObject(CObject::LABEL_SPAWNPOINT, PRIORITY), m_Idx(m_nNumIdx)
{
	// �����o�ϐ���������
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;

	// �X�|�[���ԍ��̌������Z
	m_nNumIdx++;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSpawnPoint::~CSpawnPoint()
{
	// �X�|�[���ԍ��̌������Z
	m_nNumIdx--;
}

//============================================================
//	����������
//============================================================
HRESULT CSpawnPoint::Init(void)
{
	// �����o�ϐ���������
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSpawnPoint::Uninit(void)
{
	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CSpawnPoint::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CSpawnPoint::Draw(void)
{

}

//============================================================
//	�ʒu�ݒ�
//============================================================
void CSpawnPoint::SetVec3Position(const D3DXVECTOR3& rPos)
{
	m_pos = rPos;
}

//============================================================
//	�ʒu�擾
//============================================================
D3DXVECTOR3 CSpawnPoint::GetVec3Position(void) const
{
	return m_pos;
}

//============================================================
//	�����ݒ�
//============================================================
void CSpawnPoint::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	m_rot = rRot;
}

//============================================================
//	�����擾
//============================================================
D3DXVECTOR3 CSpawnPoint::GetVec3Rotation(void) const
{
	return m_rot;
}

//============================================================
//	�ԍ��擾
//============================================================
int CSpawnPoint::GetIndex(void) const
{
	return m_Idx;
}

//============================================================
//	��������
//============================================================
CSpawnPoint *CSpawnPoint::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �|�C���^��錾
	CSpawnPoint *pSpawnPoint = NULL;	// �����ʒu�����p

	if (pSpawnPoint == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pSpawnPoint = new CSpawnPoint;	// �����ʒu
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pSpawnPoint != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �����ʒu�̏�����
		if (FAILED(pSpawnPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pSpawnPoint->SetVec3Position(rPos);

		// ������ݒ�
		pSpawnPoint->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pSpawnPoint;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Z�[�u�|�C���g�擾
//============================================================
CObject * CSpawnPoint::GetSavePoint(int Idx)
{
	for (int nCntPri = 0; nCntPri < MAX_PRIO; nCntPri++)
	{ // �D�揇�ʂ̑������J��Ԃ�

		// �|�C���^��錾
		CObject *pObjectTop = CObject::GetTop(nCntPri);	// �擪�I�u�W�F�N�g

		if (pObjectTop != NULL)
		{ // �擪�����݂���ꍇ

			// �|�C���^��錾
			CObject *pObjCheck = pObjectTop;	// �I�u�W�F�N�g�m�F�p

			while (pObjCheck != NULL)
			{ // �I�u�W�F�N�g���g�p����Ă���ꍇ�J��Ԃ�

				// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_SPAWNPOINT)
				{ // �I�u�W�F�N�g���x�����X�|�[���|�C���g�ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				if (pObjCheck->GetIndex() != Idx)
				{ // �ԍ����������̔ԍ��ł͂Ȃ��ꍇ

					// ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// ���݂̃I�u�W�F�N�g��Ԃ�
				return pObjCheck;
			}
		}
	}

	// nullptr��Ԃ�
	return nullptr;
}

//============================================================
//	�j������
//============================================================
void CSpawnPoint::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
