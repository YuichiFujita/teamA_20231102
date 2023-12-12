//<==============================================
//
//�A�C�e���X�|�[���|�C���g���� [ItemSpawnPoint.cpp]
//Author�Fkazuki watanebe
//
//<==============================================
#include "ItemSpawnPoint.h"
#include "manager.h"
#include "renderer.h"

//<********************************
//�ÓI�����o�֐��̐ݒ�
//<********************************
int CItemSpawnPoint::m_nNumIdx = 0;		// �X�|�[���ԍ��̌�

//<==================================
//�R���X�g���N�^
//<==================================
CItemSpawnPoint::CItemSpawnPoint() : CObject(CObject::LABEL_ITEMPOINT, 0), m_nIdx(m_nNumIdx)
{
	//�l�̏�����
	m_pos = VEC3_ZERO;

	//�ԍ������Z
	m_nNumIdx++;
}
//<==================================
//�f�X�g���N�^
//<==================================
CItemSpawnPoint::~CItemSpawnPoint()
{
	//�ԍ������Z
	m_nNumIdx--;
}
//<==================================
//������
//<==================================
HRESULT CItemSpawnPoint::Init(void) 
{
	//�l�̏�����
	m_pos = VEC3_ZERO;

	return S_OK;
}		
//<==================================
//�I������
//<==================================
void CItemSpawnPoint::Uninit(void) 
{
	Release();
}			
//<=======================================
//�A�C�e���X�|�[���|�C���g�̐�������
//<=======================================
CItemSpawnPoint *CItemSpawnPoint::Create(const D3DXVECTOR3& rPos)
{
	CItemSpawnPoint *pItemSpawnPoint = new CItemSpawnPoint;

	assert(pItemSpawnPoint != nullptr);

	//����������
	if (SUCCEEDED(pItemSpawnPoint->Init()))
	{
		// �ʒu��ݒ�
		pItemSpawnPoint->SetVec3Position(rPos);

		return pItemSpawnPoint;
	}
	//���s������
	else
	{
		return nullptr;
	}

	return nullptr;
}
//<=======================================
//�ԍ��̐ݒ�
//<=======================================
CObject *CItemSpawnPoint::GetPoint(int Idx)
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