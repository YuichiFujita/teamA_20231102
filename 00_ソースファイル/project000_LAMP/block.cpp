//============================================================
//
//	�u���b�N���� [block.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "sound.h"
#include "ModelParticle.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\TXT\\block.txt";	// �u���b�N�Z�b�g�A�b�v�e�L�X�g

#ifdef ON_STENCIL_PLAYER

	const int	PRIORITY = 4;	// �u���b�N�̗D�揇��

#else	// OFF_STENCIL_PLAYER

	const int	PRIORITY = 1;	// �u���b�N�̗D�揇��

#endif	// ON_STENCIL_PLAYER
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CBlock::SStatusInfo CBlock::m_aStatusInfo[TYPE_MAX] = {};	// �X�e�[�^�X���
const char *CBlock::mc_apTextureFile[][6] =	// �e�N�X�`���萔
{
	{ // �΃e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// ���e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// ���e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// ��e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\stone_001.png",	// ��e�N�X�`��
	},

	{ // ���e�N�X�`��
		"data\\TEXTURE\\block001.png",	// ���e�N�X�`��
		"data\\TEXTURE\\block001.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\block001.png",	// ���e�N�X�`��
		"data\\TEXTURE\\block001.png",	// ��e�N�X�`��
		"data\\TEXTURE\\block001.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\block001.png",	// ��e�N�X�`��
	},

	{ // �����K�e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// ���e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// ���e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// ��e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\Brick.png",	// ��e�N�X�`��
	},

	{ // �R���N���[�g�e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// ���e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// ���e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// ��e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\Concrete000.png",	// ��e�N�X�`��
	},

	{ // �X�e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// ���e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// �E�e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// ���e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// ��e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// �O�e�N�X�`��
		"data\\TEXTURE\\iceblock.png",	// ��e�N�X�`��
	},
};

