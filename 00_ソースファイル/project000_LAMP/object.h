//============================================================
//
//	�I�u�W�F�N�g�w�b�_�[ [object.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"

//************************************************************
//	�}�N����`
//************************************************************
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// ���_�t�H�[�}�b�g [2D]
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_�t�H�[�}�b�g [3D]
#define FVF_VERTEX_MULTEX3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)	// ���_�t�H�[�}�b�g [MULTEX3D]

#define MAX_PRIO		(16)	// �D�揇�ʂ̑���
#define DEFAULT_PRIO	(8)		// �f�t�H���g�̗D�揇��

//************************************************************
//	�\���̒�`
//************************************************************
// ���_��� [2D]
struct VERTEX_2D
{
	D3DXVECTOR3	pos;	// ���_���W
	float		rhw;	// ���W�ϊ��p�W�� (1.0f�ŌŒ�)
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

// ���_��� [3D]
struct VERTEX_3D
{
	D3DXVECTOR3	pos;	// ���_���W
	D3DXVECTOR3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
};

// ���_��� [MULTEX3D]
struct VERTEX_MULTEX3D
{
	D3DXVECTOR3	pos;	// ���_���W
	D3DXVECTOR3	nor;	// �@���x�N�g��
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2	tex0;	// �e�N�X�`�����W_0����
	D3DXVECTOR2	tex1;	// �e�N�X�`�����W_1����
};

//************************************************************
//	�N���X��`
//************************************************************
// �I�u�W�F�N�g�N���X
class CObject
{
public:
	// �I�u�W�F�N�g���x����
	enum ELabel
	{
		LABEL_NONE = 0,		// �Ȃ�
		LABEL_UI,			// UI
		LABEL_NUMBER,		// ����
		LABEL_PLAYER,		// �v���C���[
		LABEL_SPAWNPOINT,	// �����ʒu
		LABEL_ITEMPOINT,	// �A�C�e�������ʒu
		LABEL_GROUND,		// �n��
		LABEL_BLOCK,		// �u���b�N
		LABEL_OBSTACLE,		// ��Q��
		LABEL_FIELD,		// �n��
		LABEL_WALL,			// ��
		LABEL_SCENERY,		// �i�F
		LABEL_SKY,			// ��
		LABEL_LIQUID,		// �t��
		LABEL_EFFECT,		// �G�t�F�N�g
		LABEL_PARTICLE,		// �p�[�e�B�N��
		LABEL_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CObject();
	explicit CObject(const ELabel label, const int nPriority = DEFAULT_PRIO);

	// �f�X�g���N�^
	virtual ~CObject();

	// �������z�֐�
	virtual HRESULT Init(void)	= 0;	// ������
	virtual void Uninit(void)	= 0;	// �I��
	virtual void Update(void)	= 0;	// �X�V
	virtual void Draw(void)		= 0;	// �`��
	virtual void Release(void)	= 0;	// �j��

	// ���z�֐�
	virtual void Hit(void);	// �q�b�g
	virtual void Hit(const int nDmg);	// �_���[�W�q�b�g
	virtual void HitKnockBack(const int nDmg, const D3DXVECTOR3& vecKnock);	// �m�b�N�o�b�N�q�b�g

	virtual void BindTexture(const int nTextureID);		// �e�N�X�`������ (�C���f�b�N�X)
	virtual void BindTexture(const char *pTexturePass);	// �e�N�X�`������ (�p�X)
	virtual void BindModel(const int nModelID);			// ���f������ (�C���f�b�N�X)
	virtual void BindModel(const char *pModelPass);		// ���f������ (�p�X)

	virtual void SetIndex(const int nIndex);		// �C���f�b�N�X�ݒ�
	virtual int GetIndex(void) const;				// �C���f�b�N�X�擾
	virtual void SetState(const int nState);		// ��Ԑݒ�
	virtual int GetState(void) const;				// ��Ԏ擾
	virtual void SetType(const int nType);			// ��ސݒ�
	virtual int GetType(void) const;				// ��ގ擾
	virtual void SetDodge(const int nDodge);		// ���@�ݒ�
	virtual int GetDodge(void) const;				// ���@�擾
	virtual void SetAngle(const float fAngle);		// �p�x�ݒ�
	virtual float GetAngle(void) const;				// �p�x�擾
	virtual void SetLength(const float fLength);	// �����ݒ�
	virtual float GetLength(void) const;			// �����擾
	virtual void SetRadius(const float fRadius);	// ���a�ݒ�
	virtual float GetRadius(void) const;			// ���a�擾
	virtual void SetHeight(const float fHeight);	// �c���ݒ�
	virtual float GetHeight(void) const;			// �c���擾
	virtual void SetScale(const float fScale);		// �g�嗦�ݒ�
	virtual float GetScale(void) const;				// �g�嗦�擾

