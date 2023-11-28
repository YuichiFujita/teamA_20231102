//============================================================
//
//	�|�[�Y���� [pause.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "pause.h"
#include "renderer.h"
#include "manager.h"
#include "sceneGame.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"
#include "timerManager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define PAUSE_PRIO	(14)	// �|�[�Y�\���̗D�揇��

#define PAUSE_POS	(D3DXVECTOR3(640.0f, 210.0f, 0.0f))	// �Z���N�g���j���[�̈ʒu
#define PAUSE_SIZE	(D3DXVECTOR3(440.0f, 110.0f, 0.0f))	// �Z���N�g���j���[�̑傫��
#define PAUSE_SPACE	(D3DXVECTOR3(0.0f, 150.0f, 0.0f))	// �Z���N�g���j���[�̋�

#define BG_COL		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f))	// �w�i�J���[
#define CHOICE_COL	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	// �I�𒆃J���[
#define DEFAULT_COL	(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f))	// ��I�𒆃J���[

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CPause::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\TEXTURE\\pause000.png",	// �ĊJ�e�N�X�`��
	"data\\TEXTURE\\pause001.png",	// ���g���C�e�N�X�`��
	"data\\TEXTURE\\pause002.png",	// �I���e�N�X�`��
};

//************************************************************
//	�q�N���X [CPause] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPause::CPause()
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�����
	m_pBG		 = NULL;			// �w�i���
	m_nSelect	 = SELECT_RESUME;	// ���݂̑I��
	m_nOldSelect = SELECT_RESUME;	// �O��̑I��
	m_bPause	 = false;			// �|�[�Y��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPause::~CPause()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPause::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`��

	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�����
	m_pBG		 = NULL;			// �w�i���
	m_nSelect	 = SELECT_RESUME;	// ���݂̑I��
	m_nOldSelect = SELECT_RESUME;	// �O��̑I��
	m_bPause	 = false;			// �|�[�Y��

	// �w�i���̐���
	m_pBG = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		BG_COL			// �F
	);
	if (m_pBG == NULL)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pBG->SetPriority(PAUSE_PRIO);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̐���
		m_apSelect[nCntPause] = CObject2D::Create
		( // ����
			PAUSE_POS + (PAUSE_SPACE * (float)nCntPause),	// �ʒu
			PAUSE_SIZE,	// �傫��
			VEC3_ZERO,	// ����
			DEFAULT_COL	// �F
		);
		if (m_apSelect[nCntPause] == NULL)
		{ // ��g�p���̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apSelect[nCntPause]->SetPriority(PAUSE_PRIO);

		// �e�N�X�`����o�^�E����
		m_apSelect[nCntPause]->BindTexture(pTexture->Regist(mc_apTextureFile[nCntPause]));
	}

	// �`��󋵂̐ݒ�
	SetEnableDraw(m_bPause);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPause::Uninit(void)
{
	// �w�i���̏I��
	m_pBG->Uninit();

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̏I��
		m_apSelect[nCntPause]->Uninit();
	}
}

//============================================================
//	�X�V����
//============================================================
void CPause::Update(void)
{
	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (CManager::GetInstance()->GetKeyboard()->IsTrigger(DIK_P)
		||  CManager::GetInstance()->GetPad()->IsTrigger(CInputPad::KEY_START))
		{
			// �|�[�Y�󋵂�؂�ւ�
			m_bPause = (!m_bPause) ? true : false;

			// �^�C���̌v���󋵂�؂�ւ�
			CSceneGame::GetTimerManager()->EnableStop(m_bPause);

			// ���݂̑I����������
			m_nSelect = SELECT_RESUME;

			// �`��󋵂̐ݒ�
			SetEnableDraw(m_bPause);

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
		}
	}

	if (m_bPause)
	{ // �|�[�Y���̏ꍇ

		// �I�𑀍�
		Select();

		// �O��̑I��v�f�̐F�����ɐݒ�
		m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

		// ���݂̑I��v�f�̐F�𔒂ɐݒ�
		m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

		// ���݂̑I��v�f����
		m_nOldSelect = m_nSelect;
	}

	// �w�i���̍X�V
	m_pBG->Update();

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̍X�V
		m_apSelect[nCntPause]->Update();
	}
}

