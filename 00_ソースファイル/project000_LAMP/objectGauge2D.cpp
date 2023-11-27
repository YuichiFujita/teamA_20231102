//============================================================
//
//	�I�u�W�F�N�g�Q�[�W2D���� [objectGauge2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectGauge2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_VERTEX	(12)	// ���_��
#define GAUGE_PRIO	(13)	// �Q�[�W2D�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CObjectGauge2D::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	NULL,	// �t���[������
	"data\\TEXTURE\\lifeGauge2D000.png",	// �o���l�t���[��
};

//************************************************************
//	�q�N���X [CObjectGauge2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectGauge2D::CObjectGauge2D() : m_nFrame(0)
{
	// �����o�ϐ����N���A
	m_pVtxBuff	= NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		= VEC3_ZERO;	// �ʒu
	m_sizeGauge	= VEC3_ZERO;	// �Q�[�W�傫��
	m_sizeFrame	= VEC3_ZERO;	// �g�傫��
	m_colFront	= XCOL_WHITE;	// �\�Q�[�W�F
	m_colBack	= XCOL_WHITE;	// ���Q�[�W�F
	m_state = STATE_NONE;		// ���
	m_bDrawFrame = false;		// �g�\����
	m_fChange = 0.0f;			// �Q�[�W�ϓ���
	m_fCurrentNumGauge = 0.0f;	// ���ݕ\���l
	m_fAddRight = 0.0f;			// �������Z��
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nNumGauge = 0;			// �\���l
	m_nMaxNumGauge = 0;			// �\���l�̍ő�l

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		// �e�N�X�`���C���f�b�N�X���N���A
		m_aTextureID[nCntGauge] = 0;
	}
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectGauge2D::CObjectGauge2D(const int nFrame, const CObject::ELabel label, const int nPriority) : CObject(label, nPriority), m_nFrame(nFrame)
{
	// �����o�ϐ����N���A
	m_pVtxBuff	= NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		= VEC3_ZERO;	// �ʒu
	m_sizeGauge = VEC3_ZERO;	// �Q�[�W�傫��
	m_sizeFrame = VEC3_ZERO;	// �g�傫��
	m_colFront	= XCOL_WHITE;	// �\�Q�[�W�F
	m_colBack	= XCOL_WHITE;	// ���Q�[�W�F
	m_state = STATE_NONE;		// ���
	m_bDrawFrame = false;		// �g�\����
	m_fChange = 0.0f;			// �Q�[�W�ϓ���
	m_fCurrentNumGauge = 0.0f;	// ���ݕ\���l
	m_fAddRight = 0.0f;			// �������Z��
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nNumGauge = 0;			// �\���l
	m_nMaxNumGauge = 0;			// �\���l�̍ő�l

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		// �e�N�X�`���C���f�b�N�X���N���A
		m_aTextureID[nCntGauge] = 0;
	}
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectGauge2D::~CObjectGauge2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectGauge2D::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff	= NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	m_pos		= VEC3_ZERO;		// �ʒu
	m_sizeGauge = VEC3_ZERO;		// �Q�[�W�傫��
	m_sizeFrame = VEC3_ZERO;		// �g�傫��
	m_colFront	= XCOL_WHITE;		// �\�Q�[�W�F
	m_colBack	= XCOL_WHITE;		// ���Q�[�W�F
	m_state = STATE_NONE;			// ���
	m_bDrawFrame = false;			// �g�\����
	m_fChange = 0.0f;				// �Q�[�W�ϓ���
	m_fCurrentNumGauge = 0.0f;		// ���ݕ\���l
	m_fAddRight = 0.0f;				// �������Z��
	m_nCounterState = 0;			// ��ԊǗ��J�E���^�[
	m_nNumGauge = 0;				// �\���l
	m_nMaxNumGauge = 0;				// �\���l�̍ő�l

	for (int nCntTexture = 0; nCntTexture < POLYGON_MAX; nCntTexture++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		// �e�N�X�`���C���f�b�N�X���N���A
		m_aTextureID[nCntTexture] = NONE_IDX;
	}

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (m_pVtxBuff == NULL)
	{ // ��g�p���̏ꍇ

		// ���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer
		( // ����
			sizeof(VERTEX_2D) * MAX_VERTEX,	// �K�v���_��
			D3DUSAGE_WRITEONLY,				// �g�p���@
			FVF_VERTEX_2D,					// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,				// �������̎w��
			&m_pVtxBuff,					// ���_�o�b�t�@�ւ̃|�C���^
			NULL
		)))
		{ // ���_�o�b�t�@�̐����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	// ���_���̐ݒ�
	SetVtx();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectGauge2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�Q�[�W2D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectGauge2D::Update(void)
{
	// �Q�[�W�̐ݒ�
	if (m_state == STATE_CHANGE)
	{ // �̗͂��ϓ����̏ꍇ

		// �J�E���^�[�����Z
		m_nCounterState--;

		// ���݂̃Q�[�W�ɕϓ��ʂ����Z
		m_fCurrentNumGauge += m_fChange;

		// ���݂̕\���l��␳
		useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);

		// �Q�[�W�̉������Z�ʂ�ݒ�
		m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

		// ���_���̐ݒ�
		SetVtx();

		if (m_nCounterState <= 0)
		{ // �J�E���^�[�� 0�ȉ��ɂȂ����ꍇ

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectGauge2D::Draw(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		if (nCntGauge == POLYGON_FRAME)
		{ // �`�悷��l�p�`�|���S�����g�̏ꍇ

			if (m_bDrawFrame)
			{ // �g��\������ꍇ

				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pTexture->GetTexture(m_aTextureID[nCntGauge]));

				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
			}
		}
		else
		{ // �`�悷��l�p�`�|���S�����g�ȊO�̏ꍇ

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture->GetTexture(m_aTextureID[nCntGauge]));

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);
		}
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectGauge2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	��������
//============================================================
CObjectGauge2D *CObjectGauge2D::Create
(
	const CObject::ELabel label,	// �I�u�W�F�N�g���x��
	const int nMax,					// �ő�\���l
	const int nFrame,				// �\���l�ϓ��t���[��
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rSizeGauge,	// �Q�[�W�傫��
	const D3DXCOLOR& rColFront,		// �\�Q�[�W�F
	const D3DXCOLOR& rColBack,		// ���Q�[�W�F
	const bool bDrawFrame,			// �g�`���
	const EType frameType,			// �g���
	const D3DXVECTOR3& rSizeFrame	// �g�傫��
)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CObjectGauge2D *pObjectGauge2D = NULL;			// �I�u�W�F�N�g�Q�[�W2D�����p

	if (pObjectGauge2D == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectGauge2D = new CObjectGauge2D(nFrame, label, GAUGE_PRIO);	// �I�u�W�F�N�g�Q�[�W2D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectGauge2D != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�Q�[�W2D�̏�����
		if (FAILED(pObjectGauge2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectGauge2D;
			pObjectGauge2D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^�E����
		pObjectGauge2D->BindTexture(POLYGON_FRAME, pTexture->Regist(mc_apTextureFile[frameType]));

		// �Q�[�W�ő�l��ݒ�
		pObjectGauge2D->SetMaxNum(nMax);

		// �ʒu��ݒ�
		pObjectGauge2D->SetVec3Position(rPos);

		// �傫����ݒ�
		pObjectGauge2D->SetScalingGauge(rSizeGauge);	// �Q�[�W�傫��
		pObjectGauge2D->SetScalingFrame(rSizeFrame);	// �g�傫��

		// �F��ݒ�
		pObjectGauge2D->SetColorFront(rColFront);	// �\�Q�[�W�F
		pObjectGauge2D->SetColorBack(rColBack);		// ���Q�[�W�F

		// �g�̕\���󋵂�ݒ�
		pObjectGauge2D->SetEnableDrawFrame(bDrawFrame);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectGauge2D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Q�[�W�̉��Z����
//============================================================
void CObjectGauge2D::AddNum(const int nAdd)
{
	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_nNumGauge;

	// �\���l�̕ϓ��ʂ����߂�
	m_fChange = ((float)nAdd / (float)m_nFrame);

	// ����ݒ�
	m_state = STATE_CHANGE;		// �Q�[�W�ϓ����
	m_nCounterState = m_nFrame;	// ��ԊǗ��J�E���^�[

	// �\���l�����������Z
	m_nNumGauge += nAdd;

	// �\���l�̐���
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);
}

//============================================================
//	�Q�[�W�̐ݒ菈��
//============================================================
void CObjectGauge2D::SetNum(const int nNum)
{
	// �����̕\���l��ݒ�
	m_nNumGauge = nNum;
	useful::LimitNum(m_nNumGauge, 0, m_nMaxNumGauge);	// �\���l�̐���

	// ���݂̕\���l��ݒ�
	m_fCurrentNumGauge = (float)m_nNumGauge;
	useful::LimitNum(m_fCurrentNumGauge, 0.0f, (float)m_nMaxNumGauge);	// ���݂̕\���l�̐���

	// ����ݒ�
	m_state = STATE_NONE;	// �Q�[�W�ϓ����
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �Q�[�W�̉������Z�ʂ�ݒ�
	m_fAddRight = (m_fCurrentNumGauge * ((m_sizeGauge.x * 2.0f) / (float)m_nMaxNumGauge)) - m_sizeGauge.x;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�Q�[�W�擾����
//============================================================
int CObjectGauge2D::GetNum(void) const
{
	// �\���l��Ԃ�
	return m_nNumGauge;
}

//============================================================
//	�Q�[�W�ő�l�̐ݒ菈��
//============================================================
void CObjectGauge2D::SetMaxNum(const int nMax)
{
	// �����̕\���ő�l��ݒ�
	m_nMaxNumGauge = nMax;

	// �Q�[�W�̐ݒ�
	SetNum(m_nMaxNumGauge);
}

//============================================================
//	�Q�[�W�ő�l�擾����
//============================================================
int CObjectGauge2D::GetMaxNum(void) const
{
	// �\���ő�l��Ԃ�
	return m_nMaxNumGauge;
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectGauge2D::BindTexture(const int nPolygonID, const int nTextureID)
{
	if (nPolygonID < POLYGON_MAX)
	{ // �C���f�b�N�X���g�p����l�p�`�|���S������菬�����ꍇ

		if (nTextureID >= NONE_IDX)
		{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

			// �e�N�X�`���C���f�b�N�X����
			m_aTextureID[nPolygonID] = nTextureID;
		}
		else { assert(false); }	// �͈͊O
	}
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObjectGauge2D::BindTexture(const int nPolygonID, const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	if (pTexture == NULL)
	{ // �e�N�X�`���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (nPolygonID < POLYGON_MAX)
	{ // �C���f�b�N�X���g�p����l�p�`�|���S������菬�����ꍇ

		if (pTexturePass != NULL)
		{ // ���蓖�Ă�e�N�X�`���p�X�����݂���ꍇ
	
			// �e�N�X�`���C���f�b�N�X����
			m_aTextureID[nPolygonID] = pTexture->Regist(pTexturePass);
		}
		else { assert(false); }	// �e�N�X�`���p�X����
	}
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CObjectGauge2D::SetScalingGauge(const D3DXVECTOR3& rSize)
{
	// �����̃Q�[�W�傫������
	m_sizeGauge = rSize;

	// �Q�[�W�̐ݒ�
	SetNum(m_nNumGauge);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�Q�[�W�傫���擾����
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetScalingGauge(void) const
{
	// �Q�[�W�傫����Ԃ�
	return m_sizeGauge;
}

//============================================================
//	�w�i�傫���̐ݒ菈��
//============================================================
void CObjectGauge2D::SetScalingFrame(const D3DXVECTOR3& rSize)
{
	// �����̔w�i�傫������
	m_sizeFrame = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�w�i�傫���擾����
//============================================================
D3DXVECTOR3 CObjectGauge2D::GetScalingFrame(void) const
{
	// �w�i�傫����Ԃ�
	return m_sizeFrame;
}

//============================================================
//	�\�Q�[�W�F�̐ݒ菈��
//============================================================
void CObjectGauge2D::SetColorFront(const D3DXCOLOR& rCol)
{
	// �����̕\�Q�[�W�F����
	m_colFront = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�\�Q�[�W�F�擾����
//============================================================
D3DXCOLOR CObjectGauge2D::GetColorFront(void) const
{
	// �\�Q�[�W�F��Ԃ�
	return m_colFront;
}

//============================================================
//	���Q�[�W�F�̐ݒ菈��
//============================================================
void CObjectGauge2D::SetColorBack(const D3DXCOLOR& rCol)
{
	// �����̗��Q�[�W�F����
	m_colBack = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���Q�[�W�F�擾����
//============================================================
D3DXCOLOR CObjectGauge2D::GetColorBack(void) const
{
	// ���Q�[�W�F��Ԃ�
	return m_colBack;
}

//============================================================
//	�g�\���󋵐ݒ菈��
//============================================================
void CObjectGauge2D::SetEnableDrawFrame(const bool bDraw)
{
	// �����̘g�̕\���󋵂�ݒ�
	m_bDrawFrame = bDraw;
}

//============================================================
//	�g�\���󋵎擾����
//============================================================
bool CObjectGauge2D::IsEnableDrawFrame(void) const
{
	// �g�\���󋵂�Ԃ�
	return m_bDrawFrame;
}

//============================================================
//	�j������
//============================================================
void CObjectGauge2D::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectGauge2D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
		{ // �g�p����l�p�`�|���S�������J��Ԃ�

			switch (nCntGauge)
			{ // �l�p�`�|���S�����Ƃ̏���
			case POLYGON_BACK:	// �w�i

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colBack;
				pVtx[1].col = m_colBack;
				pVtx[2].col = m_colBack;
				pVtx[3].col = m_colBack;

				break;

			case POLYGON_FRONT:	// �Q�[�W

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y - m_sizeGauge.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeGauge.x, m_pos.y + m_sizeGauge.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fAddRight,   m_pos.y + m_sizeGauge.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colFront;
				pVtx[1].col = m_colFront;
				pVtx[2].col = m_colFront;
				pVtx[3].col = m_colFront;

				break;

			case POLYGON_FRAME:	// �g

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y - m_sizeFrame.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_sizeFrame.x, m_pos.y + m_sizeFrame.y, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = XCOL_WHITE;
				pVtx[1].col = XCOL_WHITE;
				pVtx[2].col = XCOL_WHITE;
				pVtx[3].col = XCOL_WHITE;

				break;

			default:	// ��O����
				assert(false);
				break;
			}

			// rhw �̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�f�[�^�̃|�C���^�� 4���i�߂�
			pVtx += 4;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}
