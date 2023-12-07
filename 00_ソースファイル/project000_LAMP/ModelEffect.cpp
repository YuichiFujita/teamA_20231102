//<============================================================
//
//���f���ł̃G�t�F�N�g���� [ModelEffect.cpp]
//Author�Fkazuki watanebe
//
//<============================================================
#include "ModelEffect.h"
#include "useful.h"

//<************************************************************
//�ÓI�����o�ϐ��錾
//<************************************************************
const char *CModEffect::mc_apTextureFile[] =	// �e�N�X�`���萔
{
	"data\\MODEL\\PIECE\\PIECE000.x",		//������P
	"data\\MODEL\\PIECE\\PIECE001.x",		//������Q
	"data\\MODEL\\PIECE\\PIECE002.x",		//������R
};

//<**************************************************************
//���O�錾
//<**************************************************************
namespace
{
	const int		MAX_PIECE	= 3;		//������̍ő��
	const float		VALUE_GRAV	= 1.5f;		//�d�͂�������l
	const float		VALUE_UP	= 50.0f;	//�㏸����l
	const int		MAX_LAND	= 4;		//���n���̍ő吔
	const float		VALUE_ALPHA = 0.1f;		//�����x��������l

}
//<=============================================================
//���f���G�t�F�N�g�̃R���X�g���N�^
//<=============================================================
CModEffect::CModEffect() : CObjectModel(CObject::LABEL_EFFECT,1)
{
	//�l�̃N���A
	m_rMove = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_rotMove = VEC3_ZERO;
	m_rSize = VEC3_ZERO;
	m_rPos = VEC3_ZERO;
	m_nLife = 0;
	m_nNumLand = 0;
	m_Col = XCOL_WHITE;

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

		//�����_���ŏo���������ݒ�
		pModelEffect->BindModel(mc_apTextureFile[rand() % MAX_PIECE]);

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
	//�����̈ړ��ʂ̐ݒ�
	m_rotMove = D3DXVECTOR3(0.05f, 0.05f, 0.05f);

	//���̎擾
	m_rPos = GetVec3Position();
	m_rot = GetVec3Rotation();

	//�����d�͂��邩�ǂ�����false�ɂȂ��Ă�����
	if (!m_bGravity)
	{
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
	}

	//�����d�͂��邩�ǂ�����true�ɂȂ��Ă�����
	if (m_bGravity)
	{
		//���l����
		useful::LimitNum(m_Col.a, 0.0f, 1.0f);

		//�n�ʂ̈ʒu������肻���ɂȂ�����
		if (m_rPos.y <= VEC3_ZERO.y)
		{
			//���n�����񐔂𑝂₷
			m_nNumLand++;

			//�n�ʈ��O�̒l�ɂ���
			m_rPos.y = 0.1f;

			//��Ɉړ�������
			m_rMove.y += VALUE_UP / m_nNumLand;

			//�����x�����炷
			m_Col.a = m_Col.a / m_nNumLand;

			//���n�����񐔂��ő�l�ɂȂ��Ă�����
			if (m_nNumLand >= MAX_LAND)
			{
				//�O�̂��߂̏�����
				m_nNumLand = 0;

				//�I��������
				Uninit();

				//�����𔲂���
				return;
			}
			//������return������ƒ��˂Ȃ��Ȃ��Ă��܂�
		}
		//����ȊO�̏ꍇ
		else
		{
			//�d�͂𓭂�����
			m_rMove.y -= VALUE_GRAV;
		}

	}

	//�ړ��l�����Z
	m_rPos += m_rMove;

	//�����̈ړ�������
	m_rot += m_rotMove;

	//�~�����𒴂��Ȃ��悤�ɂ���
	useful::NormalizeRot(m_rot.y);

	//���ꂼ��̐ݒ�
	CObjectModel::SetVec3Position(m_rPos);
	CObjectModel::SetVec3Rotation(m_rot);
	CObjectModel::SetAlpha(m_Col.a);

	//�X�V����
	CObjectModel::Update();
}