//============================================================
//	�`�揈��
//============================================================
void CPause::Draw(void)
{

}

//============================================================
//	�|�[�Y�󋵎擾����
//============================================================
bool CPause::IsPause(void)
{
	// �|�[�Y�󋵂�Ԃ�
	return m_bPause;
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPause::SetEnableDraw(const bool bDraw)
{
	// �ϐ���錾
	bool bDrawPause = bDraw;	// �|�[�Y�̕`���

	if (!CSceneGame::IsDrawPause())
	{ // �|�[�Y��`�悵�Ȃ��󋵂������ꍇ

		bDrawPause = false;
	}

	// �w�i���̕`��󋵂�ݒ�
	m_pBG->SetEnableDraw(bDrawPause);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // �I�����̍��ڐ����J��Ԃ�

		// �I�����̕`��󋵂�ݒ�
		m_apSelect[nCntPause]->SetEnableDraw(bDrawPause);
	}
}

//============================================================
//	��������
//============================================================
CPause *CPause::Create(void)
{
	// �|�C���^��錾
	CPause *pPause = NULL;		// �|�[�Y�����p

	if (pPause == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pPause = new CPause;	// �|�[�Y
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pPause != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �|�[�Y�̏�����
		if (FAILED(pPause->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pPause;
			pPause = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPause;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CPause::Release(CPause *&prPause)
{
	if (prPause != NULL)
	{ // �g�p���̏ꍇ

		// �|�[�Y�̏I��
		prPause->Uninit();

		// �������J��
		delete prPause;
		prPause = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�I�𑀍쏈��
//============================================================
void CPause::Select(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h
	CInputPad		*pPad		= CManager::GetInstance()->GetPad();		// �p�b�h

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (pKeyboard->IsTrigger(DIK_W)
		||  pKeyboard->IsTrigger(DIK_UP)
		||  pPad->IsTrigger(CInputPad::KEY_UP))
		{ // ��ړ��̑��삪�s��ꂽ�ꍇ

			// ��ɑI�������炷
			m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}
		if (pKeyboard->IsTrigger(DIK_S)
		||  pKeyboard->IsTrigger(DIK_DOWN)
		||  pPad->IsTrigger(CInputPad::KEY_DOWN))
		{ // ���ړ��̑��삪�s��ꂽ�ꍇ

			// ���ɑI�������炷
			m_nSelect = (m_nSelect + 1) % SELECT_MAX;

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_SELECT_000);	// �I�𑀍쉹00
		}

		if (pKeyboard->IsTrigger(DIK_RETURN)  || pKeyboard->IsTrigger(DIK_SPACE)
		||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
		||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
		{ // ����̑��삪�s��ꂽ�ꍇ

			// �T�E���h�̍Đ�
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00

			switch (m_nSelect)
			{ // �I�����Ƃ̏���
			case SELECT_RESUME:	// �ĊJ

				// �|�[�Y���I������
				m_bPause = false;

				// �^�C���̌v�����ĊJ����
				CSceneGame::GetTimerManager()->EnableStop(m_bPause);

				// �`��󋵂̐ݒ�
				SetEnableDraw(m_bPause);

				// �����𔲂���
				break;

			case SELECT_RETRY:	// ���g���C

				// �V�[���̐ݒ�
				CManager::GetInstance()->SetScene(CScene::MODE_GAME);	// �Q�[�����

				// �����𔲂���
				break;

			case SELECT_EXIT:	// �I��

				// �V�[���̐ݒ�
				CManager::GetInstance()->SetScene(CScene::MODE_TITLE);	// �^�C�g�����

				// �����𔲂���
				break;
			}
		}
	}
}
