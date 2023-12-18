//============================================================
//
//	�A�j���[�V����2D�w�b�_�[ [Ready.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _Ready_H_
#define _Ready_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object2D.h"
#include "anim2D.h"
//************************************************************
//	�N���X��`
//************************************************************
// �A�j���[�V����2D�N���X
class CReady : public CObject
{
public:
	// �R���X�g���N�^
	CReady();
	explicit CReady(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CReady();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	

	// �ÓI�����o�֐�
	static CReady *Create( 	);

	// �����o�֐�


private:
	void Release();
	// �����o�ϐ�
	int m_nStateCount;
	CObject2D * m_pTexReady;
	CObject2D * m_pTexFight;
	CAnim2D * m_pFlare;
};

#endif	// _Ready_H_
