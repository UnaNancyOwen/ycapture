#pragma once

#include "yctypes.h"

/**
* レシーバーです。
*/
class IReceiver
{
public:

	/**
	* 破棄します。
	*/
	virtual void Destroy() = 0;

	/**
	* データを受信します。
	*/
	virtual HRESULT Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer) = 0;

	/**
	* データの処理完了を通知します。
	*/
	virtual HRESULT NotifyProcessed(unsigned char* pBuffer) = 0;

};
