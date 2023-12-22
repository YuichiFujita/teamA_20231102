//============================================================
//
//	�I�u�W�F�N�g�r���{�[�h���� [objectBillboard.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"
#include "texture.h"

//************************************************************
//	�}�N����`
//************************************************************
#define MAX_VERTEX	(4)	// ���_��

//************************************************************
//	�q�N���X [CObjectBillboard] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectBillboard::CObjectBillboard()
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos      = VEC3_ZERO;		// �ʒu
	m_rot      = VEC3_ZERO;		// ����
	m_size     = VEC3_ZERO;		// �傫��
	m_col      = XCOL_WHITE;	// �F
	m_origin   = ORIGIN_CENTER;	// ���_
	m_rotate   = ROTATE_NORMAL;	// ��]
	m_func     = D3DCMP_ALWAYS;	// Z�e�X�g�ݒ�
	m_bZEnable = false;			// Z�o�b�t�@�̎g�p��
	m_fAngle   = 0.0f;			// �Ίp���̊p�x
	m_fLength  = 0.0f;			// �Ίp���̒���
	m_nTextureID = 0;			// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectBillboard::CObjectBillboard(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos      = VEC3_ZERO;		// �ʒu
	m_rot      = VEC3_ZERO;		// ����
	m_size     = VEC3_ZERO;		// �傫��
	m_col      = XCOL_WHITE;	// �F
	m_origin   = ORIGIN_CENTER;	// ���_
	m_rotate   = ROTATE_NORMAL;	// ��]
	m_func     = D3DCMP_ALWAYS;	// Z�e�X�g�ݒ�
	m_bZEnable = false;			// Z�o�b�t�@�̎g�p��
	m_fAngle   = 0.0f;			// �Ίp���̊p�x
	m_fLength  = 0.0f;			// �Ίp���̒���
	m_nTextureID = 0;			// �e�N�X�`���C���f�b�N�X
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectBillboard::~CObjectBillboard()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectBillboard::Init(void)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �����o�ϐ���������
	memset(&m_mtxWorld, 0, sizeof(m_mtxWorld));	// ���[���h�}�g���b�N�X
	m_pVtxBuff = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
	m_pos      = VEC3_ZERO;		// �ʒu
	m_rot      = VEC3_ZERO;		// ����
	m_size     = VEC3_ZERO;		// �傫��
	m_col      = XCOL_WHITE;	// �F
	m_origin   = ORIGIN_CENTER;	// ���_
	m_rotate   = ROTATE_NORMAL;	// ��]
	m_func     = D3DCMP_ALWAYS;	// Z�e�X�g�ݒ�
	m_bZEnable = false;			// Z�o�b�t�@�̎g�p��
	m_fAngle   = 0.0f;			// �Ίp���̊p�x
	m_fLength  = 0.0f;			// �Ίp���̒���
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

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
void CObjectBillboard::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�r���{�[�h��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectBillboard::Update(void)
{
	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CObjectBillboard::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;		// �r���[�}�g���b�N�X

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();							// �e�N�X�`���ւ̃|�C���^

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g��ݒ肷��
	pDevice->SetRenderState(D3DRS_ZFUNC, m_func);				// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, m_bZEnable);	// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�

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

	switch (m_rotate)
	{ // ��]���Ƃ̏���
	case ROTATE_NORMAL:		// �ʏ��]

		// ����

		break;

	case ROTATE_LATERAL:	// ����]

		m_mtxWorld._21 = 0.0f;
		m_mtxWorld._23 = 0.0f;
		m_mtxWorld._24 = 0.0f;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);		// Z�o�b�t�@�X�V�̗L�� / �����̐ݒ�
}

//============================================================
//	��������
//============================================================
CObjectBillboard *CObjectBillboard::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rSize,	// �傫��
	const D3DXVECTOR3& rRot,	// ����
	const D3DXCOLOR& rCol,		// �F
	const EOrigin origin,		// ���_
	const ERotate rotate,		// ��]
	const D3DCMPFUNC func,		// Z�e�X�g�ݒ�
	const bool bZEnable			// Z�o�b�t�@�̎g�p��
)
{
	// �|�C���^��錾
	CObjectBillboard *pObjectBillboard = NULL;	// �I�u�W�F�N�g�r���{�[�h�����p

	if (pObjectBillboard == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectBillboard = new CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectBillboard != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�r���{�[�h�̏�����
		if (FAILED(pObjectBillboard->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectBillboard;
			pObjectBillboard = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ���_��ݒ�
		pObjectBillboard->SetOrigin(origin);

		// �ʒu��ݒ�
		pObjectBillboard->SetVec3Position(rPos);

		// ������ݒ�
		pObjectBillboard->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pObjectBillboard->SetVec3Sizing(rSize);

		// �F��ݒ�
		pObjectBillboard->SetColor(rCol);

		// ��]��ݒ�
		pObjectBillboard->SetRotate(rotate);

		// Z�e�X�g��ݒ�
		pObjectBillboard->SetFunc(func);

		// Z�o�b�t�@�̎g�p�󋵂�ݒ�
		pObjectBillboard->SetZEnable(bZEnable);

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectBillboard;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectBillboard::BindTexture(const int nTextureID)
{
	if (nTextureID > NONE_IDX)
	{ // �e�N�X�`���C���f�b�N�X���g�p�\�ȏꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = nTextureID;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���������� (�p�X)
//============================================================
void CObjectBillboard::BindTexture(const char *pTexturePass)
{
	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	if (pTexture == NULL)
	{ // �e�N�X�`���|�C���^�����݂��Ȃ��ꍇ

		// �֐��𔲂���
		assert(false);
		return;
	}

	if (pTexturePass != NULL)
	{ // ���蓖�Ă�e�N�X�`���p�X�����݂���ꍇ

		// �e�N�X�`���C���f�b�N�X����
		m_nTextureID = pTexture->Regist(pTexturePass);
	}
	else { assert(false); }	// �e�N�X�`���p�X����
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CObjectBillboard::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�ʒu�擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetVec3Position(void) const
{
	// �ʒu��Ԃ�
	return m_pos;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CObjectBillboard::SetVec3Rotation(const D3DXVECTOR3& rRot)
{
	// �����̌�����ݒ�
	m_rot = rRot;

	// �����̐��K��
	useful::Vec3NormalizeRot(m_rot);

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�����擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetVec3Rotation(void) const
{
	// ������Ԃ�
	return m_rot;
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CObjectBillboard::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �����̑傫������
	m_size = rSize;

	switch (m_origin)
	{ // ���_���Ƃ̏���
	case ORIGIN_CENTER:	// ����

		// �Ίp���̊p�x�����߂�
		m_fAngle = atan2f(m_size.x, m_size.y);

		// �Ίp���̒��������߂�
		m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

		break;

	case ORIGIN_DOWN:	// ��

		// �Ίp���̊p�x�����߂�
		m_fAngle = atan2f(m_size.x, m_size.y);

		// �Ίp���̒��������߂�
		m_fLength = sqrtf(m_size.x * m_size.x + (m_size.y * 2.0f) * (m_size.y * 2.0f)) * 0.5f;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�傫���擾����
//============================================================
D3DXVECTOR3 CObjectBillboard::GetVec3Sizing(void) const
{
	// �傫����Ԃ�
	return m_size;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CObjectBillboard::SetColor(const D3DXCOLOR& rCol)
{
	// �����̐F����
	m_col = rCol;

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	�F�擾����
//============================================================
D3DXCOLOR CObjectBillboard::GetColor(void) const
{
	// �F��Ԃ�
	return m_col;
}

//============================================================
//	�}�g���b�N�X�|�C���^�擾����
//============================================================
D3DXMATRIX *CObjectBillboard::GetPtrMtxWorld(void)
{
	// ���[���h�}�g���b�N�X�̃|�C���^��Ԃ�
	return &m_mtxWorld;
}

//============================================================
//	�}�g���b�N�X�擾����
//============================================================
D3DXMATRIX CObjectBillboard::GetMtxWorld(void) const
{
	// ���[���h�}�g���b�N�X��Ԃ�
	return m_mtxWorld;
}

//============================================================
//	���_�̐ݒ菈��
//============================================================
void CObjectBillboard::SetOrigin(const EOrigin origin)
{
	// �����̌��_��ݒ�
	m_origin = origin;

	switch (m_origin)
	{ // ���_���Ƃ̏���
	case ORIGIN_CENTER:	// ����

		// �Ίp���̊p�x�����߂�
		m_fAngle = atan2f(m_size.x, m_size.y);

		// �Ίp���̒��������߂�
		m_fLength = sqrtf(m_size.x * m_size.x + m_size.y * m_size.y) * 0.5f;

		break;

	case ORIGIN_DOWN:	// ��

		// �Ίp���̊p�x�����߂�
		m_fAngle = atan2f(m_size.x, m_size.y);

		// �Ίp���̒��������߂�
		m_fLength = sqrtf(m_size.x * m_size.x + (m_size.y * 2.0f) * (m_size.y * 2.0f)) * 0.5f;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���_���̐ݒ�
	SetVtx();
}

//============================================================
//	���_�擾����
//============================================================
CObjectBillboard::EOrigin CObjectBillboard::GetOrigin(void) const
{
	// ���_��Ԃ�
	return m_origin;
}

//============================================================
//	��]�̐ݒ菈��
//============================================================
void CObjectBillboard::SetRotate(const ERotate rotate)
{
	// �����̉�]��ݒ�
	m_rotate = rotate;
}

//============================================================
//	��]�擾����
//============================================================
CObjectBillboard::ERotate CObjectBillboard::GetRotate(void) const
{
	// ��]��Ԃ�
	return m_rotate;
}

//============================================================
//	Z�e�X�g�̐ݒ菈��
//============================================================
void CObjectBillboard::SetFunc(const D3DCMPFUNC func)
{
	// ������Z�e�X�g�̐ݒ��ݒ�
	m_func = func;
}

//============================================================
//	Z�e�X�g�擾����
//============================================================
D3DCMPFUNC CObjectBillboard::GetFunc(void) const
{
	// Z�e�X�g�̐ݒ��Ԃ�
	return m_func;
}

//============================================================
//	Z�o�b�t�@�̎g�p�󋵂̐ݒ菈��
//============================================================
void CObjectBillboard::SetZEnable(const bool bEnable)
{
	// ������Z�o�b�t�@�̎g�p�󋵂�ݒ�
	m_bZEnable = bEnable;
}

//============================================================
//	Z�o�b�t�@�̎g�p�󋵎擾����
//============================================================
bool CObjectBillboard::IsZEnable(void) const
{
	// Z�o�b�t�@�̎g�p�󋵂�Ԃ�
	return m_bZEnable;
}

//============================================================
//	���_���̐ݒ菈��
//============================================================
void CObjectBillboard::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_origin)
	{ // ���_���Ƃ̏���
	case ORIGIN_CENTER:	// ����

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f + sinf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.y = 0.0f - cosf(m_rot.z - m_fAngle) * m_fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f + sinf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.y = 0.0f - cosf(m_rot.z + m_fAngle) * m_fLength;
		pVtx[3].pos.z = 0.0f;

		break;

	case ORIGIN_DOWN:	// ��

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI + m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f + sinf(m_rot.z - (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[2].pos.y = 0.0f - cosf(m_rot.z - (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f + sinf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[3].pos.y = 0.0f - cosf(m_rot.z + (D3DX_PI * 0.5f)) * m_size.x;
		pVtx[3].pos.z = 0.0f;

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//============================================================
//	�j������
//============================================================
void CObjectBillboard::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
