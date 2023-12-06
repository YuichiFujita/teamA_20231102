//<============================================================
//
//���f���ł̃G�t�F�N�g���� [ModelEffect.cpp]
//Author�Fkazuki watanebe
//
//<============================================================
#include "ModelEffect.h"

//<=============================================================
//���f���G�t�F�N�g�̃R���X�g���N�^
//<=============================================================
CModEffect::CModEffect() : CObjectModel(CObject::LABEL_EFFECT,1)
{
	//�l�̃N���A
	m_rMove = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_rSize = VEC3_ZERO;
	m_rPos = VEC3_ZERO;
	m_nLife = 0;

	m_bGravity = false;
}
//<=============================================================
//���f���G�t�F�N�g�̐�������
//<=============================================================
CModEffect *CModEffect::Create(const D3DXVECTOR3 rPos, const D3DXVECTOR3 rRot, 
	const D3DXVECTOR3 rMove, const int nLife, const bool bGravity)
{
	//�������邽�߂̃I�u�W�F�N�g
	CModEffect *pModelEffect = new CModEffect;

	//���g�`�F�b�N
	if (pModelEffect != nullptr)
	{
		//����������
		assert(pModelEffect->Init() == S_OK);

		//<*********************************
		//���f���̊��蓖��
		//<*********************************
		pModelEffect->SetVec3Position(rPos);	//�ʒu
		pModelEffect->SetVec3Rotation(rRot);	//����
		pModelEffect->m_rMove = rMove;			//�ړ��l
		pModelEffect->m_nLife = nLife;			//����
		pModelEffect->m_bGravity = bGravity;	//�d�͂������邩�ǂ���

		//���f���̊��蓖��
		pModelEffect->BindModel("data\\MODEL\\OBSTACLE\\Jar000.x");

		//�|�C���^��Ԃ�
		return pModelEffect;
	}
	//�Ȃ����
	else
	{
		//nullptr��Ԃ�
		return nullptr;
	}

	//nullptr��Ԃ�
	return nullptr;
}
//<=============================================================
//���f���G�t�F�N�g�̍X�V����
//<=============================================================
void CModEffect::Update(void)
{
	//���̎擾
	m_rPos = GetVec3Position();
	m_rot = GetVec3Rotation();

	//�������܂������
	if (m_nLife > 0)
	{
		//���������炷
		m_nLife--;
	}
	//�Ȃ����
	else
	{
		//�I������������
		Uninit();

		return;
	}
	//�����d�͂��邩�ǂ�����true�ɂȂ��Ă�����
	if (m_bGravity)
	{
		//�n�ʂ̈ʒu������肻���ɂȂ�����
		if (m_rPos.y <= 0.0f)
		{
			m_rPos.y = 0.0f;

			//�I��������
			Uninit();

			return;
		}
		//����ȊO�̏ꍇ
		else
		{
			//�d�͂𓭂�����
			m_rMove.y -= 1.5f;
		}

	}

	//�ړ��l�����Z
	m_rPos += m_rMove;

	//���ꂼ��̐ݒ�
	CObjectModel::SetVec3Position(m_rPos);
	CObjectModel::SetVec3Rotation(m_rot);

	//�X�V����
	CObjectModel::Update();
}