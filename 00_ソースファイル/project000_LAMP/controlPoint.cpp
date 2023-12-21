//============================================================
//
//	����ʒu���� [controlPoint.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "controlPoint.h"
#include "manager.h"
#include "renderer.h"
#include "objectBillboard.h"
#include "player.h"
#include "collision.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "retentionManager.h"
#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY = 4;	// �͈̗͂D�揇��
	const int	WAIT_FRAME = 8;	// �J�ڂ̗]�C�t���[��

	namespace tutorial
	{
		const int	PRIORITY	= 9;		// ���b�X���\���̗D�揇��
		const float	MIN_SCALE	= 1.0f;		// �ŏ��̊g�嗦
		const float	MAX_SCALE	= 1.9f;		// �ő�̊g�嗦
		const float	REV_SCALE	= 0.45f;	// �g�嗦�̕␳�W��

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 500.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 300.0f, 0.0f);		// ��

		const D3DXCOLOR	COL_NORMAL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �ʏ�J���[
		const D3DXCOLOR	COL_LAST	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// �ŏI�J���[
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CControlPoint::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\field_font001.png",	// �n�ʃe�N�X�`��
	"data\\TEXTURE\\tutorial_next.png",	// ���e�N�X�`��
	"data\\TEXTURE\\tutorial_prev.png",	// �O�e�N�X�`��
};

const char *CControlPoint::mc_apLessonFile[] =	// ���b�X���e�N�X�`���萔
{
	"data\\TEXTURE\\lesson000.png",	// �ړ��e�N�X�`��
	"data\\TEXTURE\\lesson001.png",	// �_�b�V���e�N�X�`��
	"data\\TEXTURE\\lesson002.png",	// �U���e�N�X�`��
	"data\\TEXTURE\\lesson003.png",	// �U����ރe�N�X�`��
	"data\\TEXTURE\\lesson004.png",	// �t�b�N�V���b�g�e�N�X�`��
};

//************************************************************
//	�q�N���X [CControlPoint] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CControlPoint::CControlPoint() : CObject3D(CObject::LABEL_NEXT, PRIORITY)
{
	// �����o�ϐ����N���A
	m_pTutorial = NULL;		// �`���[�g���A���\�����
	m_pNext = NULL;			// ������̕\�����
	m_pPrev = NULL;			// �O����̕\�����
	m_nLesson = LESSON_00;	// ���b�X��
	m_fScale = 0.0f;		// �g�嗦
	m_fDestScale = 0.0f;	// �ڕW�g�嗦
}

//============================================================
//	�f�X�g���N�^
//============================================================
CControlPoint::~CControlPoint()
{

}

