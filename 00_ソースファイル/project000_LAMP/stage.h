//============================================================
//
//	�X�e�[�W�w�b�_�[ [stage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CField;	// �n�ʃN���X
class CWall;	// �ǃN���X
class CScenery;	// �i�F�N���X
class CSky;		// ��N���X
class CLiquid;	// �t�̃N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// ������
	enum ELimit
	{
		LIMIT_BOX = 0,	// ��`�͈�
		LIMIT_CIRCLE,	// �~�͈�
		LIMIT_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// �X�e�[�W�͈͍\����
	struct SStageLimit
	{
		ELimit mode;		// �������[�h
		D3DXVECTOR3 center;	// ���S���W
		float fNear;		// �����ʒu (�O)
		float fFar;			// �����ʒu (��)
		float fRight;		// �����ʒu (�E)
		float fLeft;		// �����ʒu (��)
		float fRadius;		// �����ʒu (���a)
		float fField;		// �����ʒu (�n��)
	};

	// �n�ʍ\����
	struct SField
	{
		CField **ppField;	// �n�ʂ̏��
		int nNum;			// �n�ʂ̑���
	};

	// �Ǎ\����
	struct SWall
	{
		CWall **ppWall;	// �ǂ̏��
		int nNum;		// �ǂ̑���
	};

	// ���i�\����
	struct SScenery
	{
		CScenery **ppScenery;	// ���i�̏��
		int nNum;				// ���i�̑���
	};

	// ��\����
	struct SSky
	{
		CSky **ppSky;	// ��̏��
		int nNum;		// ��̑���
	};

	// �t�̍\����
	struct SLiquid
	{
		CLiquid **ppLiquid;	// �t�̂̏��
		int nNum;			// �t�̂̑���
	};

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V

	void SetStageLimit(const SStageLimit& rLimit);	// �X�e�[�W�͈͐ݒ�
	SStageLimit GetStageLimit(void) const;			// �X�e�[�W�͈͎擾

	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool CollisionKillY(const D3DXVECTOR3& rPos);				// �L��Y���W�Ƃ̓����蔻��

	bool LandFieldPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove);	// �n�ʒ��n
	bool IsFieldPositionRange(const D3DXVECTOR3&rPos);				// ���b�V���͈͓̔��擾 (��]�l��)
	float GetFieldPositionHeight(const D3DXVECTOR3&rPos);			// ���b�V���̒��n�ʒu�擾 (��]�l��)

	CLiquid *GetLiquid(void);	// �t�̎擾

	// �ÓI�����o�֐�
	static CStage *Create(const CScene::EMode mode);	// ����
	static HRESULT Release(CStage *&prStage);			// �j��

private:
	// �ÓI�����o�֐�
	static HRESULT LoadSetup(const CScene::EMode mode, CStage *pStage);					// �Z�b�g�A�b�v
	static HRESULT LoadLimit(const char* pString, FILE *pFile, CStage *pStage);			// �͈͏��̓Ǎ�
	static HRESULT LoadField(const char* pString, FILE *pFile, CStage *pStage);			// �n�ʏ��̓Ǎ�
	static HRESULT LoadWall(const char* pString, FILE *pFile, CStage *pStage);			// �Ǐ��̓Ǎ�
	static HRESULT LoadScenery(const char* pString, FILE *pFile, CStage *pStage);		// �i�F���̓Ǎ�
	static HRESULT LoadSky(const char* pString, FILE *pFile, CStage *pStage);			// ����̓Ǎ�
	static HRESULT LoadLiquid(const char* pString, FILE *pFile, CStage *pStage);		// �t�̏��̓Ǎ�
	static HRESULT LoadGround(const char* pString, FILE *pFile, CStage *pStage);		// �n�Տ��̓Ǎ�
	static HRESULT LoadBlock(const char* pString, FILE *pFile, CStage *pStage);			// �u���b�N���̓Ǎ�
	static HRESULT LoadObstacle(const char* pString, FILE *pFile, CStage *pStage);		// ��Q�����̓Ǎ�
	static HRESULT LoadSpawnPoint(const char* pString, FILE *pFile, CStage *pStage);	// �����ʒu���̓Ǎ�
	static HRESULT LoadItemPoint(const char* pString, FILE *pFile, CStage *pStage);		// �A�C�e�������ʒu���̓Ǎ�
	static HRESULT LoadNextPoint(const char* pString, FILE *pFile, CStage *pStage);		// �J�ڈʒu���̓Ǎ�

	// �����o�ϐ�
	SStageLimit	m_stageLimit;	// �͈͏��
	SField		m_field;		// �n�ʏ��
	SWall		m_wall;			// �Ǐ��
	SScenery	m_scenery;		// �i�F���
	SSky		m_sky;			// ����
	SLiquid		m_liquid;		// �t�̏��
};

#endif	// _STAGE_H_
