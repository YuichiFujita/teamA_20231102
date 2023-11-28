//============================================================
//
//	���[�V�������� [motion.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "motion.h"
#include "multiModel.h"

//************************************************************
//	�}�N����`
//************************************************************
#define SUB_STOP	(2)	// ���[�v���Ȃ����[�V�����̒�~�J�E���g�̌��Z�p

//************************************************************
//	�e�N���X [CMotion] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CMotion::CMotion()
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
	m_ppModel = NULL;	// ���f�����
	m_nNumModel = 0;	// ���f���̃p�[�c��
	m_bUpdate = true;	// �X�V��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CMotion::~CMotion()
{

}

//============================================================
//	����������
//============================================================
HRESULT CMotion::Init(void)
{
	// �����o�ϐ����N���A
	memset(&m_info, 0, sizeof(m_info));	// ���[�V�������
	m_ppModel = NULL;	// ���f�����
	m_nNumModel = 0;	// ���f���̃p�[�c��
	m_bUpdate = true;	// �X�V��

	// ���[�V�������I����Ԃɂ���
	m_info.bFinish = true;

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CMotion::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CMotion::Update(void)
{
	// �ϐ���錾
	D3DXVECTOR3 diffPos;		// ���|�[�Y�܂ł̍����ʒu
	D3DXVECTOR3 diffRot;		// ���|�[�Y�܂ł̍�������
	D3DXVECTOR3 currentPos;		// ���݃t���[���̈ʒu
	D3DXVECTOR3 currentRot;		// ���݃t���[���̌���
	int nType = m_info.nType;	// ���[�V�������
	int nPose = m_info.nPose;	// ���[�V�����|�[�Y�ԍ�
	int nNextPose;				// ���̃��[�V�����|�[�Y�ԍ�

	if (m_bUpdate)
	{ // �X�V����󋵂̏ꍇ

		if (m_info.aMotionInfo[nType].nNumKey > 0)
		{ // �L�[���ݒ肳��Ă���ꍇ

			// ���̃��[�V�����|�[�Y�ԍ������߂�
			nNextPose = (nPose + 1) % m_info.aMotionInfo[nType].nNumKey;

			// �p�[�c�̈ʒu�̍X�V
			for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
			{ // ���f���̃p�[�c�����J��Ԃ�

				// �ʒu�E�����̍��������߂�
				diffPos = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].pos - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos;
				diffRot = m_info.aMotionInfo[nType].aKeyInfo[nNextPose].aKey[nCntKey].rot - m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot;

				// ���������̐��K��
				useful::NormalizeRot(diffRot.x);
				useful::NormalizeRot(diffRot.y);
				useful::NormalizeRot(diffRot.z);

				// ���݂̃p�[�c�̈ʒu�E�������X�V
				m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].pos + diffPos * ((float)m_info.nCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame));
				m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[nPose].aKey[nCntKey].rot + diffRot * ((float)m_info.nCounter / (float)m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame));
			}

			// ���[�V�����̑J�ڂ̍X�V
			if (m_info.nCounter >= m_info.aMotionInfo[nType].aKeyInfo[nPose].nFrame)
			{ // ���݂̃��[�V�����J�E���^�[�����݂̃|�[�Y�̍Đ��t���[�����𒴂��Ă���ꍇ

				// ���̃|�[�Y�Ɉڍs
				if (m_info.aMotionInfo[nType].bLoop == true)
				{ // ���[�V���������[�v����ݒ�̏ꍇ

					// ���[�V�����J�E���^�[��������
					m_info.nCounter = 0;

					// �|�[�Y�J�E���g�����Z (�����ɒB�����ꍇ 0�ɖ߂�)
					m_info.nPose = (m_info.nPose + 1) % m_info.aMotionInfo[nType].nNumKey;
				}
				else
				{ // ���[�V���������[�v���Ȃ��ݒ�̏ꍇ

					if (m_info.nPose < m_info.aMotionInfo[nType].nNumKey - SUB_STOP)
					{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�ł͂Ȃ��ꍇ

						// ���[�V�����J�E���^�[��������
						m_info.nCounter = 0;

						// �|�[�Y�J�E���g�����Z
						m_info.nPose++;
					}
					else
					{ // ���݂̃|�[�Y���ŏI�̃|�[�Y�̏ꍇ

						// ���[�V�������I����Ԃɂ���
						m_info.bFinish = true;
					}
				}
			}
			else
			{ // ���݂̃��[�V�����J�E���^�[�����݂̃|�[�Y�̍Đ��t���[�����𒴂��Ă��Ȃ��ꍇ

				// ���[�V�����J�E���^�[�����Z
				m_info.nCounter++;
			}
		}
	}
}

