//============================================================
//
//	�t�̃w�b�_�[ [liquid.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIQUID_H_
#define _LIQUID_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CScrollMeshField;	// �X�N���[�����b�V���t�B�[���h�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �t�̃N���X
class CLiquid : public CObject
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SEA = 0,	// �t��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �㉺��
	enum ELiquid
	{
		LIQUID_LOW = 0,	// ��
		LIQUID_HIGH,	// ��
		LIQUID_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLiquid();

	// �f�X�g���N�^
	~CLiquid();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void Draw(void) override;		// �`��

	// �ÓI�����o�֐�
	static CLiquid *Create(void);	// ����

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override;	// �j��

	// �ÓI�����o�ϐ�
	static const char *mc_apTextureFile[][LIQUID_MAX];	// �e�N�X�`���萔

	// �����o�ϐ�
	CScrollMeshField *m_apLiquid[LIQUID_MAX];	// �t�̂̏��
	float m_fSinRot;	// �g�ł�����
};

#endif	// _LIQUID_H_
