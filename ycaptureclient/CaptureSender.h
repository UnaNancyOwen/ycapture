#pragma once

#include <yctypes.h>
class YCMutex;

/**
* キャプチャデータ送信用クラスです。
*/
class CaptureSender
{
public:

	/**
	* 構築します。
	*/
	CaptureSender(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName);

	/**
	* 破棄します。
	*/
	virtual ~CaptureSender(void);

	/**
	* 送信します。
	* timeはミリ秒でサンプルの時刻を指定してください。(参考値として使用します。厳密に同期はされません。)
	* バッファは必ず24bit RGBで表現してください。パディングなどは未対応です。
	*/
	virtual HRESULT Send(LONGLONG time, int width, int height, const unsigned char* buffer);

private:

	/**
	* 開きます。
	*/
	HRESULT Open();

	/**
	* メモリ名を保持します。
	*/
	std::wstring m_memName;

	/**
	* 書き出しイベント名を保持します。
	*/
	std::wstring m_writeEventName;

	/**
	* 読み込みイベント名を保持します。
	*/
	std::wstring m_readEventName;

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

};
