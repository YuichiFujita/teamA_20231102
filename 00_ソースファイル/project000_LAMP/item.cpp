//============================================================
//
//	�A�C�e���X�|�[������ [item.h]
//	Author�F�X��x��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "item.h"
#include "player.h"
#include "input.h"
#include "scene.h"
#include "collision.h"
#include "orbitalParticle.h"
#include "ItemSpawnPoint.h"
//============================================================
// �ÓI�����o�ϐ�
//============================================================
const char *CItem::mc_apModelFile[TYPE::TYPE_MAX] =
{
	"data\\MODEL\\PLAYER\\02_head.x",				//�񕜃A�C�e��
	"data\\MODEL\\OBSTACLE\\Castle-Fence000.x",		//�U���̓A�b�v
	"data\\MODEL\\OBSTACLE\\Castle-Fence001.x",		//�m�b�N�o�b�N����
	"data\\MODEL\\OBSTACLE\\Conifer000.x",			//�X�[�p�[�A�[�}�[
	"data\\MODEL\\OBSTACLE\\Fountain000.x",			//����S��
	"data\\MODEL\\OBSTACLE\\Hardwood000.x",			//���蔲����
	"data\\MODEL\\OBSTACLE\\Jar000.x",				//�_�u����
	"data\\MODEL\\OBSTACLE\\Jar001.x",				//�R���Ă��鍽
};

//<***************************************************
//���O�錾
//<***************************************************
namespace
{
	const float RADIUS = 50.0f;	//���a
}

//============================================================
// �R���X�g���N�^
//============================================================
CItem::CItem()
{
	//�l�̏�����
	m_nType = TYPE::TYPE_HEAL;
	SetLabel(LABEL_EFFECT);
}

//============================================================
// �f�X�g���N�^
//============================================================
CItem::~CItem()
{

}

//============================================================
// ������
//============================================================
HRESULT CItem::Init(void)
{
	//���o����(�R���Ă��鍽�`�񕜃A�C�e���܂Œ��o)
	m_nType = rand() % TYPE::TYPE_BURNING_FRAIL + TYPE::TYPE_HEAL;

	//���������s��
	CObjectModel::Init();

	return S_OK;
}
//============================================================
// �I��
//============================================================
void CItem::Uninit(void)
{
	// �p�����̏I��
	CObjectModel::Uninit();
}

//============================================================
// �X�V
//============================================================
void CItem::Update(void)
{
	Collision();
}
//============================================================
// �`��
//============================================================
void CItem::Draw(void)
{
	// �p�����̕`��
	CObjectModel::Draw();
}

//============================================================
// �����蔻��
//============================================================
bool CItem::Collision()
{
	bool bHit = false;

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// CScene�̃v���C���[�擾
		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);

		if (pPlayer != nullptr)
		{
			//�v���C���[�̈ʒu�擾
			D3DXVECTOR3 playerPos = pPlayer->GetVec3Position();
			//�v���C���[�̔��a�擾
			float fPlayerRadius = pPlayer->GetRadius();

			//�A�C�e���̈ʒu�擾
			D3DXVECTOR3 itemPos = GetVec3Position();

			//�A�C�e���̔��a�擾
			float fItemRadisu = GetRadius();

			//�~���̓����蔻��
			if (collision::Circle2D(playerPos, itemPos, fPlayerRadius, fItemRadisu) == true)
			{
				pPlayer->Hit();
				CorbitalParticle::Create(GetVec3Position(), D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f), VEC3_ZERO, VEC3_ZERO, VEC3_ZERO, 6, 800, 60, 60, 150, 1.0f, 0.85f);
				bHit = true;
				break;
			}
		}
	}
	return bHit;
}

//============================================================
// ����
//============================================================
CItem *CItem::Create(D3DXVECTOR3 pos)
{
	CItem *pItem = nullptr;

	if (pItem == nullptr)
	{// �C���X�^���X����
		pItem = new CItem;
	}

	if (pItem != nullptr)
	{
		if (FAILED(pItem->Init()))
		{
			//pItem��nullptr�ɂ���
			delete pItem;
			pItem = nullptr;

			//	nullptr��Ԃ�
			return nullptr;
		}

		// �ʒu�ݒ�
		pItem->SetVec3Position(pos);

		//�����_���ŃA�C�e���𐶐�����
		pItem->BindModel("data\\MODEL\\OBSTACLE\\present.x");
	}

	// �|�C���^��Ԃ�
	return pItem;
}
