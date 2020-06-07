#pragma once

#include "IReceiver.h"

/**
* �������Ȃ����V�[�o�ł��B
*/
class NullReceiver : public IReceiver
{
public:

	/**
	* �\�z���܂��B
	*/
	NullReceiver(int red, int green, int blue);

	/**
	* �j�����܂��B
	*/
	virtual ~NullReceiver(void);

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
	* �]��������ێ����܂��B
	*/
	LONGLONG m_llTime;

	/**
	* �J�E���g��ێ����܂��B
	*/
	int m_iCount;

	/**
	* �F��ێ����܂��B
	*/
	int m_iRed;

	/**
	* �F��ێ����܂��B
	*/
	int m_iGreen;

	/**
	* �F��ێ����܂��B
	*/
	int m_iBlue;

};
