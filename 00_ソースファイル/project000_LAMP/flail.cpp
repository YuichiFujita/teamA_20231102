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
#include "retentionManager.h"
#include "stage.h"
#include "liquid.h"
#include "scrollMeshField.h"
#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 3;	// �t���C���̗D�揇��

	const float	RADIUS		= 50.0f;	// ���a
	const int	HIT_DAMAGE	= 10;		// �_���[�W��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CFlail::mc_apModelFileFlail[] =	// ���f���萔(�t���C��)
{
	"data\\MODEL\\PLAYER\\ironBall_Resize.x",	// �S��
	"data\\MODEL\\OBSTACLE\\obstacle018.x",	// �v���n�u����
};

const char *CFlail::mc_apModelFileChain[] =	// ���f���萔(��)
{
	"data\\MODEL\\PLAYER\\14_chain.x",	// ��
};

//************************************************************
//	�q�N���X [CFlail] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFlail::CFlail() : CObjectModel(CObject::LABEL_NONE, PRIORITY)
{
	memset(&m_chain[0], 0, sizeof(m_chain));	// ���f���̏��
	m_oldPos = VEC3_ZERO;
	m_move = VEC3_ZERO;
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

	BindModel(mc_apModelFileFlail[FLAIL_NORMAL]);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		// ���f���̐���
		m_chain[nCntChain].multiModel = CMultiModel::Create(VEC3_ZERO, VEC3_ZERO);

		// ���f��������
		m_chain[nCntChain].multiModel->BindModel(mc_apModelFileChain[CFlail::CHAIN_NORMAL]);
	}

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

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		// ���f���̏I��
		m_chain[nCntChain].multiModel->Uninit();
	}
}

//============================================================
//	�X�V����
//============================================================
void CFlail::Update(void)
{
	m_oldPos = GetVec3Position();
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);
	m_fLengthChain = 0.0f;

	// �p�x�C��
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	// �������鎞�̂݊p�x����
	if (player->GetCounterFlail() < flail::FLAIL_DROP)
	{
		m_fChainRot += (m_fChainRotMove - m_fChainRot) * 0.008f;
	}

	if (D3DXVec3Length(&m_move) > 0.0f && D3DXVec3Length(&m_move) < 5.0f)
	{
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}
	
	// �p�x�C��
	useful::NormalizeRot(m_fChainRot);
	useful::NormalizeRot(m_fChainRotMove);

	if (m_nPlayerID == 0)
	{
		/*CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "���ڕW�p�x %f\n", m_fChainRotMove);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "���p�x %f\n", m_fChainRot);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "������ %f\n", m_fLengthChain);
		CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "[�J�E���^�[]�F%d\n", player->GetCounterFlail());*/
	}

	// �I�u�W�F�N�g���f���̍X�V
	CObjectModel::Update();

	// ���̍X�V
	UpdateChain();
}

