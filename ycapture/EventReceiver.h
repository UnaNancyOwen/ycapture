#pragma once

class YCMutex;

#define MAX_BUF_SIZE	2000 * 2000 * 3
#include "IReceiver.h"

/**
* �C�x���g��M�p�I�u�W�F�N�g�ł��B
*/
class EventReceiver : public IReceiver
{
public:

	/**
	* �\�z���܂��B
	*/
	EventReceiver(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName, HRESULT* phr);

	/**
	* �j�����܂��B
	*/
	virtual ~EventReceiver(void);

	/**
	* �j�����܂��B
	*/
	virtual void Destroy();

	/**
	* �f�[�^����M���܂��B
	*/
	virtual HRESULT Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer);

	/**
	* �f�[�^�̏���������ʒm���܂��B
	*/
	virtual HRESULT NotifyProcessed(unsigned char* pBuffer);

private:

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

	/**
	* ���[�J���o�b�t�@�̃T�C�Y��ێ����܂��B
	*/
	size_t m_iLocalBufSize;

	/**
	* ���[�J���o�b�t�@��ێ����܂��B
	*/
	unsigned char* m_pLocalBuf;

};
