//============================================================
//
//	�t�F�[�h���� [fade.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "fade.h"
#include "manager.h"
#include "object2D.h"

//************************************************************
//	�}�N����`
//************************************************************
#define FADE_PRIO	(15)	// �t�F�[�h�̗D�揇��
#define FADE_LEVEL	(0.05f)	// �t�F�[�h�̃��l�̉�����

#if _DEBUG

#define INIT_SCENE	(CScene::MODE_RESULT)	// �����V�[��

#else

#define INIT_SCENE	(CScene::MODE_TITLE)	// �����V�[��

#endif

//************************************************************
//	�e�N���X [CFade] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CFade::CFade()
{
	// �����o�ϐ����N���A
	m_pObject2D	= NULL;					// �t�F�[�h�\���̏��
	m_fade		= FADE_NONE;			// �t�F�[�h���
	m_modeNext	= CScene::MODE_TITLE;	// ���̃V�[�����[�h
	m_nCounterWait = 0;					// �]�C�Ǘ��J�E���^�[
}

//============================================================
//	�f�X�g���N�^
//============================================================
CFade::~CFade()
{

}

//============================================================
//	����������
//============================================================
HRESULT CFade::Init(void)
{
	// �����o�ϐ���������
	m_pObject2D = NULL;			// �t�F�[�h�\���̏��
	m_fade		= FADE_IN;		// �t�F�[�h���
	m_modeNext	= INIT_SCENE;	// ���̃V�[�����[�h
	m_nCounterWait = 0;			// �]�C�Ǘ��J�E���^�[

	// �I�u�W�F�N�g2D�̐���
	m_pObject2D = CObject2D::Create
	( // ����
		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),	// �ʒu
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),					// �傫��
		VEC3_ZERO,	// ����
		XCOL_WHITE	// �F
	);
	if (m_pObject2D == NULL)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ̐ݒ�
	m_pObject2D->SetPriority(FADE_PRIO);

	// ���[�h�̐ݒ�
	CManager::GetInstance()->SetMode(m_modeNext);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CFade::Uninit(void)
{
	// �I�u�W�F�N�g2D�̏I��
	m_pObject2D->Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		// �ϐ���錾
		D3DXCOLOR colFade = m_pObject2D->GetColor();	// �t�F�[�h�F

		switch (m_fade)
		{ // �t�F�[�h��Ԃ��Ƃ̏���
		case FADE_WAIT:	// �t�F�[�h�]�C���

			if (m_nCounterWait > 0)
			{ // �J�E���^�[�� 0���傫���ꍇ

				// �J�E���^�[�����Z
				m_nCounterWait--;
			}
			else
			{ // �J�E���^�[�� 0�ȉ��̏ꍇ

				// �t�F�[�h��Ԃ�ݒ�
				m_fade = FADE_OUT;	// �t�F�[�h�A�E�g���
			}

			break;

		case FADE_IN:	// �t�F�[�h�C�����

			// ���l�����Z
			colFade.a -= FADE_LEVEL;

			if (colFade.a <= 0.0f)
			{ // ���l�� 0.0f����������ꍇ

				// ���l��␳
				colFade.a = 0.0f;

				// �t�F�[�h��Ԃ�ݒ�
				m_fade = FADE_NONE;	// �������Ȃ����
			}

			break;

		case FADE_OUT:	// �t�F�[�h�A�E�g���

			// ���l�����Z
			colFade.a += FADE_LEVEL;

			if (colFade.a >= 1.0f)
			{ // ���l�� 1.0f ���������ꍇ

				// ���l��␳
				colFade.a = 1.0f;

				// �t�F�[�h��Ԃ�ݒ�
				m_fade = FADE_IN;	// �t�F�[�h�C�����

				// ���[�h�̐ݒ�
				CManager::GetInstance()->SetMode(m_modeNext);
			}

			break;

		default:	// ��O����
			assert(false);
			break;
		}

		// �F�̍X�V
		m_pObject2D->SetColor(colFade);

		// �I�u�W�F�N�g2D�̍X�V
		m_pObject2D->Update();
	}
}

//============================================================
//	���V�[���ւ̃t�F�[�h�ݒ菈��
//============================================================
void CFade::Set(const CScene::EMode mode, const int nWait)
{
	// ��O����
	assert(nWait >= 0);	// �]�C�t���[���I�[�o�[

	if (m_fade == FADE_NONE)
	{ // �t�F�[�h���s���Ă��Ȃ��ꍇ

		// �����̃��[�h��ݒ�
		m_modeNext = mode;

		// �]�C�Ǘ��J�E���^�[��ݒ�
		m_nCounterWait = nWait;

		// �t�F�[�h��Ԃ�ݒ�
		if (m_nCounterWait <= 0)
		{ // �J�E���^�[�� 0�ȉ��̏ꍇ

			m_fade = FADE_OUT;	// �t�F�[�h�A�E�g���
		}
		else
		{ // �J�E���^�[�� 0���傫���ꍇ

			m_fade = FADE_WAIT;	// �t�F�[�h�]�C���
		}
	}
}

//============================================================
//	�t�F�[�h��Ԏ擾����
//============================================================
CFade::EFade CFade::GetState(void) const
{
	// ���݂̃t�F�[�h��Ԃ�Ԃ�
	return m_fade;
}

//============================================================
//	��������
//============================================================
CFade *CFade::Create(void)
{
	// �|�C���^��錾
	CFade *pFade = NULL;	// �t�F�[�h�����p

	if (pFade == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pFade = new CFade;	// �t�F�[�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pFade != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �t�F�[�h�̏�����
		if (FAILED(pFade->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pFade;
			pFade = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pFade;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CFade::Release(CFade *&prFade)
{
	if (prFade != NULL)
	{ // �g�p���̏ꍇ

		// �t�F�[�h�̏I��
		prFade->Uninit();

		// �������J��
		delete prFade;
		prFade = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
