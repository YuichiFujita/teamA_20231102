//============================================================
//
//	�I�u�W�F�N�g�O�Տ��� [objectOrbit.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "pause.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
const D3DXVECTOR3 CObjectOrbit::mc_aOffset[][MAX_OFFSET]	// �I�t�Z�b�g�̈ʒu������
{
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(-6.0f, 0.0f, 0.0f) },	// �����r�I�t�Z�b�g
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(6.0f, 0.0f, 0.0f) },	// �E���r�I�t�Z�b�g
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, 6.0f, 0.0f) },	// �����I�t�Z�b�g
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),	D3DXVECTOR3(0.0f, -6.0f, 0.0f) },	// �E���I�t�Z�b�g
};

//************************************************************
//	�q�N���X [CObjectOrbit] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CObjectOrbit::CObjectOrbit()
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_state = STATE_NORMAL;	// ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nNumVtx = 0;			// �K�v���_��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_orbit, 0, sizeof(m_orbit));	// �O�Ղ̏��
}

//============================================================
//	�I�[�o�[���[�h�R���X�g���N�^
//============================================================
CObjectOrbit::CObjectOrbit(const CObject::ELabel label, const int nPriority) : CObject(label, nPriority)
{
	// �����o�ϐ����N���A
	m_pVtxBuff = NULL;		// ���_�o�b�t�@
	m_state = STATE_NORMAL;	// ���
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[
	m_nNumVtx = 0;			// �K�v���_��
	m_nTextureID = 0;		// �e�N�X�`���C���f�b�N�X
	memset(&m_orbit, 0, sizeof(m_orbit));	// �O�Ղ̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CObjectOrbit::~CObjectOrbit()
{

}

//============================================================
//	����������
//============================================================
HRESULT CObjectOrbit::Init(void)
{
	// �����o�ϐ���������
	m_pVtxBuff = NULL;			// ���_�o�b�t�@
	m_state = STATE_NORMAL;		// ���
	m_nCounterState = 0;		// ��ԊǗ��J�E���^�[
	m_nNumVtx = 0;				// �K�v���_��
	m_nTextureID = NONE_IDX;	// �e�N�X�`���C���f�b�N�X

	// �O�Ղ̏���������
	memset(&m_orbit.mtxVanish, 0, sizeof(m_orbit.mtxVanish));	// �����J�n���̐e�̃}�g���b�N�X
	m_orbit.pMtxParent = NULL;	// �e�̃}�g���b�N�X
	m_orbit.pPosPoint = NULL;	// �e���_���W
	m_orbit.pColPoint = NULL;	// �e���_�J���[
	m_orbit.nPart = 1;			// ������
	m_orbit.nTexPart = 1;		// �e�N�X�`��������
	m_orbit.bAlpha = false;		// ��������
	m_orbit.bInit = false;		// ��������

	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		m_orbit.aOffset[nCntOff] = VEC3_ZERO;	// ���[�̃I�t�Z�b�g
		m_orbit.aCol[nCntOff] = XCOL_WHITE;		// ���[�̊�F
	}

	// ������ݒ�
	if (FAILED(SetLength(1)))
	{ // �����̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CObjectOrbit::Uninit(void)
{
	// �e���_���W�̔j��
	if (m_orbit.pPosPoint != NULL)
	{ // �e���_���W���g�p���̏ꍇ

		// �������J��
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// �e���_�J���[�̔j��
	if (m_orbit.pColPoint != NULL)
	{ // �e���_�J���[���g�p���̏ꍇ

		// �������J��
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�O�Ղ�j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CObjectOrbit::Update(void)
{

}

//============================================================
//	�`�揈��
//============================================================
void CObjectOrbit::Draw(void)
{
	// �ϐ���錾
	D3DXMATRIX mtxIdent;	// �P�ʃ}�g���b�N�X�ݒ�p
	D3DXMATRIX mtxParent;	// �e�̃}�g���b�N�X
	bool bUpdate = true;	// �X�V��

	// �P�ʃ}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxIdent);

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CTexture *pTexture = CManager::GetInstance()->GetTexture();						// �e�N�X�`���ւ̃|�C���^

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{ // ���[�h���Q�[���̏ꍇ

		if (CSceneGame::GetPause()->IsPause())
		{ // �|�[�Y���̏ꍇ

			// �X�V���Ȃ��󋵂ɂ���
			bUpdate = false;
		}
	}
	
	if (m_state != STATE_NONE)
	{ // �������Ȃ���Ԃł͂Ȃ��ꍇ

		//----------------------------------------------------
		//	�����_�[�X�e�[�g��ύX
		//----------------------------------------------------
		// ���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �|���S���̗��ʂ�\����Ԃɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (bUpdate)
		{ // �X�V����󋵂̏ꍇ

			//------------------------------------------------
			//	��Ԃ��Ƃ̐ݒ�
			//------------------------------------------------
			switch (m_state)
			{ // ��Ԃ��Ƃ̏���
			case STATE_NORMAL:	// �ʏ���

				// �e�}�g���b�N�X��ݒ�
				mtxParent = *m_orbit.pMtxParent;

				break;

			case STATE_VANISH:	// �������

				// �e�}�g���b�N�X��ݒ�
				mtxParent = m_orbit.mtxVanish;

				// �J�E���^�[�����Z
				if (m_nCounterState < (m_nNumVtx / MAX_OFFSET) + 1)
				{ // �J�E���^�[���O�Ղ��L�ѐ؂鎞�Ԃ�菬�����ꍇ

					// �J�E���^�[�����Z
					m_nCounterState++;
				}
				else
				{ // �J�E���^�[���O�Ղ��L�ѐ؂鎞�Ԉȏ�̏ꍇ

					// �J�E���^�[��␳
					m_nCounterState = 0;

					// ��Ԃ�ݒ�
					m_state = STATE_NONE;	// �������Ȃ����
				}

				break;

			default:	// ��O���
				assert(false);
				break;
			}

			//------------------------------------------------
			//	�I�t�Z�b�g�̏�����
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // �I�t�Z�b�g�̐����J��Ԃ�

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&m_orbit.aMtxWorldPoint[nCntOff]);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&m_orbit.aMtxWorldPoint[nCntOff], m_orbit.aOffset[nCntOff].x, m_orbit.aOffset[nCntOff].y, m_orbit.aOffset[nCntOff].z);

				// �e�̃}�g���b�N�X�Ɗ|�����킹��
				D3DXMatrixMultiply(&m_orbit.aMtxWorldPoint[nCntOff], &m_orbit.aMtxWorldPoint[nCntOff], &mtxParent);
			}

			//------------------------------------------------
			//	���_���W�ƒ��_�J���[�̏������炷
			//------------------------------------------------
			for (int nCntVtx = m_nNumVtx - 1; nCntVtx >= MAX_OFFSET; nCntVtx--)
			{ // �ێ����钸�_�̍ő吔���J��Ԃ� (�I�t�Z�b�g���͊܂܂Ȃ�)

				// ���_�������炷
				m_orbit.pPosPoint[nCntVtx] = m_orbit.pPosPoint[nCntVtx - MAX_OFFSET];
				m_orbit.pColPoint[nCntVtx] = m_orbit.pColPoint[nCntVtx - MAX_OFFSET];
			}

			//------------------------------------------------
			//	�ŐV�̒��_���W�ƒ��_�J���[�̏���ݒ�
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // �I�t�Z�b�g�̐����J��Ԃ�

				// ���_���W�̐ݒ�
				m_orbit.pPosPoint[nCntOff] = D3DXVECTOR3
				( // ����
					m_orbit.aMtxWorldPoint[nCntOff]._41,	// x
					m_orbit.aMtxWorldPoint[nCntOff]._42,	// y
					m_orbit.aMtxWorldPoint[nCntOff]._43		// z
				);

				// ���_�J���[�̐ݒ�
				m_orbit.pColPoint[nCntOff] = m_orbit.aCol[nCntOff];
			}
		}

		//----------------------------------------------------
		//	���_���W�ƒ��_�J���[�̏���������
		//----------------------------------------------------
		if (!m_orbit.bInit)
		{ // �������ς݂ł͂Ȃ��ꍇ

			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{ // �ێ����钸�_�̍ő吔���J��Ԃ�

				// ���_���W�̐ݒ�
				m_orbit.pPosPoint[nCntVtx] = D3DXVECTOR3
				( // ����
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._41,	// x
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._42,	// y
					m_orbit.aMtxWorldPoint[nCntVtx % MAX_OFFSET]._43	// z
				);

				// ���_�J���[�̐ݒ�
				m_orbit.pColPoint[nCntVtx] = m_orbit.aCol[nCntVtx % MAX_OFFSET];
			}

			// �������ς݂ɂ���
			m_orbit.bInit = true;
		}

		//----------------------------------------------------
		//	�|���S���̕`��
		//----------------------------------------------------
		// ���_���̐ݒ�
		SetVtx();

		// �P�ʃ}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtxIdent);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

		//----------------------------------------------------
		//	�����_�[�X�e�[�g�����ɖ߂�
		//----------------------------------------------------
		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �|���S���̕\�ʂ݂̂�\����Ԃɂ���
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

//============================================================
//	�e�N�X�`���������� (�C���f�b�N�X)
//============================================================
void CObjectOrbit::BindTexture(const int nTextureID)
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
void CObjectOrbit::BindTexture(const char *pTexturePass)
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
//	�F�̐ݒ菈��
//============================================================
void CObjectOrbit::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		// �����̐F��ݒ�
		m_orbit.aCol[nCntOff] = rCol;
	}
}

//============================================================
//	��Ԏ擾����
//============================================================
int CObjectOrbit::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	D3DXMATRIX *pMtxParent,	// �e�}�g���b�N�X
	const D3DXCOLOR& rCol,	// �F
	const EOffset offset,	// �I�t�Z�b�g
	const int nPart,		// ������
	const int nTexPart,		// �e�N�X�`��������
	const bool bAlpha		// ��������
)
{
	// �|�C���^��錾
	CObjectOrbit *pObjectOrbit = NULL;		// �I�u�W�F�N�g�O�Ր����p

	if (pObjectOrbit == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pObjectOrbit = new CObjectOrbit;	// �I�u�W�F�N�g�O��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pObjectOrbit != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �I�u�W�F�N�g�O�Ղ̏�����
		if (FAILED(pObjectOrbit->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �e�̃}�g���b�N�X��ݒ�
		pObjectOrbit->SetMatrixParent(pMtxParent);

		// �F��ݒ�
		pObjectOrbit->SetColor(rCol);

		// �I�t�Z�b�g��ݒ�
		pObjectOrbit->SetOffset(offset);

		// �e�N�X�`����������ݒ�
		pObjectOrbit->SetTexPart(nTexPart);

		// �������󋵂�ݒ�
		pObjectOrbit->SetEnableAlpha(bAlpha);

		// ������ݒ�
		if (FAILED(pObjectOrbit->SetLength(nPart)))
		{ // �����̐ݒ�Ɏ��s�����ꍇ

			// �������J��
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pObjectOrbit;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�e�}�g���b�N�X�폜����
//============================================================
void CObjectOrbit::DeleteMatrixParent(void)
{
	// ������Ԃɂ���
	SetState(STATE_VANISH);

	// �e�}�g���b�N�X��NULL�ɂ���
	m_orbit.pMtxParent = NULL;
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CObjectOrbit::SetState(const EState state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // �ݒ肷���Ԃ����݂̏�Ԋ��A�ݒ肷���Ԃ��ʏ��Ԃ̏ꍇ

		// �����𔲂���
		return;
	}

	// �����̏�Ԃ�ݒ�
	m_state = state;

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_NORMAL:	// �ʏ���

		// ���������Ă��Ȃ���Ԃɂ���
		m_orbit.bInit = false;

		break;

	case STATE_VANISH:	// �������

		// ���݂̐e�}�g���b�N�X����������}�g���b�N�X�ɐݒ�
		m_orbit.mtxVanish = *m_orbit.pMtxParent;

		break;

	default:	// ��O���
		assert(false);
		break;
	}
}

//============================================================
//	�e�̃}�g���b�N�X�̐ݒ菈��
//============================================================
void CObjectOrbit::SetMatrixParent(D3DXMATRIX *pMtxParent)
{
	// �����̐e�}�g���b�N�X��ݒ�
	m_orbit.pMtxParent = pMtxParent;
}

//============================================================
//	�I�t�Z�b�g�̐ݒ菈��
//============================================================
void CObjectOrbit::SetOffset(const EOffset offset)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // �I�t�Z�b�g�̐����J��Ԃ�

		// �����̃I�t�Z�b�g��ݒ�
		m_orbit.aOffset[nCntOff] = mc_aOffset[(int)offset][nCntOff];
	}
}

//============================================================
//	�e�N�X�`���������̐ݒ菈��
//============================================================
void CObjectOrbit::SetTexPart(const int nTexPart)
{
	// ��O����
	assert(nTexPart > 0);	// 0�ȉ��G���[

	// �����̃e�N�X�`����������ݒ�
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	�������󋵂̐ݒ菈��
//============================================================
void CObjectOrbit::SetEnableAlpha(const bool bAlpha)
{
	// �����̓������󋵂�ݒ�
	m_orbit.bAlpha = bAlpha;
}

//============================================================
//	�������󋵂̐ݒ菈��
//============================================================
void CObjectOrbit::SetEnableInit(const bool bInit)
{
	// �����̏������󋵂�ݒ�
	m_orbit.bInit = bInit;
}

//============================================================
//	�����̐ݒ菈��
//============================================================
HRESULT CObjectOrbit::SetLength(const int nPart)
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �K�v���_�������߂�
	m_nNumVtx = nPart * MAX_OFFSET;

	//--------------------------------------------------------
	//	�e���_���W�̔j���E����
	//--------------------------------------------------------
	// �e���_���W�̔j��
	if (m_orbit.pPosPoint != NULL)
	{ // �e���_���W���g�p���̏ꍇ

		// �������J��
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// �e���_���W�̏���ݒ�
	if (m_orbit.pPosPoint == NULL)
	{ // ��g�p���̏ꍇ

		// �e���_���W�̃������m��
		m_orbit.pPosPoint = new D3DXVECTOR3[m_nNumVtx];

		if (m_orbit.pPosPoint != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_orbit.pPosPoint, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	�e���_�J���[�̔j���E����
	//--------------------------------------------------------
	// �e���_�J���[�̔j��
	if (m_orbit.pColPoint != NULL)
	{ // �e���_�J���[���g�p���̏ꍇ

		// �������J��
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// �e���_�J���[�̏���ݒ�
	if (m_orbit.pColPoint == NULL)
	{ // ��g�p���̏ꍇ

		// �e���_�J���[�̃������m��
		m_orbit.pColPoint = new D3DXCOLOR[m_nNumVtx];

		if (m_orbit.pColPoint != NULL)
		{ // �m�ۂɐ��������ꍇ

			// �������N���A
			memset(m_orbit.pColPoint, 0, sizeof(D3DXCOLOR) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// �m�ێ��s
	}
	else { assert(false); return E_FAIL; }	// �g�p��

	//--------------------------------------------------------
	//	���_�o�b�t�@�̔j���E����
	//--------------------------------------------------------
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{ // ���_�o�b�t�@���g�p���̏ꍇ

		// �������J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// ���_�o�b�t�@�̏���ݒ�
	if (m_pVtxBuff == NULL)
	{ // ��g�p���̏ꍇ

		// ���_�o�b�t�@�̐���
		if (FAILED(pDevice->CreateVertexBuffer
		( // ����
			sizeof(VERTEX_3D) * m_nNumVtx,	// �K�v���_��
			D3DUSAGE_WRITEONLY,	// �g�p���@
			FVF_VERTEX_3D,		// ���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,	// �������̎w��
			&m_pVtxBuff,		// ���_�o�b�t�@�ւ̃|�C���^
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
//	���_���̐ݒ菈��
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// �|�C���^��錾
	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	if (m_pVtxBuff != NULL)
	{ // �O�Ղ̒��_�o�b�t�@���g�p���̏ꍇ

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{ // �ێ����钸�_�̍ő吔���J��Ԃ�

			// ���_���W�̐ݒ�
			pVtx[0].pos = m_orbit.pPosPoint[nCntVtx];

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			if (m_orbit.bAlpha)
			{ // �����ɂ��Ă����ꍇ

				pVtx[0].col = D3DXCOLOR
				( // ����
					m_orbit.pColPoint[nCntVtx].r,	// r
					m_orbit.pColPoint[nCntVtx].g,	// g
					m_orbit.pColPoint[nCntVtx].b,	// b
					m_orbit.pColPoint[nCntVtx].a - (m_orbit.pColPoint[nCntVtx].a / (m_nNumVtx * 0.5f)) * (nCntVtx / 2)	// a
				);
			}
			else
			{ // �����x��ς��Ȃ��ꍇ

				pVtx[0].col = m_orbit.pColPoint[nCntVtx];
			}

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			( // ����
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// ���_�f�[�^�̃|�C���^��i�߂�
			pVtx += 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// ��g�p��
}

//============================================================
//	�j������
//============================================================
void CObjectOrbit::Release(void)
{
	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
