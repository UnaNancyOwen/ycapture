#include "StdAfx.h"
#include "NullReceiver.h"

// 構築します。
NullReceiver::NullReceiver(int red, int green, int blue)
{
	m_iRed = red;
	m_iGreen = green;
	m_iBlue = blue;
	m_iCount = 0;
	m_llTime = 0;
}

// 破棄します。
NullReceiver::~NullReceiver(void)
{
	;
}

// 破棄します。
void NullReceiver::Destroy()
{
	delete this;
}

// データを受信します。
HRESULT NullReceiver::Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer)
{
	if(pDesc == NULL || ppBuffer == NULL) {
		return E_POINTER;
	}
	int w = 320;
	int h = 200;
	int sizeImage = w * h * 3;
	unsigned char* pBuf = new unsigned char[sizeImage];
	for(int i = 0; i < w * h; i ++) {
		int offs = i * 3;
		pBuf[offs + 0] = min(m_iRed + m_iCount, 0xff);
		pBuf[offs + 1] = min(m_iGreen + m_iCount, 0xff);
		pBuf[offs + 2] = min(m_iBlue + m_iCount, 0xff);
	}

	m_iCount += 0x10;
	if(m_iCount > 0xff) {
		m_iCount = 0x00;
	}

	pDesc->width = w;
	pDesc->height = h;
	pDesc->time = m_llTime;
	m_llTime += 50;
	*ppBuffer = pBuf;
	return S_OK;
}

// データの処理完了を通知します。
HRESULT NullReceiver::NotifyProcessed(unsigned char* pBuffer)
{
	if(pBuffer == NULL) {
		return E_POINTER;
	}
	delete[] pBuffer;
	pBuffer = NULL;
	return S_OK;
}
