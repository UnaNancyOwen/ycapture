#pragma once

class YCMutex;

#define MAX_BUF_SIZE	2000 * 2000 * 3
#include "IReceiver.h"

/**
* イベント受信用オブジェクトです。
*/
class EventReceiver : public IReceiver
{
public:

	/**
	* 構築します。
	*/
	EventReceiver(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName, HRESULT* phr);

	/**
	* 破棄します。
	*/
	virtual ~EventReceiver(void);

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
	* 書き出しイベントを保持します。
	*/
	HANDLE m_hWriteEvent;

	/**
	* 同期オブジェクトを保持します。
	*/
	YCMutex* m_pMutex;

	/**
	* 共有ファイルのハンドルを保持します。
	*/
	HANDLE m_hSharedFile;

	/**
	* 共有バッファを保持します。
	*/
	SHARED_BUF* m_pSharedBuf;

	/**
	* ローカルバッファのサイズを保持します。
	*/
	size_t m_iLocalBufSize;

	/**
	* ローカルバッファを保持します。
	*/
	unsigned char* m_pLocalBuf;

};