//============================================================
//	����������
//============================================================
HRESULT CControlPoint::Init(void)
{
	// �����o�ϐ���������
	m_pTutorial = NULL;		// �`���[�g���A���\�����
	m_pNext = NULL;			// ������̕\�����
	m_pPrev = NULL;			// �O����̕\�����
	m_nLesson = LESSON_00;	// ���b�X��
	m_fScale = 1.0f;		// �g�嗦
	m_fDestScale = 1.0f;	// �ڕW�g�嗦

	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	BindTexture(mc_apTextureFile[TEXTURE_FIELD]);

	// �`���[�g���A���\���̐���
	m_pTutorial = CObjectBillboard::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		tutorial::SIZE	// �傫��
	);

	// �e�N�X�`����o�^�E����
	m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);

	// ���x����ݒ�
	m_pTutorial->SetLabel(LABEL_UI);

	// �D�揇�ʂ�ݒ�
	m_pTutorial->SetPriority(tutorial::PRIORITY);

	// �O����\���̐���
	m_pPrev = CObjectBillboard::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		tutorial::SIZE	// �傫��
	);

	// �e�N�X�`����o�^�E����
	m_pPrev->BindTexture(mc_apTextureFile[TEXTURE_PREV]);

	// ���x����ݒ�
	m_pPrev->SetLabel(LABEL_UI);

	// �D�揇�ʂ�ݒ�
	m_pPrev->SetPriority(tutorial::PRIORITY);

	// ������\���̐���
	m_pNext = CObjectBillboard::Create
	( // ����
		VEC3_ZERO,		// �ʒu
		tutorial::SIZE	// �傫��
	);

	// �e�N�X�`����o�^�E����
	m_pNext->BindTexture(mc_apTextureFile[TEXTURE_NEXT]);

	// ���x����ݒ�
	m_pNext->SetLabel(LABEL_UI);

	// �D�揇�ʂ�ݒ�
	m_pNext->SetPriority(tutorial::PRIORITY);

	// ���g�̉e�̕`���OFF�E�e�̉f�荞�݂�ON�ɂ���
	SetEnableDepthShadow(true);
	SetEnableZTex(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CControlPoint::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CControlPoint::Update(void)
{
	if (CManager::GetInstance()->GetMode() != CScene::MODE_GAME)
	{ // �Q�[�����[�h�ł͂Ȃ��ꍇ

		return;
	}

	// �`���[�g���A���̑��씻��
	if (Collision())
	{ // �N��������͈͓̔��ɂ����ꍇ

		// �\�����g��
		m_fDestScale = tutorial::MAX_SCALE;

		// ����̕`���ON�ɂ���
		m_pPrev->SetEnableDraw(true);	// �O����\��
		m_pNext->SetEnableDraw(true);	// ������\��
	}
	else
	{ // �N�����Ȃ������ꍇ

		// �\�����k��
		m_fDestScale = tutorial::MIN_SCALE;

		// ����̕`���OFF�ɂ���
		m_pPrev->SetEnableDraw(false);	// �O����\��
		m_pNext->SetEnableDraw(false);	// ������\��
	}

	// ���b�X���̍X�V
	UpdateLesson();

	// �I�u�W�F�N�g3D�̍X�V
	CObject3D::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CControlPoint::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw();
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CControlPoint::SetVec3Position(const D3DXVECTOR3 & rPos)
{
	// ���g�̈ʒu��ݒ�
	CObject3D::SetVec3Position(rPos);

	// �`���[�g���A���\���̈ʒu��ݒ�
	m_pTutorial->SetVec3Position(rPos + tutorial::SPACE);

	// �O����\���̈ʒu��ݒ�
	m_pPrev->SetVec3Position(rPos + tutorial::SPACE);

	// ������\���̈ʒu��ݒ�
	m_pNext->SetVec3Position(rPos + tutorial::SPACE);
}

//============================================================
//	��������
//============================================================
CControlPoint *CControlPoint::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize	// �傫��
)
{
	// �|�C���^��錾
	CControlPoint *pControlPoint = NULL;	// ����ʒu�����p

	if (pControlPoint == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pControlPoint = new CControlPoint;	// ����ʒu
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pControlPoint != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ����ʒu�̏�����
		if (FAILED(pControlPoint->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pControlPoint;
			pControlPoint = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �ʒu��ݒ�
		pControlPoint->SetVec3Position(rPos);

		// �傫����ݒ�
		pControlPoint->SetVec3Sizing(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pControlPoint;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�v���C���[�S���Ƃ̓����蔻��
//============================================================
bool CControlPoint::Collision(void)
{
	// �ϐ���錾
	bool bColl = false;	// �����

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{ // �v���C���[�̍ő吔���J��Ԃ�

		CPlayer *pPlayer = CScene::GetPlayer(nCntPlayer);	// �v���C���[���
		if (pPlayer != NULL)
		{ // �v���C���[���g�p����Ă���ꍇ

			if (pPlayer->GetState() == CPlayer::STATE_DEATH)
			{ // ���S���Ă����ꍇ

				continue;
			}

			float fPlayerRadius = pPlayer->GetRadius();	// �v���C���[���a

			// ��`����
			bool bHit = collision::Box2D
			( // ����
				pPlayer->GetVec3Position(),								// ����ʒu
				GetVec3Position(),										// ����ڕW�ʒu
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// ����T�C�Y(�E�E��E��)
				D3DXVECTOR3(fPlayerRadius, 0.0f, fPlayerRadius) * 0.5f,	// ����T�C�Y(���E���E�O)
				GetVec3Sizing() * 0.5f,									// ����ڕW�T�C�Y(�E�E��E��)
				GetVec3Sizing() * 0.5f									// ����ڕW�T�C�Y(���E���E�O)
			);
			if (bHit)
			{ // ������̏ꍇ

				// �`���[�g���A���̑���
				UpdateTutorial(nCntPlayer);

				// ���肪��������Ԃɂ���
				bColl = true;
			}
		}
	}

	// ����󋵂�Ԃ�
	return bColl;
}

//============================================================
//	�`���[�g���A���̑��쏈��
//============================================================
void CControlPoint::UpdateTutorial(const int nID)
{
	// �|�C���^��錾
	CRetentionManager *pRetention = CManager::GetInstance()->GetRetentionManager();	// �f�[�^�ۑ����
	CInputPad *pPad = CManager::GetInstance()->GetPad();	// �p�b�h

	if (pPad->IsTrigger(CInputPad::KEY_Y, nID))
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// ���b�X����i�߂�
		m_nLesson++;

		if (m_nLesson >= LESSON_MAX)
		{ // ���b�X�����ő�l�𒴂����ꍇ

			// ���b�X����␳ (�߂�)
			m_nLesson--;
		}
		else
		{ // ���b�X���������Ă��Ȃ��ꍇ

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}

		// �e�N�X�`����o�^�E����
		m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);
	}
	if (pPad->IsTrigger(CInputPad::KEY_X, nID))
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// ���b�X����߂�
		m_nLesson--;

		if (m_nLesson <= NONE_IDX)
		{ // ���b�X�����ŏ��l����������ꍇ

			// ���b�X����␳ (�i�߂�)
			m_nLesson++;
		}
		else
		{ // ���b�X����������Ă��Ȃ��ꍇ

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}

		// �e�N�X�`����o�^�E����
		m_pTutorial->BindTexture(mc_apLessonFile[m_nLesson]);
	}
}

//============================================================
//	���b�X���̍X�V����
//============================================================
void CControlPoint::UpdateLesson(void)
{
	// �ϐ���錾
	float fDiffScale = 0.0f;	// �����̊g�嗦

	// �g�嗦�̍������v�Z
	fDiffScale = m_fDestScale - m_fScale;

	// �g�嗦���v�Z
	m_fScale += fDiffScale * tutorial::REV_SCALE;

	// �`���[�g���A���\���̑傫����ݒ�
	m_pTutorial->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// �O����\���̑傫����ݒ�
	m_pPrev->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// ������\���̑傫����ݒ�
	m_pNext->SetVec3Sizing(tutorial::SIZE * m_fScale);

	// �O����\���̐F��ݒ�
	if (m_nLesson <= 0)
	{ // �O������ȏ�Ȃ��ꍇ

		m_pPrev->SetColor(tutorial::COL_LAST);
	}
	else
	{ // �O������ꍇ

		m_pPrev->SetColor(tutorial::COL_NORMAL);
	}

	// ������\���̐F��ݒ�
	if (m_nLesson >= LESSON_MAX - 1)
	{ // ��������ȏ�Ȃ��ꍇ

		m_pNext->SetColor(tutorial::COL_LAST);
	}
	else
	{ // ��������ꍇ

		m_pNext->SetColor(tutorial::COL_NORMAL);
	}
}
