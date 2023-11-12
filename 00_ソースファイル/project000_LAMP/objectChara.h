//============================================================
//
//	�I�u�W�F�N�g�L�����N�^�[�w�b�_�[ [objectChara.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"
#include "motion.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiModel;	// �}���`���f���N���X
class CMotion;		// ���[�V�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�L�����N�^�[�N���X
class CObjectChara : public CObject
{
public:
	// �R���X�g���N�^
	CObjectChara();
	explicit CObjectChara(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectChara();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾
	D3DXMATRIX *GetPtrMtxWorld(void) override;				// �}�g���b�N�X�|�C���^�擾

	// �����o�֐�
	void SetPartsInfo	// �p�[�c���ݒ�
	( // ����
		const int nID,				// �p�[�c�C���f�b�N�X
		const int nParentID,		// �e�C���f�b�N�X
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const char *pFileName		// �t�@�C����
	);
	void SetModelInfo(void);						// ���f�����ݒ�
	void SetMotionInfo(CMotion::SMotionInfo info);	// ���[�V�������ݒ�
	void SetMotion(const int nType);				// ���[�V�����ݒ�
	void SetEnableMotionUpdate(const bool bUpdate);	// �X�V�󋵐ݒ�

	void SetPartsPosition(const int nID, const D3DXVECTOR3& rPos);	// �p�[�c�ʒu�ݒ�
	D3DXVECTOR3 GetPartsPosition(const int nID) const;				// �p�[�c�ʒu�擾
	void SetPartsRotation(const int nID, const D3DXVECTOR3& rRot);	// �p�[�c�����ݒ�
	D3DXVECTOR3 GetPartsRotation(const int nID) const;				// �p�[�c�����擾
	CMultiModel *GetMultiModel(const int nID) const;				// �}���`���f���擾

	int  GetMotionType(void) const;				// ���[�V������ގ擾
	int  GetMotionPose(void) const;				// ���[�V�����|�[�Y�ԍ��擾
	int  GetMotionCounter(void) const;			// ���[�V�����J�E���^�[�擾
	bool IsMotionFinish(void) const;			// ���[�V�����I���擾
	bool IsMotionLoop(const int nType) const;	// ���[�V�������[�v�擾
	void SetMaterial(const D3DXMATERIAL& rMat);	// �}�e���A���ݒ�
	void ResetMaterial(void);					// �}�e���A���Đݒ�
	void SetAlpha(const float fAlpha);			// �����x�ݒ�
	float GetAlpha(void) const;					// �����x�擾
	float GetMaxAlpha(void) const;				// �ő哧���x�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �����o�ϐ�
	CMultiModel	*m_apMultiModel[MAX_PARTS];	// ���f���̏��
	CMotion		*m_pMotion;		// ���[�V�����̏��
	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3	m_pos;			// �ʒu
	D3DXVECTOR3	m_rot;			// ����
	int			m_nNumModel;	// �p�[�c�̑���
};

#endif	// _OBJECTCHARA_H_
