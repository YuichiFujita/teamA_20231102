//========================================================================================
//
// �G�t�F�N�g
// Author: �O�� ���V��
//
//========================================================================================
//�C���N���[�h
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "object2D.h"
#include "orbitalParticle.h"
#include "orbitalEffect.h"
#include "input.h"


//=============================================
//�R���X�g���N�^
//=============================================
CorbitalParticle::CorbitalParticle():CObject(ELabel::LABEL_EFFECT)
{

	m_nCount = 0;
	m_nSpeed = 0;
	m_nEffectLife = 0;
	m_nNumSpawn = 0;
	m_fAngle = 0;
}
//=============================================
//�f�X�g���N�^
//=============================================
CorbitalParticle::~CorbitalParticle()
{
}

//=============================================
//�������֐�
//=============================================
HRESULT CorbitalParticle::Init()
{
	m_nCount = 0;
	return S_OK;
}
void CorbitalParticle::Uninit()
{

	Release();
}

//=============================================
//�X�V�֐�
//=============================================
void CorbitalParticle::Update()
{
	m_nCount += m_nNumSpawn;
	while (m_nCount >= 60)
	{
		m_nCount -= 60;
		float fRot, fRot2;//�p�x
		float fMove;//�ړ���
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 pos = m_pos;
		if (m_range.x >= 1.0f)
		{
			pos.x += (rand() % (int)(m_range.x * 2 * 10) - (m_range.x * 10)) * 0.1f;
		}
		if (m_range.y >= 1.0f)
		{
			pos.y += (rand() % (int)(m_range.y * 2 * 10) - (m_range.y * 10)) * 0.1f;
		}
		if (m_range.z >= 1.0f)
		{
			pos.z += (rand() % (int)(m_range.z * 2 * 10) - (m_range.z * 10)) * 0.1f;
		}
	
		fMove = (float)(rand() % m_nSpeed) / 10.0f + 2.0f;
		// �����_���Ȋp�x�𐶐�
		float theta = D3DXToRadian(static_cast<float>(rand() % 360));
		float phi = D3DXToRadian(static_cast<float>(rand() % 180));

		// �����Ɋ�Â��Ċp�x�𐧖�

		phi *= m_fAngle;


		move.x = sinf(phi) * cosf(theta) * fMove;
		move.y = cosf(phi) * fMove;
		move.z = sinf(phi) * sinf(theta) * fMove;
		COrbitalEffect * p = COrbitalEffect::Create(pos, m_Offset, m_col, m_nEffectLife, m_nLength, move, m_Forth, m_fAttenuation);
		p->SetPriority(0);
	}
	

	m_nLife--;

		
	
	if (m_nLife <= 0)
	{
		CObject::Release();
	}
}
void CorbitalParticle::Draw()
{

}
//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CorbitalParticle::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �����̈ʒu��ݒ�
	m_pos = rPos;
}
//=============================================
//�����֐�
//=============================================
CorbitalParticle * CorbitalParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Offset, D3DXCOLOR col, D3DXVECTOR3 range, D3DXVECTOR3 rot, D3DXVECTOR3 forth, int nLife,int nSpeed, int nEffectLife, int nLength, int nNumSpawn, float fAngle, float fAttenuation)
{

	CorbitalParticle * porbitalParticle = NULL;
	porbitalParticle = new  CorbitalParticle;
	porbitalParticle->m_pos = pos;
	porbitalParticle->m_Offset = Offset;
	porbitalParticle->m_col = col;
	porbitalParticle->m_range = range;
	porbitalParticle->m_rot = rot;
	porbitalParticle->m_Forth = forth;
	porbitalParticle->m_nLife = nLife;
	porbitalParticle->m_nSpeed = nSpeed;
	porbitalParticle->m_nEffectLife = nEffectLife;
	porbitalParticle->m_nLength = nLength;
	porbitalParticle->m_nNumSpawn = nNumSpawn;
	porbitalParticle->m_fAngle = fAngle;
	porbitalParticle->m_fAttenuation = fAttenuation;
	porbitalParticle->Init();

	return porbitalParticle;
}
void CorbitalParticle::Release()
{
	CObject::Release();
}