//============================================================
//
//	�v���R���p�C���w�b�_�[ [precompile.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
// �r���h���̌x���Ώ��p�}�N��
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>
#include <time.h>
#include <assert.h>

#include <d3dx9.h>

// �r���h���̌x���Ώ��p�}�N��
#define  DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>		// ���͏����ɕK�v

#include <Xinput.h>		// �p�b�h�g�p�ɕK�v
#include <xaudio2.h>	// �T�E���h�����ɕK�v

// ���������[�N�o�͗p�}�N��
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>		// ���������[�N���o�ɕK�v

// ���������[�N�̃\�[�X�\�� ON/OFF
#if 1
#ifdef _DEBUG
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG
#endif

#endif	// _PRECOMPILE_H_
