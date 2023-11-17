//============================================================
//
//	�t���C������ [flail.cpp]
//	Author�F������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "flail.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "model.h"
#include "useful.h"
#include "player.h"
#include "retentionManager.h"
#include "stage.h"
#include "liquid.h"
#include "scrollMeshField.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MODEL_UI_PRIO	(14)	// ���f��UI�\���̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CFlail::mc_apModelFile[] =	// ���f���萔
{
	"data\\MODEL\\PLAYER\\15_ironBall.x",	// �S��
	"data\\MODEL\\OBSTACLE\\obstacle018.x",	// �v���n�u����
};

//************************************************************
//	�q�N���X [CFlail] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, MODEL_UI_PRIO)
{
	m_move = 0.0f;
	m_fChainRot = 0.0f;
	m_fLengthChain = 0.0f;
	m_fChainRotMove = 0.0f;
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFlail::~CFlail()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFlail::Init(void)
{
	// �I�u�W�F�N�g���f���̏�����
	if (FAILED(CObjectModel::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	BindModel(mc_apModelFile[CFlail::MODEL_FLAIL]);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFlail::Uninit(void)
{
	// �I�u�W�F�N�g���f���̏I��
	CObjectModel::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFlail::Update(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	// ���̒����Ɉړ��ʑ��
	m_fLengthChain += m_move;

	if (player->GetCounterFlail() < 0)
	{
		// �ړ��ʌ���
		m_move += (0.0f - m_move) * 0.01f;
	}
	else
	{
		// �ړ��ʌ���
		m_move += (0.0f - m_move) * 0.08f;
	}

	// �p�x�C��
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	// �������鎞�̂݊p�x����
	if (m_move < 0.0f)
	{
		m_fChainRot += (m_fChainRotMove - m_fChainRot) * 0.008f;
	}

	if (m_move > 0.0f && m_move < 5.0f)
	{
		m_move = 0.0f;
	}
	
	// �p�x�C��
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "���ڕW�p�x %f\n", m_fChainRotMove);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "���p�x %f\n", m_fChainRot);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "������ %f\n", m_fLengthChain);
	CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "[���_�ʒu]�F%f %f %f\n", m_posOrg.x, m_posOrg.y, m_posOrg.z);

	// ���̒����𒴂�����~�߂�
	if (m_fLengthChain > 1000.0f)
	{
		m_fLengthChain = 1000.0f;
	}

	// �p�x�ƒ�������S���̈ʒu����
	D3DXVECTOR3 pos = GetVec3Position();

	pos.x = m_posOrg.x + (sinf(m_fChainRot) * m_fLengthChain);

	if (m_move == 0.0f)
	{
		if (player->GetCounterFlail() < 0)
		{
			if (pos.y > -13.0f)
			{
				pos.y -= 5.0f;
			}
			else
			{
				pos.y = CManager::GetInstance()->GetScene()->GetStage()->GetLiquid()->GetScrollMeshField(0)->GetPositionHeight(pos);
			}
		}
		else
		{
			pos.y = m_posOrg.y;
		}
	}

	pos.z = m_posOrg.z + (cosf(m_fChainRot) * m_fLengthChain);

	Collision();

	SetVec3Position(pos);

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CFlail::Draw(void)
{
	// �ϐ���錾
	D3DVIEWPORT9 viewportDef;	// �J�����̃r���[�|�[�g�ۑ��p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���݂̃r���[�|�[�g���擾
	pDevice->GetViewport(&viewportDef);

	// �J�����̐ݒ�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();

	// �J�����̐ݒ�����ɖ߂�
	CManager::GetInstance()->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

	// �r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&viewportDef);
}

//============================================================
//	��������
//============================================================
CFlail *CFlail::Create
( // ����
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const D3DXVECTOR3& rScale	// �傫��
)
{
	// �|�C���^��錾
	CFlail *pModelUI = NULL;	// ���f��UI�����p

	if (pModelUI == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pModelUI = new CFlail;	// ���f��UI
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pModelUI != NULL)
	{ // �g�p����Ă���ꍇ
		
		// ���f��UI�̏�����
		if (FAILED(pModelUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pModelUI;
			pModelUI = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pModelUI->SetVec3Position(rPos);

		// ������ݒ�
		pModelUI->SetVec3Rotation(rRot);

		// �g�嗦��ݒ�
		pModelUI->SetVec3Scaling(rScale);

		// �m�ۂ����A�h���X��Ԃ�
		return pModelUI;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����蔻�菈��
//============================================================
void CFlail::Collision(void)
{
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPlayerID)
		{
			D3DXVECTOR3 vec;
			float length;

			vec = player->GetVec3Position() - GetVec3Position();
			vec.y = 0.0f;
			length = D3DXVec3Length(&vec);
			
			if (length < GetModelData().fRadius + player->GetRadius())
			{
				D3DXVECTOR3 pos = player->GetVec3Position();

				player->SetVec3Position(D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z));
			}
		}
	}
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CFlail::SetVec3PosOrg(const D3DXVECTOR3& rPosOrg)
{
	// �����̈ʒu��ݒ�
	m_posOrg = rPosOrg;
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
D3DXVECTOR3 CFlail::GetVec3PosOrg(void)
{
	// �ʒu��Ԃ�
	return m_posOrg;
}

//============================================================
//	�v���C���[�ԍ��̐ݒ菈��
//============================================================
void CFlail::SetPlayerID(const int& rPlayerID)
{
	// �����̃v���C���[�ԍ���ݒ�
	m_nPlayerID = rPlayerID;
}

//============================================================
//	�v���C���[�ԍ��̐ݒ菈��
//============================================================
int CFlail::GetPlayerID(void)
{
	// �v���C���[�ԍ���Ԃ�
	return m_nPlayerID;
}

//============================================================
//	�ړ��ʂ̐ݒ菈��
//============================================================
void CFlail::SetMove(const float& rMove)
{
	// �����̈ʒu��ݒ�
	m_move = rMove;
}

//============================================================
//	�ړ��ʂ̎擾����
//============================================================
float CFlail::GetMove(void)
{
	// �ʒu��Ԃ�
	return m_move;
}

//============================================================
//	�p�x�̐ݒ菈��
//============================================================
void CFlail::SetChainRot(const float& rChainRot)
{
	// �����̊p�x��ݒ�
	m_fChainRot = rChainRot;
}

//============================================================
//	�p�x�̎擾����
//============================================================
float CFlail::GetChainRot(void)
{
	// �p�x��Ԃ�
	return m_fChainRot;
}

//============================================================
//	�ڕW�p�x�̐ݒ菈��
//============================================================
void CFlail::SetChainRotMove(const float& rChainRotMove)
{
	// �����̖ڕW�p�x��ݒ�
	m_fChainRotMove = rChainRotMove;
}

//============================================================
//	�ڕW�p�x�̎擾����
//============================================================
float CFlail::GetChainRotMove(void)
{
	// �ڕW�p�x��Ԃ�
	return m_fChainRotMove;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CFlail::SetLengthChain(const float& rLengthChain)
{
	// �����̒�����ݒ�
	m_fLengthChain = rLengthChain;
}

//============================================================
//	�����̎擾����
//============================================================
float CFlail::GetLengthChain(void)
{
	// ������Ԃ�
	return m_fLengthChain;
}