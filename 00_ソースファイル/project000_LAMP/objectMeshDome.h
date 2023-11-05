//============================================================
//
//	�I�u�W�F�N�g���b�V���h�[���w�b�_�[ [objectMeshDome.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_MESHDOME_H_
#define _OBJECT_MESHDOME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g���b�V���h�[���N���X
class CObjectMeshDome : public CObject
{
public:
	// �R���X�g���N�^
	CObjectMeshDome();
	CObjectMeshDome(const CObject::ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	~CObjectMeshDome();

	// ���b�V���h�[���\����
	struct SMeshDome
	{
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3	rot;		// ����
		D3DXCOLOR	col;		// �F
		D3DXMATRIX	mtxWorld;	// ���[���h�}�g���b�N�X
		float fRadius;	// ���a
		D3DCULL cull;	// �J�����O��
		bool bLight;	// ���C�e�B���O��
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;	// �I��
	void Update(void) override;	// �X�V
	void Draw(void) override;	// �`��

	void BindTexture(const int nTextureID) override;		// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char *pTexturePass) override;	// �e�N�X�`������ (�p�X)
	void SetVec3Position(const D3DXVECTOR3& rPos) override;	// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override;		// �ʒu�擾
	void SetVec3Rotation(const D3DXVECTOR3& rRot) override;	// �����ݒ�
	D3DXVECTOR3 GetVec3Rotation(void) const override;		// �����擾

	void SetColor(const D3DXCOLOR& rCol) override;	// �F�ݒ�
	D3DXCOLOR GetColor(void) const override;		// �F�擾
	void SetRadius(const float fRadius) override;	// ���a�ݒ�
	float GetRadius(void) const override;			// ���a�擾

	// �ÓI�����o�֐�
	static CObjectMeshDome *Create	// ����
	( // ����
		const D3DXVECTOR3& rPos,	// �ʒu
		const D3DXVECTOR3& rRot,	// ����
		const D3DXCOLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const float fRadius,		// ���a
		const D3DCULL cull = D3DCULL_CCW,	// �J�����O��
		const bool bLight = true			// ���C�e�B���O��
	);

	// �����o�֐�
	void SetCulling(const D3DCULL cull);	// �J�����O�ݒ�
	D3DCULL GetCulling(void) const;			// �J�����O�擾
	void SetLighting(const bool bLight);	// ���C�e�B���O�ݒ�
	bool GetLighting(void) const;			// ���C�e�B���O�擾

	HRESULT SetPattern(const POSGRID2& rPart);		// �������ݒ�
	POSGRID2 GetPattern(void) const;				// �������擾
	void SetTexPattern(const POSGRID2& rTexPart);	// �e�N�X�`���������ݒ�
	POSGRID2 GetTexPattern(void) const;				// �e�N�X�`���������擾

protected:
	// �����o�֐�
	void SetVtx(void);	// ���_���̐ݒ�
	void SetIdx(void);	// �C���f�b�N�X���̐ݒ�

	void SetScrollTex	// �X�N���[���̃e�N�X�`�����W�̐ݒ�
	( // ����
		const float fTexU,	// �e�N�X�`���̉����W�̊J�n�ʒu
		const float fTexV	// �e�N�X�`���̏c���W�̊J�n�ʒu
	);

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	SMeshDome m_meshDome;	// ���b�V���h�[���̏��
	POSGRID2 m_part;		// ������
	POSGRID2 m_texPart;		// �e�N�X�`��������
	int m_nNumVtx;			// �K�v���_��
	int m_nNumIdx;			// �K�v�C���f�b�N�X��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _OBJECT_MESHDOME_H_
