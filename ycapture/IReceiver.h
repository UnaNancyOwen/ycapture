#pragma once

#include "yctypes.h"

/**
* ���V�[�o�[�ł��B
*/
class IReceiver
{
public:

	/**
	* �j�����܂��B
	*/
	virtual void Destroy() = 0;

	/**
	* �f�[�^����M���܂��B
	*/
	virtual HRESULT Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer) = 0;

	/**
	* �f�[�^�̏���������ʒm���܂��B
	*/
	virtual HRESULT NotifyProcessed(unsigned char* pBuffer) = 0;

};
