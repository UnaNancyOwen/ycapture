#pragma once

// ycapture.h : �t�B���^�̏���`

// �����͎����̐��i�ɍ��킹�ĕύX���邱��

// �t�B���^����`
#define NAME_CAPTURESOURCE L"ARgil Capture Source"

// {5C2CD55C-92AD-4999-8666-912BD3E70001}
DEFINE_GUID(CLSID_CaptureSource, 
	0x5c2cd55c, 0x92ad, 0x4999, 0x86, 0x66, 0x91, 0x2b, 0xd3, 0xe7, 0x0, 0x1);

// �C�x���g�E�������}�b�v�h�t�@�C������`
// �t�B���^���E���M���œ���̖��O���g�p���邱��

// �������݂������C�x���g��
#define CS_EVENT_WRITE	L"ARgil_CaptureSource_Write"
// �f�[�^�ǂݍ��݂������~���[�e�b�N�X��
#define CS_EVENT_READ	L"ARgil_CaptureSource_Read"
// ���L�f�[�^�t�@�C����(�������}�b�v�h�t�@�C��)
#define CS_SHARED_PATH	L"ARgil_CaptureSource_Data"
