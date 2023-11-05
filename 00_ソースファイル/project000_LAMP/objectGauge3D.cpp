//============================================================
//
//	�I�u�W�F�N�g�Q�[�W3D���� [objectGauge3D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectGauge3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_VERTEX	(12)	// ���_��
#define GAUGE_PRIO	(9)		// �Q�[�W3D�̗D�揇��

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const char *CObjectGauge3D::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	NULL,	// �t���[������
	"data\\TEXTURE\\lifeGauge3D000.png",	// �v���C���[�t���[��
	"data\\TEXTURE\\lifeGauge3D001.png",	// �^�[�Q�b�g�t���[��
};

//************************************************************
//	�q�N���X [CObjectGauge3D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectGauge3D::CObjectGauge3D() : m_nFrame(0)
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
CObjectGauge3D::CObjectGauge3D(const int nFrame, const CObject::ELabel label, const int nPriority) : CObject(label, nPriority), m_nFrame(nFrame)
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
CObjectGauge3D::~CObjectGauge3D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectGauge3D::Init(void)
{
	// �����o�ϐ���������
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
			sizeof(VERTEX_3D) * MAX_VERTEX,	// �K�v���_��
			D3DUSAGE_WRITEONLY,				// �g�p���@
			FVF_VERTEX_3D,					// ���_�t�H�[�}�b�g
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
void CObjectGauge3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �Q�[�W�\���I�u�W�F�N�g�̍폜
	DeleteObjectGauge();

	// �I�u�W�F�N�g�Q�[�W3D��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectGauge3D::Update(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxGauge;	// �Q�[�W�\���I�u�W�F�N�g�̃}�g���b�N�X

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

	// �ʒu�̐ݒ�
	for (int nCntGauge = 0; nCntGauge < POLYGON_MAX; nCntGauge++)
	{ // �g�p����l�p�`�|���S�������J��Ԃ�

		if (m_pGauge != NULL)
		{ // �Q�[�W�\���Ώۂ��g�p����Ă����ꍇ

			// ���[���h�}�g���b�N�X���擾
			mtxGauge = m_pGauge->GetMtxWorld();

			// �ʒu��ݒ�
			m_pos = D3DXVECTOR3(mtxGauge._41, mtxGauge._42 + m_fPosUp, mtxGauge._43);
		}
		else
		{ // �Q�[�W�\���Ώۂ��g�p����Ă��Ȃ������ꍇ

			// �Q�[�W�\�����Ă���I�u�W�F�N�g�̏���������
			m_pGauge = NULL;

			// �I�u�W�F�N�g�̏I��
			Uninit();

			// �֐��𔲂���
			return;
		}
	}
}

//============================================================
//	�`�揈��
//============================================================
void CObjectGauge3D::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	// �t�s������߂�

	// �}�g���b�N�X�̃��[���h���W�����_�ɂ���
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

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

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectGauge3D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectGauge3D::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	��������
//============================================================
CObjectGauge3D *CObjectGauge3D::Create
(
	const CObject::ELabel label,		// �I�u�W�F�N�g���x��
	CObject *pObject,				// �Q�[�W�\���I�u�W�F�N�g
	const int nMax,					// �ő�\���l
	const int nFrame,				// �\���l�ϓ��t���[��
	const float fPosUp,				// �\��Y�ʒu�̉��Z��
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
	CObjectGauge3D *pObjectGauge3D = NULL;			// �I�u�W�F�N�g�Q�[�W3D�����p

	if (pObjectGauge3D == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectGauge3D = new CObjectGauge3D(nFrame, label, GAUGE_PRIO);	// �I�u�W�F�N�g�Q�[�W3D
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectGauge3D != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�Q�[�W3D�̏�����
		if (FAILED(pObjectGauge3D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectGauge3D;
			pObjectGauge3D = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�N�X�`����o�^�E����
		pObjectGauge3D->BindTexture(POLYGON_FRAME, pTexture->Regist(mc_apTextureFile[frameType]));

		// �Q�[�W�\���I�u�W�F�N�g��ݒ�
		pObjectGauge3D->SetGaugeObject(pObject);

		// Y�ʒu���Z�ʂ�ݒ�
		pObjectGauge3D->SetVec3PositionUp(fPosUp);

		// �Q�[�W�ő�l��ݒ�
		pObjectGauge3D->SetMaxNum(nMax);

		// �傫����ݒ�
		pObjectGauge3D->SetScalingGauge(rSizeGauge);	// �Q�[�W�傫��
		pObjectGauge3D->SetScalingFrame(rSizeFrame);	// �g�傫��

		// �F��ݒ�
		pObjectGauge3D->SetColorFront(rColFront);	// �\�Q�[�W�F
		pObjectGauge3D->SetColorBack(rColBack);		// ���Q�[�W�F

		// �g�̕\���󋵂�ݒ�
		pObjectGauge3D->SetEnableDrawFrame(bDrawFrame);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectGauge3D;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Q�[�W�̉��Z����
//============================================================
void CObjectGauge3D::AddNum(const int nAdd)
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
void CObjectGauge3D::SetNum(const int nNum)
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
int CObjectGauge3D::GetNum(void) const
{
	// �\���l��Ԃ�
	return m_nNumGauge;
}

//============================================================
//	�Q�[�W�ő�l�̐ݒ菈��
//============================================================
void CObjectGauge3D::SetMaxNum(const int nMax)
{
	// �����̕\���ő�l��ݒ�
	m_nMaxNumGauge = nMax;

	// �Q�[�W�̐ݒ�
	SetNum(m_nMaxNumGauge);
}

//============================================================
//	�Q�[�W�ő�l�擾����
//============================================================
int CObjectGauge3D::GetMaxNum(void) const
{
	// �\���ő�l��Ԃ�
	return m_nMaxNumGauge;
}

//============================================================
//	�Q�[�W�\���I�u�W�F�N�g�̍폜����
//============================================================
void CObjectGauge3D::DeleteObjectGauge(void)
{
	// �Q�[�W3D�\���I�u�W�F�N�g��NULL�ɂ���
	m_pGauge = NULL;
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectGauge3D::BindTexture(const int nPolygonID, const int nTextureID)
{
	if (nPolygonID < POLYGON_MAX)
	{ // �C���f�b�N�X���g�p����l�p�`�|���S������菬�����ꍇ

		if (nTextureID > NONE_IDX)
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
void CObjectGauge3D::BindTexture(const int nPolygonID, const char *pTexturePass)
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
//	�Q�[�W�\���I�u�W�F�N�g�̐ݒ菈��
//============================================================
void CObjectGauge3D::SetGaugeObject(CObject *pObject)
{
	// �����̃I�u�W�F�N�g��ݒ�
	m_pGauge = pObject;
}

//============================================================
//	�Q�[�W�\���I�u�W�F�N�g�擾����
//============================================================
const CObject *CObjectGauge3D::GetGaugeObject(void) const
{
	// �Q�[�W�\�����̃I�u�W�F�N�g��Ԃ�
	return m_pGauge;
}

//============================================================
//	Y�ʒu���Z�ʂ̐ݒ菈��
//============================================================
void CObjectGauge3D::SetVec3PositionUp(const float fUp)
{
	// ������Y�ʒu���Z�ʂ�ݒ�
	m_fPosUp = fUp;
}

//============================================================
//	Y�ʒu���Z�ʎ擾����
//============================================================
float CObjectGauge3D::GetVec3PositionUp(void) const
{
	// Y�ʒu���Z�ʂ�Ԃ�
	return m_fPosUp;
}

//============================================================
//	�Q�[�W�傫���̐ݒ菈��
//============================================================
void CObjectGauge3D::SetScalingGauge(const D3DXVECTOR3& rSize)
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
D3DXVECTOR3 CObjectGauge3D::GetScalingGauge(void) const
{
	// �Q�[�W�傫����Ԃ�
	return m_sizeGauge;
}

//============================================================
//	�w�i�傫���̐ݒ菈��
//============================================================
void CObjectGauge3D::SetScalingFrame(const D3DXVECTOR3& rSize)
{
	// �����̔w�i�傫������
	m_sizeFrame = rSize;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�w�i�傫���擾����
//============================================================
D3DXVECTOR3 CObjectGauge3D::GetScalingFrame(void) const
{
	// �w�i�傫����Ԃ�
	return m_sizeFrame;
}

//============================================================
//	�\�Q�[�W�F�̐ݒ菈��
//============================================================
void CObjectGauge3D::SetColorFront(const D3DXCOLOR& rCol)
{
	// �����̕\�Q�[�W�F����
	m_colFront = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�\�Q�[�W�F�擾����
//============================================================
D3DXCOLOR CObjectGauge3D::GetColorFront(void) const
{
	// �\�Q�[�W�F��Ԃ�
	return m_colFront;
}

//============================================================
//	���Q�[�W�F�̐ݒ菈��
//============================================================
void CObjectGauge3D::SetColorBack(const D3DXCOLOR& rCol)
{
	// �����̗��Q�[�W�F����
	m_colBack = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���Q�[�W�F�擾����
//============================================================
D3DXCOLOR CObjectGauge3D::GetColorBack(void) const
{
	// ���Q�[�W�F��Ԃ�
	return m_colBack;
}

//============================================================
//	�g�\���󋵐ݒ菈��
//============================================================
void CObjectGauge3D::SetEnableDrawFrame(const bool bDraw)
{
	// �����̘g�̕\���󋵂�ݒ�
	m_bDrawFrame = bDraw;
}

//============================================================
//	�g�\���󋵎擾����
//============================================================
bool CObjectGauge3D::GetEnableDrawFrame(void) const
{
	// �g�\���󋵂�Ԃ�
	return m_bDrawFrame;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectGauge3D::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

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
				pVtx[0].pos = D3DXVECTOR3(-m_sizeGauge.x * 0.5f,  m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( m_sizeGauge.x * 0.5f,  m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-m_sizeGauge.x * 0.5f, -m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( m_sizeGauge.x * 0.5f, -m_sizeGauge.y * 0.5f, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colBack;
				pVtx[1].col = m_colBack;
				pVtx[2].col = m_colBack;
				pVtx[3].col = m_colBack;

				break;

			case POLYGON_FRONT:	// �Q�[�W

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(-m_sizeGauge.x * 0.5f,  m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( m_fAddRight   * 0.5f,  m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-m_sizeGauge.x * 0.5f, -m_sizeGauge.y * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( m_fAddRight   * 0.5f, -m_sizeGauge.y * 0.5f, 0.0f);

				// ���_�J���[�̐ݒ�
				pVtx[0].col = m_colFront;
				pVtx[1].col = m_colFront;
				pVtx[2].col = m_colFront;
				pVtx[3].col = m_colFront;

				break;

			case POLYGON_FRAME:	// �g

				// ���_���W��ݒ�
				pVtx[0].pos = D3DXVECTOR3(-m_sizeFrame.x * 0.5f,  m_sizeFrame.y * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3( m_sizeFrame.x * 0.5f,  m_sizeFrame.y * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-m_sizeFrame.x * 0.5f, -m_sizeFrame.y * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3( m_sizeFrame.x * 0.5f, -m_sizeFrame.y * 0.5f, 0.0f);

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

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = VEC3_ZERO;
			pVtx[1].nor = VEC3_ZERO;
			pVtx[2].nor = VEC3_ZERO;
			pVtx[3].nor = VEC3_ZERO;

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
