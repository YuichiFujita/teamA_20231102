//<============================================================
//
//���f���ł̃G�t�F�N�g���� [ModelEffect.h]
//Author�Fkazuki watanebe
//
//<============================================================
//<************************************************************
//��d�C���N���[�h�h�~
//<************************************************************
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

//<************************************************************
//�C���N���[�h�t�@�C��
//<************************************************************
#include "main.h"
#include "ObjectModel.h"

//<************************************************************
//�N���X�錾
//<************************************************************
class CModEffect : public CObjectModel
{
public:

	CModEffect();
	~CModEffect() {};

	//�I�[�o�[���C�h�֐�
	//������
	HRESULT Init(void) override 
	{
		CObjectModel::Init();

		return S_OK;

	}	

	//�I��
	void Uninit(void) override { CObjectModel::Uninit(); }		

	//�X�V
	void Update(void) override;	

	//�`��
	void Draw(void) override { CObjectModel::Draw(); }		

	//��������
	static CModEffect *Create(const D3DXVECTOR3 rPos, const D3DXVECTOR3 rRot, const D3DXVECTOR3 rMove,const int nLife,const bool bGravity);

private:

	D3DXVECTOR3 m_rPos;		//�ʒu
	D3DXVECTOR3 m_rMove;	//�ړ���
	D3DXVECTOR3 m_rot;		//����
	D3DXVECTOR3 m_rSize;	//�T�C�Y

	bool m_bGravity;		//�d�͂������邩�ǂ���
	int m_nLife;			//����
};

#endif