//============================================================
//	�t���C���ʒu�̍X�V����
//============================================================
void CFlail::UpdateFlailPos(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	int partsNum = flail::FLAIL_NUM - 1;
	D3DXMATRIX partsMtx = m_chain[partsNum].multiModel->GetMtxWorld();
	D3DXVECTOR3 partsPos = D3DXVECTOR3(partsMtx._41, partsMtx._42, partsMtx._43);
	SetVec3PosOrg(partsPos);

	// �p�x�ƒ�������S���̈ʒu����
	D3DXVECTOR3 pos = GetVec3Position();

	if (D3DXVec3Length(&m_move) == 0.0f)
	{
		if (player->GetCounterFlail() < flail::FLAIL_DEF)
		{
			if (pos.y > -13.0f)
			{
				pos.y -= 0.5f;
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

	if (player->GetCounterFlail() < flail::FLAIL_DEF)
	{
		// �ړ��ʌ���
		m_move.x += (0.0f - m_move.x) * 0.15f;
		m_move.z += (0.0f - m_move.z) * 0.15f;
	}
	else
	{
		// �ړ��ʌ���
		m_move.x += (0.0f - m_move.x) * 0.15f;
		m_move.z += (0.0f - m_move.z) * 0.15f;
	}

	pos.x = m_posOrg.x + (sinf(m_fChainRot) * 1.0f);
	pos.z = m_posOrg.z + (cosf(m_fChainRot) * 1.0f);

	if (player->GetCounterFlail() < flail::FLAIL_DEF || player->GetCounterFlail() == flail::FLAIL_THROW)
	{
		Collision(pos);
	}

	SetVec3Position(pos);
}

//============================================================
//	���̍X�V����
//============================================================
void CFlail::UpdateChain(void)
{
	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		D3DXVECTOR3 pos, rot;
		m_chain[nCntChain].rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		pos = m_chain[nCntChain].multiModel->GetVec3Position();
		rot = m_chain[nCntChain].multiModel->GetVec3Rotation();

		if (nCntChain == 0)
		{
			rot.x = 0.0f;
			rot.y = m_fChainRot;
			rot.z = 0.0f;

			pos.x = -1.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
		}
		else
		{
			int IDParent = nCntChain - 1;

			if (nCntChain == 1)
			{
				rot.y = m_chain[IDParent].rotOld.y - m_chain[IDParent].multiModel->GetVec3Rotation().y;
			}
			else
			{
				rot.y = m_chain[IDParent].rotOld.y * 0.80f;
			}

			if (player->GetCounterFlail() == flail::FLAIL_THROW)
			{
				if ((m_chain[IDParent].multiModel->GetVec3Position().x >= 4.0f) || nCntChain == 1)
				{
					pos.x += 3.9f;

					if (pos.x > 4.0f)
					{
						pos.x = 4.0f;
					}
				}
			}

			if (player->GetCounterFlail() < flail::FLAIL_DROP)
			{
				if (m_chain[IDParent].multiModel->GetVec3Position().x <= 0.0f)
				{
					pos.x -= -0.04f * player->GetCounterFlail();
					
					if (pos.x < 0.0f)
					{
						pos.x = 0.0f;
					}
				}
			}

			if (m_nPlayerID == 0)
			{
				//CManager::GetInstance()->GetDebugProc()->Print(CDebugProc::POINT_LEFT, "���ʒu[%d] %f %f %f\n", nCntChain, rot.x, rot.y, rot.z);
			}
		}

		m_chain[nCntChain].multiModel->SetVec3Position(pos);
		m_chain[nCntChain].multiModel->SetVec3Rotation(rot);

		if (nCntChain != 0)
		{
			m_fLengthChain += pos.x;
		}

		// ���f���̍X�V
		m_chain[nCntChain].multiModel->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CFlail::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(m_nPlayerID);

	D3DXMATRIX mtx;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtx);

	mtx._41 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_41;
	mtx._42 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_42;
	mtx._43 = player->GetMultiModel(CPlayer::MODEL_STICK)->GetPtrMtxWorld()->_43;

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		int IDParent = nCntChain - 1;
		D3DXVECTOR3 rotOld = m_chain[nCntChain].multiModel->GetVec3Rotation();
		D3DXVECTOR3 rotNow = m_chain[nCntChain].multiModel->GetVec3Rotation();

		if (nCntChain != 0)
		{
			if (nCntChain % 2 == 0)
			{
				rotNow.x = -D3DX_PI * 0.5f;
				rotNow.y = 0.0f;
				rotNow.z = 0.0f;
			}
			else if (nCntChain % 2 == 1)
			{
				rotNow.x = D3DX_PI * 0.5f;
				rotNow.y = rotOld.y;
				rotNow.z = 0.0f;
			}

			m_chain[nCntChain].multiModel->SetVec3Rotation(rotNow);
		}

		if (m_chain[nCntChain].multiModel->GetVec3Position().x == 0.0f)
		{
			// �ϐ���錾
			D3DXMATRIX   mtxScale, mtxRot, mtxTrans, *mtxOrg;	// �v�Z�p�}�g���b�N�X
			D3DXMATRIX   mtxParent;	// �e�̃}�g���b�N�X
			D3DXVECTOR3 pos, rot, scale;

			pos = m_chain[nCntChain].multiModel->GetVec3Position();
			rot = m_chain[nCntChain].multiModel->GetVec3Rotation();
			scale = m_chain[nCntChain].multiModel->GetVec3Scaling();
			mtxOrg = m_chain[nCntChain].multiModel->GetPtrMtxWorld();

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(mtxOrg);

			// �g�嗦�𔽉f
			D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxScale);

			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxTrans);

			// �e�}�g���b�N�X��ݒ�
			if (nCntChain == 0)
			{ // �e�����݂��Ȃ��ꍇ

			  // ���݂̃}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// �ݒ肳�ꂽ�ŐV�̃}�g���b�N�X (���̂̃}�g���b�N�X)
			}
			else
			{ // �e�����݂���ꍇ

			  // �e�̃}�g���b�N�X��ݒ�
				mtxParent = m_chain[IDParent].multiModel->GetMtxWorld();
			}

			// ���[���h�}�g���b�N�X�Ɛe�}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply(mtxOrg, mtxOrg, &mtxParent);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, mtxOrg);
		}
		else
		{
			// ���f���̕`��
			m_chain[nCntChain].multiModel->Draw();
		}

		if (nCntChain != 0)
		{
			m_chain[nCntChain].multiModel->SetVec3Rotation(rotOld);
		}
	}

	UpdateFlailPos();

	// �I�u�W�F�N�g���f���̕`��
	CObjectModel::Draw();
}

