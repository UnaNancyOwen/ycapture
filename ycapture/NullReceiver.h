#pragma once

#include "IReceiver.h"

/**
* 何もしないレシーバです。
*/
class NullReceiver : public IReceiver
{
public:

	/**
	* 構築します。
	*/
	NullReceiver(int red, int green, int blue);

	/**
	* 破棄します。
	*/
	virtual ~NullReceiver(void);

	/**
	* 破棄します。
	*/
	virtual void Destroy();

	/**
	* データを受信します。
	*/
	virtual HRESULT Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer);

	/**
	* データの処理完了を通知します。
	*/
	virtual HRESULT NotifyProcessed(unsigned char* pBuffer);

private:

	/**
	* 転送時刻を保持します。
	*/
	LONGLONG m_llTime;

	/**
	* カウントを保持します。
	*/
	int m_iCount;

	/**
	* 色を保持します。
	*/
	int m_iRed;

	/**
	* 色を保持します。
	*/
	int m_iGreen;

	/**
	* 色を保持します。
	*/
	int m_iBlue;

};