//************************************************************
//	�q�N���X [CBlock] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlock::CBlock() : CObjectMeshCube(CObject::LABEL_BLOCK, PRIORITY)
{
	// �����o�ϐ����N���A
	memset(&m_status, 0, sizeof(m_status));	// �X�e�[�^�X
	m_type	= TYPE_STONE;	// ���
	m_nLife	= 0;			// �̗�
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlock::Init(void)
{
	// �����o�ϐ���������
	memset(&m_status, 0, sizeof(m_status));	// �X�e�[�^�X
	m_type	= TYPE_STONE;	// ���
	m_nLife	= 0;			// �̗�

	// �I�u�W�F�N�g���b�V���L���[�u�̏�����
	if (FAILED(CObjectMeshCube::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ���_��ݒ�
	SetOrigin(CObjectMeshCube::ORIGIN_DOWN);

	// �L���[�u�F��ݒ�
	SetColor(XCOL_WHITE);

	// �����F��ݒ�
	SetBorderColor(XCOL_WHITE);

	// �����̏�Ԃ�ݒ�
	if (FAILED(SetBorderState(CObjectMeshCube::BORDER_OFF)))
	{ // �������̐ݒ�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̑�����ݒ�
	SetBorderThick(0.0f);

	// �e�N�X�`���̏�Ԃ�ݒ�
	SetTextureState(CObjectMeshCube::TEXSTATE_SELECT);

	// �J�����O��ݒ�
	SetCulling(D3DCULL_CCW);

	// ���C�e�B���O��ݒ�
	SetLighting(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏I��
	CObjectMeshCube::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBlock::Update(void)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̍X�V
	CObjectMeshCube::Update();
}

//============================================================
//	�`�揈��
//============================================================
void CBlock::Draw(void)
{
	/*
		�o�b�N�o�b�t�@�������ݗp�̕`��
	*/
	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObjectMeshCube::Draw();

#ifdef ON_STENCIL_PLAYER

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	/*
		�X�e���V���o�b�t�@�������ݗp�̕`��
	*/
	// �X�e���V���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ��r�Q�ƒl��ݒ肷��
	pDevice->SetRenderState(D3DRS_STENCILREF, 1);

	// �X�e���V���}�X�N���w�肷�� 
	pDevice->SetRenderState(D3DRS_STENCILMASK, 255);

	// �X�e���V����r�֐����w�肷��
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// �X�e���V�����ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS,	D3DSTENCILOP_INCRSAT);	// Z�e�X�g�E�X�e���V���e�X�g����
	pDevice->SetRenderState(D3DRS_STENCILFAIL,	D3DSTENCILOP_KEEP);		// Z�e�X�g�E�X�e���V���e�X�g���s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL,	D3DSTENCILOP_KEEP);		// Z�e�X�g���s�E�X�e���V���e�X�g����

	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObjectMeshCube::Draw();

	// �X�e���V���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

#endif	// ON_STENCIL_PLAYER
}
//============================================================
//	�����蔻�菈��
//============================================================
void CBlock::Hit(void)
{
	if (IsDeath())
	{ // ���S�t���O�������Ă���ꍇ

		return;
	}
	
	//�j��ł���u���b�N��������
	if (m_status.state == EBreak::BREAK_TRUE)
	{
		// WATANABE�F�r���h����Ɩ������̊O���Q�Ƃ��Č���ꂽ����R�����g�A�E�g���Ă����܂���
		CModParticle::Create(D3DXVECTOR3(CObjectMeshCube::GetVec3Position().x, CObjectMeshCube::GetVec3Position().y + 50.0f,
			CObjectMeshCube::GetVec3Position().z), GetColor(), GetMeshCube().texID.Top);
		CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_HIT2);	// �q�b�g��

		//�I���������s��
		Uninit();
	}
}
//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CBlock::SetType(const int nType)
{
	// �ϐ���錾
	SFaceTex faceTex;	// �e�N�X�`���C���f�b�N�X�ݒ�p

	// �|�C���^��錾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	if (pTexture == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return;
	}

	if (nType > NONE_IDX && nType < TYPE_MAX)
	{ // ��ނ��͈͓��̏ꍇ

		// �����̎�ނ�ݒ�
		m_type = (EType)nType;

		// �����̎�ނ̃X�e�[�^�X����ݒ�
		m_status = m_aStatusInfo[nType];

		// �����̎�ނ̗̑͂�ݒ�
		m_nLife = m_status.nLife;

		// �����̎�ނ̃e�N�X�`����o�^
		faceTex = SFaceTex
		( // ����
			pTexture->Regist(mc_apTextureFile[nType][0]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][1]),	// �E
			pTexture->Regist(mc_apTextureFile[nType][2]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][3]),	// ��
			pTexture->Regist(mc_apTextureFile[nType][4]),	// �O
			pTexture->Regist(mc_apTextureFile[nType][5])	// ��
		);
		BindTexture(faceTex);	// �e�N�X�`��������
	}
	else { assert(false); }	// ��ރI�[�o�[
}

//============================================================
//	��ގ擾����
//============================================================
int CBlock::GetType(void) const
{
	// ��ނ�Ԃ�
	return m_type;
}

//============================================================
//	�j��󋵂̐ݒ菈��
//============================================================
void CBlock::SetState(const int nState)
{
	// �j��󋵂�ݒ�
	m_status.state = (EBreak)nState;
}

//============================================================
//	�j��󋵎擾����
//============================================================
int CBlock::GetState(void) const
{
	// �j��󋵂�Ԃ�
	return m_status.state;
}

//============================================================
//	��������
//============================================================
CBlock *CBlock::Create
(
	const EType type,				// ���
	const D3DXVECTOR3& rPos,		// �ʒu
	const D3DXVECTOR3& rRot,		// ����
	const D3DXVECTOR3& rSize,		// �傫��
	const D3DXVECTOR2& rTexPartX,	// �e�N�X�`��������X
	const D3DXVECTOR2& rTexPartY,	// �e�N�X�`��������Y
	const D3DXVECTOR2& rTexPartZ	// �e�N�X�`��������Z
)
{
	// �|�C���^��錾
	CBlock *pBlock = NULL;	// �u���b�N�����p

	if (pBlock == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pBlock = new CBlock;	// �u���b�N
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pBlock != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �u���b�N�̏�����
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pBlock;
			pBlock = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// ��ނ�ݒ�
		pBlock->SetType(type);

		// �ʒu��ݒ�
		pBlock->SetVec3Position(rPos);

		// ������ݒ�
		pBlock->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pBlock->SetVec3Sizing(rSize);

		// �e�N�X�`���̕�����X��ݒ�
		pBlock->SetTexturePatternX(rTexPartX);

		// �e�N�X�`���̕�����Y��ݒ�
		pBlock->SetTexturePatternY(rTexPartY);

		// �e�N�X�`���̕�����Z��ݒ�
		pBlock->SetTexturePatternZ(rTexPartZ);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlock;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CBlock::LoadSetup(void)
{
	// �ϐ���錾
	int nType = 0;	// ��ނ̑���p
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �X�e�[�^�X���̐ÓI�����o�ϐ���������
	memset(&m_aStatusInfo[0], 0, sizeof(m_aStatusInfo));

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			// �X�e�[�^�X�̐ݒ�
			if (strcmp(&aString[0], "STATUSSET") == 0)
			{ // �ǂݍ��񂾕����� STATUSSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "BLOCKSET") == 0)
					{ // �ǂݍ��񂾕����� BLOCKSET �̏ꍇ

						do
						{ // �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v

							// �t�@�C�����當�����ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "TYPE") == 0)
							{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
							}
							else if (strcmp(&aString[0], "BREAK") == 0)
							{ // �ǂݍ��񂾕����� BREAK �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].state);	// �j��󋵂�ǂݍ���
							}
							else if (strcmp(&aString[0], "LIFE") == 0)
							{ // �ǂݍ��񂾕����� LIFE �̏ꍇ

								fscanf(pFile, "%s", &aString[0]);					// = ��ǂݍ��� (�s�v)
								fscanf(pFile, "%d", &m_aStatusInfo[nType].nLife);	// �̗͂�ǂݍ���
							}
						} while (strcmp(&aString[0], "END_BLOCKSET") != 0);	// �ǂݍ��񂾕����� END_BLOCKSET �ł͂Ȃ��ꍇ���[�v
					}
				} while (strcmp(&aString[0], "END_STATUSSET") != 0);	// �ǂݍ��񂾕����� END_STATUSSET �ł͂Ȃ��ꍇ���[�v
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�u���b�N�Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