//============================================================
//	��������
//============================================================
CFlail *CFlail::Create
( // ����
	const CPlayer& rPlayer,		// �v���C���[
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

		// ���̐e��ݒ�
		pModelUI->BindParent(rPlayer);

		// �m�ۂ����A�h���X��Ԃ�
		return pModelUI;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�����蔻�菈��
//============================================================
void CFlail::Collision(D3DXVECTOR3& rPos)
{
	for (int nCntPlayer = 0; nCntPlayer < CManager::GetInstance()->GetRetentionManager()->GetNumPlayer(); nCntPlayer++)
	{
		CPlayer *player = CManager::GetInstance()->GetScene()->GetPlayer(nCntPlayer);

		if (player != NULL && nCntPlayer != m_nPlayerID)
		{
			D3DXVECTOR3 vec;
			float length;

			// �v���C���[�ƃt���C���̃x�N�g�������߂�
			vec = player->GetVec3Position() - GetVec3Position();
			vec.y = 0.0f;	// Y�͖���

			// ���������߂�
			length = D3DXVec3Length(&vec);

			// ������уx�N�g���𐳋K��
			D3DXVec3Normalize(&vec, &vec);
			
			if (length < RADIUS + player->GetRadius())
			{
				// �_���[�W�q�b�g����
				player->HitKnockBack(HIT_DAMAGE, vec);
			}
		}
	}

	CollisionGround(CPlayer::AXIS_X, rPos);
	CollisionBlock(CPlayer::AXIS_X, rPos);

	CollisionGround(CPlayer::AXIS_Y, rPos);
	CollisionBlock(CPlayer::AXIS_Y, rPos);

	CollisionGround(CPlayer::AXIS_Z, rPos);
	CollisionBlock(CPlayer::AXIS_Z, rPos);
}

void CFlail::CollisionGround(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// �v���C���[�ő�傫��
	bool bMin = false;	// �s�̕����̔����
	bool bMax = false;	// ���̕����̔����
	bool bHit = false;	// ���n�̔�����

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

			  // �ϐ���錾
				D3DXVECTOR3 posGround = VEC3_ZERO;		// �n�Ոʒu
				D3DXVECTOR3 rotGround = VEC3_ZERO;		// �n�Ռ���
				D3DXVECTOR3 sizeMinGround = VEC3_ZERO;	// �n�Սŏ��傫��
				D3DXVECTOR3 sizeMaxGround = VEC3_ZERO;	// �n�Սő�傫��

														// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_GROUND)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

				  // ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �n�Ղ̈ʒu��ݒ�
				posGround = pObjCheck->GetVec3Position();

				// �n�Ղ̌�����ݒ�
				rotGround = pObjCheck->GetVec3Rotation();

				// �n�Ղ̍ŏ��̑傫����ݒ�
				sizeMinGround = pObjCheck->GetVec3Sizing();
				sizeMinGround.y *= 2.0f;	// �c�̑傫����{�ɂ���

											// �n�Ղ̍ő�̑傫����ݒ�
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				sizeMaxGround.y = 0.0f;		// �c�̑傫����������

				switch (axis)
				{ // ���莲���Ƃ̏���
				case CPlayer::AXIS_X:	// X��

								// X���̏Փ˔���
					collision::ResponseSingleX
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				case CPlayer::AXIS_Y:	// Y��

								// Y���̏Փ˔���
					collision::ResponseSingleY
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						true,			// X����
						true,			// Z����
						&bMin,			// ������̔���
						&bMax			// �ォ��̔���
					);

					if (bMax)
					{ // �ォ�瓖�����Ă����ꍇ

					  // ���n���Ă���󋵂ɂ���
						bHit = true;
					}

					break;

				case CPlayer::AXIS_Z:	// Z��

								// Z���̏Փ˔���
					collision::ResponseSingleZ
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

void CFlail::CollisionBlock(const CPlayer::EAxis axis, D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	D3DXVECTOR3 sizeMinPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// �v���C���[�ŏ��傫��
	D3DXVECTOR3 sizeMaxPlayer = D3DXVECTOR3(8.0f, 8.0f, 8.0f);		// �v���C���[�ő�傫��
	bool bMin = false;	// �s�̕����̔����
	bool bMax = false;	// ���̕����̔����
	bool bHit = false;	// ���n�̔�����

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

			  // �ϐ���錾
				D3DXVECTOR3 posGround = VEC3_ZERO;		// �n�Ոʒu
				D3DXVECTOR3 rotGround = VEC3_ZERO;		// �n�Ռ���
				D3DXVECTOR3 sizeMinGround = VEC3_ZERO;	// �n�Սŏ��傫��
				D3DXVECTOR3 sizeMaxGround = VEC3_ZERO;	// �n�Սő�傫��

														// �|�C���^��錾
				CObject *pObjectNext = pObjCheck->GetNext();	// ���I�u�W�F�N�g

				if (pObjCheck->GetLabel() != CObject::LABEL_BLOCK)
				{ // �I�u�W�F�N�g���x�����n�Ղł͂Ȃ��ꍇ

				  // ���̃I�u�W�F�N�g�ւ̃|�C���^����
					pObjCheck = pObjectNext;

					// ���̌J��Ԃ��Ɉڍs
					continue;
				}

				// �n�Ղ̈ʒu��ݒ�
				posGround = pObjCheck->GetVec3Position();

				// �n�Ղ̌�����ݒ�
				rotGround = pObjCheck->GetVec3Rotation();

				// �n�Ղ̍ŏ��̑傫����ݒ�
				sizeMinGround = pObjCheck->GetVec3Sizing();
				sizeMinGround.y *= 2.0f;	// �c�̑傫����{�ɂ���

											// �n�Ղ̍ő�̑傫����ݒ�
				sizeMaxGround = pObjCheck->GetVec3Sizing();
				sizeMaxGround.y = 0.0f;		// �c�̑傫����������

				switch (axis)
				{ // ���莲���Ƃ̏���
				case CPlayer::AXIS_X:	// X��

										// X���̏Փ˔���
					collision::ResponseSingleX
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				case CPlayer::AXIS_Y:	// Y��

										// Y���̏Փ˔���
					collision::ResponseSingleY
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move,		// �ړ���
						true,			// X����
						true,			// Z����
						&bMin,			// ������̔���
						&bMax			// �ォ��̔���
					);

					if (bMax)
					{ // �ォ�瓖�����Ă����ꍇ

					  // ���n���Ă���󋵂ɂ���
						bHit = true;
					}

					break;

				case CPlayer::AXIS_Z:	// Z��

										// Z���̏Փ˔���
					collision::ResponseSingleZ
					( // ����
						rPos,			// ����ʒu
						m_oldPos,		// ����ߋ��ʒu
						posGround,		// ����ڕW�ʒu
						sizeMaxPlayer,	// ����T�C�Y(�E�E��E��)
						sizeMinPlayer,	// ����T�C�Y(���E���E�O)
						sizeMaxGround,	// ����ڕW�T�C�Y(�E�E��E��)
						sizeMinGround,	// ����ڕW�T�C�Y(���E���E�O)
						&m_move			// �ړ���
					);

					break;

				default:	// ��O����
					assert(false);
					break;
				}

				// ���̃I�u�W�F�N�g�ւ̃|�C���^����
				pObjCheck = pObjectNext;
			}
		}
	}
}

//============================================================
//	�e�q�t���̐ݒ菈��
//============================================================
void CFlail::BindParent(const CPlayer& rPlayer)
{
	int parentID = 0;

	for (int nCntChain = 0; nCntChain < flail::FLAIL_NUM; nCntChain++)
	{
		if (nCntChain == 0)
		{
			// �e��ݒ�
			m_chain[nCntChain].multiModel->SetParentModel(NULL);
		}
		else
		{
			parentID = nCntChain - 1;

			// �e��ݒ�
			m_chain[nCntChain].multiModel->SetParentModel(m_chain[parentID].multiModel);
		}
	}
	
	parentID = flail::FLAIL_NUM - 1;
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
void CFlail::SetMove(const D3DXVECTOR3& rMove)
{
	// �����̈ʒu��ݒ�
	m_move = rMove;
}

//============================================================
//	�ړ��ʂ̎擾����
//============================================================
D3DXVECTOR3 CFlail::GetMove(void)
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