	virtual void SetVec2Position(const D3DXVECTOR2& rPos);	// �񎲂̈ʒu�ݒ�
	virtual D3DXVECTOR2 GetVec2Position(void) const;		// �񎲂̈ʒu�擾
	virtual void SetVec3Position(const D3DXVECTOR3& rPos);	// �O���̈ʒu�ݒ�
	virtual D3DXVECTOR3 GetVec3Position(void) const;		// �O���̈ʒu�擾
	virtual void SetVec3Rotation(const D3DXVECTOR3& rRot);	// �����ݒ�
	virtual D3DXVECTOR3 GetVec3Rotation(void) const;		// �����擾
	virtual void SetVec2Sizing(const D3DXVECTOR2& rSize);	// �񎲂̑傫���ݒ�
	virtual D3DXVECTOR2 GetVec2Sizing(void) const;			// �񎲂̑傫���擾
	virtual void SetVec3Sizing(const D3DXVECTOR3& rSize);	// �O���̑傫���ݒ�
	virtual D3DXVECTOR3 GetVec3Sizing(void) const;			// �O���̑傫���擾
	virtual void SetVec3Scaling(const D3DXVECTOR3& rSacle);	// �g�嗦�ݒ�
	virtual D3DXVECTOR3 GetVec3Scaling(void) const;			// �g�嗦�擾
	virtual void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	virtual D3DXCOLOR GetColor(void) const;					// �F�擾
	virtual void SetAllMaterial(const D3DXMATERIAL& rMat);	// �}�e���A���S�ݒ�
	virtual void ResetMaterial(void);						// �}�e���A���Đݒ�

	virtual void SetTexturePatternX(const D3DXVECTOR2& rTexPart);	// �e�N�X�`��������X�ݒ�
	virtual D3DXVECTOR2 GetTexturePatternX(void) const;				// �e�N�X�`��������X�擾
	virtual void SetTexturePatternY(const D3DXVECTOR2& rTexPart);	// �e�N�X�`��������Y�ݒ�
	virtual D3DXVECTOR2 GetTexturePatternY(void) const;				// �e�N�X�`��������Y�擾
	virtual void SetTexturePatternZ(const D3DXVECTOR2& rTexPart);	// �e�N�X�`��������Z�ݒ�
	virtual D3DXVECTOR2 GetTexturePatternZ(void) const;				// �e�N�X�`��������Z�擾

	virtual void SetPriority(const int nPriority);			// �D�揇�ʐݒ�
	virtual void SetEnableUpdate(const bool bUpdate);		// �X�V�󋵐ݒ�
	virtual void SetEnableDraw(const bool bDraw);			// �`��󋵐ݒ�
	virtual void SetEnableDepthShadow(const bool bShadow);	// �e�\���󋵐ݒ�
	virtual void SetEnableZTex(const bool bZTex);			// Z�e�N�X�`�������ݒ�
	virtual D3DXMATRIX *GetPtrMtxWorld(void);				// �}�g���b�N�X�|�C���^�擾
	virtual D3DXMATRIX GetMtxWorld(void) const;				// �}�g���b�N�X�擾

	// �ÓI�����o�֐�
	static void ReleaseAll(void);	// �S�j��
	static void UpdateAll(void);	// �S�X�V
	static void DrawAll(void);		// �S�`��
	static int  GetNumAll(void);	// �����擾
	static CObject *GetTop(const int nPriority);	// �擪�I�u�W�F�N�g�擾
	static CObject *GetCur(const int nPriority);	// �Ō���I�u�W�F�N�g�擾

	// �����o�֐�
	void	SetLabel(const ELabel label);	// ���x���ݒ�
	ELabel	GetLabel(void) const;			// ���x���擾
	int		GetPriority(void) const;		// �D�揇�ʎ擾

	DWORD	GetUniqueID(void) const;	// ���j�[�NID�擾
	bool	IsUpdate(void) const;		// �X�V�󋵎擾
	bool	IsDraw(void) const;			// �`��󋵎擾
	bool	IsDeath(void) const;		// ���S�t���O�擾
	bool	IsShadow(void) const;		// �e�\���󋵎擾
	bool	IsZTex(void) const;			// Z�`��󋵎擾
	CObject	*GetObject(void);			// �I�u�W�F�N�g�擾
	CObject	*GetPrev(void) const;		// �O�I�u�W�F�N�g�擾
	CObject	*GetNext(void) const;		// ���I�u�W�F�N�g�擾

private:
	// �ÓI�����o�֐�
	static void DeathAll(void);	// �S���S

	// �ÓI�����o�ϐ�
	static CObject	*m_apTop[MAX_PRIO];	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject	*m_apCur[MAX_PRIO];	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^
	static DWORD	m_dwNextID;			// ���̃��j�[�NID
	static int		m_nNumAll;			// �I�u�W�F�N�g�̑���

	// �����o�ϐ�
	ELabel	m_label;		// ���g�̃I�u�W�F�N�g���x��
	DWORD	m_dwID;			// ���g�̃��j�[�NID
	int		m_nPriority;	// ���g�̗D�揇��
	bool	m_bUpdate;		// ���g�̍X�V��
	bool	m_bDraw;		// ���g�̕`���
	bool	m_bDeath;		// ���g�̎��S�t���O
	bool	m_bZTex;		// ���g��Z�e�N�X�`���ɏ������܂�邩
	bool	m_bDepthShadow;	// ���g�̉e�\����
	CObject	*m_pPrev;		// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject	*m_pNext;		// ���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif	// _OBJECT_H_
