#pragma once

#include <yctypes.h>
class YCMutex;

/**
* �L���v�`���f�[�^���M�p�N���X�ł��B
*/
class CaptureSender
{
public:

	/**
	* �\�z���܂��B
	*/
	CaptureSender(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName);

	/**
	* �j�����܂��B
	*/
	virtual ~CaptureSender(void);

	/**
	* ���M���܂��B
	* time�̓~���b�ŃT���v���̎������w�肵�Ă��������B(�Q�l�l�Ƃ��Ďg�p���܂��B�����ɓ����͂���܂���B)
	* �o�b�t�@�͕K��24bit RGB�ŕ\�����Ă��������B�p�f�B���O�Ȃǂ͖��Ή��ł��B
	*/
	virtual HRESULT Send(LONGLONG time, int width, int height, const unsigned char* buffer);

private:

	/**
	* �J���܂��B
	*/
	HRESULT Open();

	/**
	* ����������ێ����܂��B
	*/
	std::wstring m_memName;

	/**
	* �����o���C�x���g����ێ����܂��B
	*/
	std::wstring m_writeEventName;

	/**
	* �ǂݍ��݃C�x���g����ێ����܂��B
	*/
	std::wstring m_readEventName;

	/**
	* �����o���C�x���g��ێ����܂��B
	*/
	HANDLE m_hWriteEvent;

	/**
	* �����I�u�W�F�N�g��ێ����܂��B
	*/
	YCMutex* m_pMutex;

	/**
	* ���L�t�@�C���̃n���h����ێ����܂��B
	*/
	HANDLE m_hSharedFile;

	/**
	* ���L�o�b�t�@��ێ����܂��B
	*/
	SHARED_BUF* m_pSharedBuf;

};
