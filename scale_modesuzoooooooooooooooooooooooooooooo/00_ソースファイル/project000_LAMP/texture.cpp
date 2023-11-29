//============================================================
//
//	�e�N�X�`������ [texture.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	�}�N����`
//************************************************************
#define TEXTURE_SETUP_TXT	"data\\TXT\\texture.txt"	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
int CTexture::m_nNumAll = 0;	// �e�N�X�`���̑���

//************************************************************
//	�e�N���X [CTexture] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTexture::CTexture()
{
	// �e�N�X�`���ւ̃|�C���^���N���A
	memset(&m_apTexture[0], 0, sizeof(m_apTexture));

	// �ǂݍ��񂾃e�N�X�`���t�@�C�������N���A
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// NULL�������R�s�[
		strcpy(&m_pFileName[nCntTexture][0], NONE_STRING);
	}
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	�e�N�X�`����������
//============================================================
HRESULT CTexture::Load(void)
{
	// �e�N�X�`���ւ̃|�C���^��������
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// NULL����
		m_apTexture[nCntTexture] = NULL;
	}

	// �ǂݍ��񂾃e�N�X�`���t�@�C������������
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// NULL�������R�s�[
		strcpy(&m_pFileName[nCntTexture][0], NONE_STRING);
	}

	// �Z�b�g�A�b�v�̓Ǎ�
	LoadSetup();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�e�N�X�`���j������
//============================================================
void CTexture::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTexture = 0; nCntTexture < MAX_TEXTURE; nCntTexture++)
	{ // �e�N�X�`���̍ő吔���J��Ԃ�

		// �e�N�X�`���̔j��
		if (m_apTexture[nCntTexture] != NULL)
		{ // �e�N�X�`�����g�p���̏ꍇ

			// �������J��
			m_apTexture[nCntTexture]->Release();
			m_apTexture[nCntTexture] = NULL;
		}
	}
}

//============================================================
//	�e�N�X�`���o�^���� (����)
//============================================================
int CTexture::Regist(const SInfo info)
{
	// �ϐ���錾
	HRESULT hr;				// �ُ�I���̊m�F�p
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (m_nNumAll >= MAX_TEXTURE)
	{ // �e�N�X�`���I�[�o�[�̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}

	// ��̃e�N�X�`���𐶐�
	hr = D3DXCreateTexture
	( // ����
		pDevice,			// Direct3D�f�o�C�X
		info.Width,			// �e�N�X�`������
		info.Height,		// �e�N�X�`���c��
		info.MipLevels,		// �~�b�v�}�b�v���x��
		info.Usage,			// �����E�m�ۃI�v�V����
		info.Format,		// �s�N�Z���t�H�[�}�b�g
		info.Pool,			// �i�[������
		&m_apTexture[nID]	// �e�N�X�`���ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �e�N�X�`���̐����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}

	// �e�N�X�`���t�@�C�����Ȃ���ۑ�
	strcpy(&m_pFileName[nID][0], NONE_STRING);

	// �e�N�X�`���̑��������Z
	m_nNumAll++;

	// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
	return nID;
}

//============================================================
//	�e�N�X�`���o�^���� (�p�X)
//============================================================
int CTexture::Regist(const char *pFileName)
{
	// �ϐ���錾
	int nID = m_nNumAll;	// �e�N�X�`���Ǎ��ԍ�

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (pFileName != NULL)
	{ // �|�C���^���g�p����Ă���ꍇ

		for (int nCntTexture = 0; nCntTexture < m_nNumAll; nCntTexture++)
		{ // �e�N�X�`���̑������J��Ԃ�

			if (strcmp(&m_pFileName[nCntTexture][0], pFileName) == 0)
			{ // �����񂪈�v�����ꍇ

				// ���łɓǂݍ���ł���e�N�X�`���̔z��ԍ���Ԃ�
				return nCntTexture;
			}
		}

		if (m_nNumAll >= MAX_TEXTURE)
		{ // �e�N�X�`���I�[�o�[�̏ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �e�N�X�`���̓ǂݍ���
		if (FAILED(D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[nID])))
		{ // �e�N�X�`���̓ǂݍ��݂Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}

		// �e�N�X�`���̃t�@�C������ۑ�
		strcpy(&m_pFileName[nID][0], pFileName);

		// �e�N�X�`���̑��������Z
		m_nNumAll++;

		// �ǂݍ��񂾃e�N�X�`���̔z��ԍ���Ԃ�
		return nID;
	}
	else
	{ // �|�C���^���g�p����Ă��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return NONE_IDX;
	}
}

//============================================================
//	�e�N�X�`�����̎擾����
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(int nID)
{
	if (nID >= 0 && nID < m_nNumAll)
	{ // �����̃C���f�b�N�X���͈͓��̏ꍇ

		// �����̃e�N�X�`���|�C���^��Ԃ�
		return m_apTexture[nID];
	}
	else if (nID == NONE_IDX)
	{ // �����̃C���f�b�N�X�� -1�̏ꍇ

		// NULL��Ԃ�
		return NULL;
	}
	else
	{ // �����̃C���f�b�N�X���g�p�s�̏ꍇ

		// NULL��Ԃ�
		assert(false);
		return NULL;
	}
}

//============================================================
//	��������
//============================================================
CTexture *CTexture::Create(void)
{
	// �|�C���^��錾
	CTexture *pTexture = NULL;	// �e�N�X�`�������p

	if (pTexture == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// ���������m��
		pTexture = new CTexture;	// �e�N�X�`��
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pTexture != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// �e�N�X�`���̓Ǎ�
		if (FAILED(pTexture->Load()))
		{ // �e�N�X�`���ǂݍ��݂Ɏ��s�����ꍇ

			// �������J��
			delete pTexture;
			pTexture = NULL;

			// ���s��Ԃ�
			assert(false);
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTexture;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CTexture::Release(CTexture *&prTexture)
{
	if (prTexture != NULL)
	{ // �g�p���̏ꍇ

		// �e�N�X�`���̔j��
		prTexture->Unload();

		// �������J��
		delete prTexture;
		prTexture = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
void CTexture::LoadSetup(void)
{
	// �ϐ���錾
	int nEnd = 0;	// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �ϐ��z���錾
	char aString[MAX_STRING];	// �e�L�X�g�̕�����̑���p

	// �|�C���^��錾
	FILE *pFile;	// �t�@�C���|�C���^

	// �t�@�C����ǂݍ��݌`���ŊJ��
	pFile = fopen(TEXTURE_SETUP_TXT, "r");

	if (pFile != NULL)
	{ // �t�@�C�����J�����ꍇ

		do
		{ // �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �e�L�X�g��ǂݍ��݂������� EOF ��Ԃ�

			if (strcmp(&aString[0], "FILENAME") == 0)
			{ // �ǂݍ��񂾕����� FILENAME �̏ꍇ

				// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%s", &aString[0]);

				// �t�@�C���p�X��ǂݍ���
				fscanf(pFile, "%s", &aString[0]);

				// �e�N�X�`����o�^
				Regist(&aString[0]);
			}
		} while (nEnd != EOF);	// �ǂݍ��񂾕����� EOF �ł͂Ȃ��ꍇ���[�v
		
		// �t�@�C�������
		fclose(pFile);
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�e�N�X�`���Z�b�g�A�b�v�t�@�C���̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}
