//<==============================================
//
//�A�C�e���X�|�[���|�C���g���� [ItemSpawnPoint.cpp]
//Author�Fkazuki watanebe
//
//<==============================================
#include "ItemSpawnPoint.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

//<********************************
//�ÓI�����o�֐��̐ݒ�
//<********************************
int CItemSpawnPoint::m_nNumIdx = 0;		// �X�|�[���ԍ��̌�
CItem *CItemSpawnPoint::m_pItem = nullptr;
D3DXVECTOR3 CItemSpawnPoint::m_rPos[MAX_SPAWNPOINT] = {};
int CItemSpawnPoint::m_nCnt = 0;

//<**************************************
//���O��`
//<**************************************
namespace ItemSpawn
{
	const int MAX_TIME = 250;	//�N�[���^�C���̍ő�l
}

//<==================================
//�R���X�g���N�^
//<==================================
CItemSpawnPoint::CItemSpawnPoint() : CObject(CObject::LABEL_ITEMPOINT, 0), m_nIdx(m_nNumIdx)
{
	//�l�̏�����
	m_nRand = 0;

	//�ԍ������Z
	m_nNumIdx++;
}
//<==================================
//�f�X�g���N�^(��ɐÓI�����o�ϐ����I��������̂Ɏg��)
//<==================================
CItemSpawnPoint::~CItemSpawnPoint()
{
	//�ԍ������Z
	m_nNumIdx--;

	//���g����������
	if (m_nCnt != 0)
	{
		m_nCnt = 0;
	}
	
	//��������������Ă��Ȃ����
	if (m_rPos[m_nNumIdx] != VEC3_ZERO)
	{
		//�ʒu�̏�����
		m_rPos[m_nNumIdx] = VEC3_ZERO;
	}
}
//<==================================
//������
//<==================================
HRESULT CItemSpawnPoint::Init(void) 
{
	//������������
	m_nRand = 0;
	
	return S_OK;
}		
//<==================================
//�I������
//<==================================
void CItemSpawnPoint::Uninit(void) 
{
	Release();

	//���g�`�F�b�N
	if (m_pItem != nullptr)
	{
		m_pItem->Uninit();
		m_pItem = nullptr;
	}
}			
//<=======================================
//�A�C�e���X�|�[���|�C���g�̐�������
//<=======================================
CItemSpawnPoint *CItemSpawnPoint::Create(const D3DXVECTOR3& rPos)
{
	//�C���X�^���X�����p�̃I�u�W�F�N�g
	CItemSpawnPoint *pItemSpawnPoint = new CItemSpawnPoint;

	//nullptr�ɂȂ��Ă��Ȃ����Ƃ��m�F����
	assert(pItemSpawnPoint != nullptr);

	//����������
	if (SUCCEEDED(pItemSpawnPoint->Init()))
	{
		// �ʒu��ݒ�
		pItemSpawnPoint->SetVec3Position(rPos);

		//�ʒu���擾���Ă���
		m_rPos[pItemSpawnPoint->m_nIdx] = pItemSpawnPoint->GetVec3Position();

		//���̃|�C���^��Ԃ�
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
//�X�V����
//<=======================================
void CItemSpawnPoint::Update(void)
{
	//�����Q�[�����[�h��������
	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::EMode::MODE_GAME)
	{
		//�������̃A�C�e����nullptr�ł͂Ȃ�+�R���W�������������Ă����ꍇ
		if (m_pItem != nullptr
			&&m_pItem->Collision() == true)
		{
			m_pItem->Uninit();
			m_pItem =nullptr;
		}

		//�A�C�e�����Ȃ����
		if (m_pItem == nullptr)
		{
			//���Z���Ă���
			m_nCnt++;

			//�����K��l�𒴂��Ă����
			if (m_nCnt >= ItemSpawn::MAX_TIME)
			{
				//���肷��
				m_nRand = rand() % m_nNumIdx + 0;

				//�����������s��(�ԍ�����ʒu���擾���Ă���)
				m_pItem = CItem::Create(m_rPos[m_nRand]);

				//����������
				m_nCnt = 0;
			}
		}
		//��������
		else if (m_pItem != nullptr)
		{
			//�������Ȃ�
		}
	}
	
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