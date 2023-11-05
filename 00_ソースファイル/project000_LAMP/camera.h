//============================================================
//
//	�J�����w�b�_�[ [camera.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�N���X��`
//************************************************************
// �J�����N���X
class CCamera
{
public:
	// �J������
	enum EType
	{
		TYPE_MAIN = 0,	// ���C���J����
		TYPE_MODELUI,	// ���f��UI�\���J����
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ����Ȃ����
		STATE_CONTROL,	// ������
		STATE_ROTATE,	// ��]���
		STATE_FOLLOW,	// �Ǐ]���
		STATE_ZOOM,		// �Y�[�����
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CCamera();

	// �f�X�g���N�^
	~CCamera();

	// �J�����\����
	struct SCamera
	{
		D3DXVECTOR3		posV;			// ���݂̎��_
		D3DXVECTOR3		posR;			// ���݂̒����_
		D3DXVECTOR3		destPosV;		// �ڕW�̎��_
		D3DXVECTOR3		destPosR;		// �ڕW�̒����_
		D3DXVECTOR3		vecU;			// ������x�N�g��
		D3DXVECTOR3		rot;			// ���݂̌���
		D3DXVECTOR3		destRot;		// �ڕW�̌���
		float			fDis;			// ���݂̎��_�ƒ����_�̋���
		float			fDestDis;		// �ڕW�̎��_�ƒ����_�̋���
		D3DXMATRIX		mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX		mtxView;		// �r���[�}�g���b�N�X
		D3DVIEWPORT9	viewport;		// �r���[�|�[�g
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SetCamera(const EType type);		// �J�����ݒ�
	SCamera GetCamera(const EType type);	// �J�����擾
	void SetState(const EState state);		// �J������Ԑݒ�
	EState GetState(void) const;			// �J������Ԏ擾

	void SetDestRotate(void);	// �J�����ڕW�ʒu�ݒ� (��])
	void SetDestFollow(void);	// �J�����ڕW�ʒu�ݒ� (�Ǐ])
	void SetEnableUpdate(const bool bUpdate);			// �X�V�󋵐ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot);		// ���݌����ݒ�
	void SetVec3DestRotation(const D3DXVECTOR3& rRot);	// �ڕW�����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const;			// ���݌����擾
	D3DXVECTOR3 GetVec3DestRotation(void) const;		// �ڕW�����擾

	// �ÓI�����o�֐�
	static CCamera *Create(void);				// ����
	static HRESULT Release(CCamera *&prCamera);	// �j��

private:
	// �����o�֐�
	void Rotate(void);		// �J�����̍X�V (��])
	void Follow(void);		// �J�����̍X�V (�Ǐ])
	void Zoom(void);		// �J�����̍X�V (�Y�[��)
	void Control(void);		// �J�����̍X�V (����)
	void Move(void);		// �ʒu�̍X�V (����)
	void Distance(void);	// �����̍X�V (����)
	void Rotation(void);	// �����̍X�V (����)

	// �����o�ϐ�
	SCamera m_aCamera[TYPE_MAX];	// �J�����̏��
	EState m_state;	// ���
	bool m_bUpdate;	// �X�V��
};

#endif	// _CAMERA_H_