//============================================================
//	�ݒ菈��
//============================================================
void CMotion::Set(const int nType)
{
	// ���[�V��������������
	m_info.nPose	= 0;		// ���[�V�����|�[�Y�ԍ�
	m_info.nCounter	= 0;		// ���[�V�����J�E���^�[
	m_info.bFinish	= false;	// ���[�V�����I����

	// �����̃��[�V�����̎�ނ�ݒ�
	m_info.nType = nType;

	// ���[�V�������Đ���Ԃɂ���
	m_info.bFinish = false;

	// �p�[�c�̈ʒu�̏�����
	for (int nCntKey = 0; nCntKey < m_nNumModel; nCntKey++)
	{ // ���f���̃p�[�c�����J��Ԃ�

		// �����ʒu�Ə���������ݒ�
		m_ppModel[nCntKey]->SetVec3Position(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].pos);
		m_ppModel[nCntKey]->SetVec3Rotation(m_info.aMotionInfo[nType].aKeyInfo[m_info.nPose].aKey[nCntKey].rot);
	}
}

//============================================================
//	���[�V�������̐ݒ菈��
//============================================================
void CMotion::SetInfo(const SMotionInfo info)
{
	// �����̃��[�V��������ݒ�
	m_info.aMotionInfo[m_info.nNumMotion] = info;

	// ���[�V�����̏�񐔂����Z
	m_info.nNumMotion++;

	// ��O����
	assert(m_info.nNumMotion <= MAX_MOTION);	// ���[�V�������I�[�o�[
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CMotion::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	m_bUpdate = bUpdate;
}

//============================================================
//	���f�����̐ݒ菈��
//============================================================
void CMotion::SetModel(CMultiModel **ppModel, const int nNum)
{
	// �����̃��f������ݒ�
	m_ppModel = ppModel;

	// �����̃��f���̃p�[�c����ݒ�
	m_nNumModel = nNum;

	// ��O����
	assert(m_nNumModel <= MAX_PARTS);	// �p�[�c���I�[�o�[
}

//============================================================
//	��ގ擾����
//============================================================
int CMotion::GetType(void) const
{
	// ���݂̃��[�V�����̎�ނ�Ԃ�
	return m_info.nType;
}

//============================================================
//	�|�[�Y�ԍ��擾����
//============================================================
int CMotion::GetPose(void) const
{
	// ���݂̃|�[�Y�ԍ���Ԃ�
	return m_info.nPose;
}

//============================================================
//	���[�V�����J�E���^�[�擾����
//============================================================
int CMotion::GetCounter(void) const
{
	// ���݂̃��[�V�����J�E���^�[��Ԃ�
	return m_info.nCounter;
}

//============================================================
//	�I���擾����
//============================================================
bool CMotion::IsFinish(void) const
{
	// ���݂̃��[�V�����̏I���󋵂�Ԃ�
	return m_info.bFinish;
}

//============================================================
//	���[�v�擾����
//============================================================
bool CMotion::IsLoop(const int nType) const
{
	// ���݂̃��[�v��ON/OFF�󋵂�Ԃ�
	return m_info.aMotionInfo[nType].bLoop;
}

//============================================================
//	��������
//============================================================
CMotion *CMotion::Create(void)
{
	// �|�C���^��錾
	CMotion *pMotion = NULL;	// ���[�V���������p

	if (pMotion == NULL)
	{ // �g�p����Ă��Ȃ��ꍇ

		// �������m��
		pMotion = new CMotion;	// ���[�V����
	}
	else { assert(false); return NULL; }	// �g�p��

	if (pMotion != NULL)
	{ // �m�ۂɐ������Ă���ꍇ

		// ���[�V�����̏�����
		if (FAILED(pMotion->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �������J��
			delete pMotion;
			pMotion = NULL;

			// ���s��Ԃ�
			return NULL;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pMotion;
	}
	else { assert(false); return NULL; }	// �m�ێ��s
}

//============================================================
//	�j������
//============================================================
HRESULT CMotion::Release(CMotion *&prMotion)
{
	if (prMotion != NULL)
	{ // �g�p���̏ꍇ

		// ���[�V�����̏I��
		prMotion->Uninit();

		// �������J��
		delete prMotion;
		prMotion = NULL;

		// ������Ԃ�
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// ��g�p��
}
