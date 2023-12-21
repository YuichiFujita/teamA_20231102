//============================================================
//
//	����ʒu�w�b�_�[ [controlPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _CONTROL_POINT_H_
#define _CONTROL_POINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object3D.h"

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
class CObjectBillboard;	// �I�u�W�F�N�g�r���{�[�h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ����ʒu�N���X
class CControlPoint : public CObject3D
{
public:
	// �e�N�X�`����
	enum ETexture
	{
		TEXTURE_FIELD = 0,	// �n�ʃe�N�X�`��
		TEXTURE_CONTROL,	// ����e�N�X�`��
		TEXTURE_MAX			// ���̗񋓌^�̑���
	};

	// ���b�X����
	enum ELesson
	{
		LESSON_YOBIKOMI = 0,	// �Ăэ��݃e�N�X�`��
		LESSON_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CControlPoint();

	// �f�X�g���N�^
	~CControlPoint();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�

	// �ÓI�����o�֐�
	static CControlPoint *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rSize	// �傫��
	);

private:
	// �����o�֐�
	bool Collision(void);				// �v���C���[�S���Ƃ̓����蔻��
	void UpdateTutorial(const int nID);	// �`���[�g���A������
	void UpdateLesson(void);			// ���b�X���X�V

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[];	// �e�N�X�`���萔
	static const char *mc_apLessonFile[];	// ���b�X���e�N�X�`���萔

	// �����o�ϐ�
	CObjectBillboard *m_pTutorial;	// �`���[�g���A���\�����
	CObjectBillboard *m_pControl;	// ����\�����
	float m_fScale;		// �g�嗦
	float m_fDestScale;	// �ڕW�g�嗦
};

#endif	// _CONTROL_POINT_H_
