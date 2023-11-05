//============================================================
//
//	�X�N���[�����b�V���t�B�[���h�w�b�_�[ [scrollMeshField.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCROLL_MESHFIELD_H_
#define _SCROLL_MESHFIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "objectMeshField.h"

//************************************************************
//	�N���X��`
//************************************************************
// �X�N���[�����b�V���t�B�[���h�N���X
class CScrollMeshField : public CObjectMeshField
{
public:
	// �R���X�g���N�^
	CScrollMeshField();
	CScrollMeshField(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CScrollMeshField();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	void SetVec2Sizing(const D3DXVECTOR2& rSize) override;	// �傫���ݒ�
	void SetColor(const D3DXCOLOR& rCol) override;			// �F�ݒ�

	// �I�[�o�[���[�h�����I�[�o�[���C�h�֐�
	HRESULT Init	// ������
	( // ����
		const float fMoveU,	// �����W�̈ړ���
		const float fMoveV	// �c���W�̈ړ���
	);

	// �ÓI�����o�֐�
	static CScrollMeshField *Create	// ����
	( // ����
		const float fMoveU,			// �����W�̈ړ���
		const float fMoveV,			// �c���W�̈ړ���
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXVECTOR2& rSize,	// �傫��
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const D3DCULL cull = D3DCULL_CCW,	// �J�����O��
		const bool bLight = true			// ���C�e�B���O��
	);

	// �����o�֐�
	HRESULT SetPattern(const POSGRID2& rPart);	// �������ݒ�

	void SetTexU(const float fTexU);	// �����W�̊J�n�ʒu�ݒ�
	void SetTexV(const float fTexV);	// �c���W�̊J�n�ʒu�ݒ�
	void SetMoveU(const float fMoveU);	// �����W�̈ړ��ʐݒ�
	void SetMoveV(const float fMoveV);	// �c���W�̈ړ��ʐݒ�

private:
	// �����o�ϐ�
	float m_fTexU;	// �e�N�X�`�������W�̊J�n�ʒu
	float m_fTexV;	// �e�N�X�`���c���W�̊J�n�ʒu
	float m_fMoveU;	// �e�N�X�`�������W�̈ړ���
	float m_fMoveV;	// �e�N�X�`���c���W�̈ړ���
};

#endif	// _SCROLL_MESHFIELD_H_
