#include "StdAfx.h"
#include "CaptureSender.h"
#include <YCMutex.h>
#include <YCCritSection.h>

// 構築します。
CaptureSender::CaptureSender(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName)
{
	m_pMutex = NULL;
	m_hSharedFile = NULL;
	m_hWriteEvent = NULL;
	m_pSharedBuf = NULL;
	m_memName = memName;
	m_writeEventName = writeEventName;
	m_readEventName = readEventName;
}

// 破棄します。
CaptureSender::~CaptureSender(void)
{
	if(m_pMutex != NULL) {
		delete m_pMutex;
		m_pMutex = NULL;
	}
	if(m_hWriteEvent != NULL) {
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = NULL;
	}
	if(m_hSharedFile != NULL) {
		CloseHandle(m_hSharedFile);
		m_hSharedFile = NULL;
	}
	m_pSharedBuf = NULL;
}

// 送信します。
HRESULT CaptureSender::Send(LONGLONG time, int width, int height, const unsigned char* buffer)
{
	if(buffer == NULL) {
		return E_POINTER;
	}
	HRESULT hr = Open();
	if(hr != S_OK) {
		return hr;
	}
	{
		YCCritSection cs(m_pMutex);
		size_t imageSize = width * height * 3;
		size_t bufSize = imageSize + sizeof(BUFFER_DESC);
		if(m_pSharedBuf->maxSize < bufSize) {
			return E_UNEXPECTED;
		}
		m_pSharedBuf->actualSize = bufSize;
		BUFFER_DESC* pDesc = (BUFFER_DESC*) m_pSharedBuf->data;
		pDesc->time = time;
		pDesc->width = width;
		pDesc->height = height;
		unsigned char* pSharedData = m_pSharedBuf->data + sizeof(BUFFER_DESC);
		memcpy(pSharedData, buffer, imageSize);
	}
	SetEvent(m_hWriteEvent);
	return S_OK;
}

// 開きます。
HRESULT CaptureSender::Open()
{
	if(m_hWriteEvent == NULL) {
		m_hWriteEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, m_writeEventName.c_str());
		if(m_hWriteEvent == NULL) {
			return S_FALSE;
		}
	}
	if(m_hSharedFile == NULL) {
		m_hSharedFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, m_memName.c_str());
		if (m_hSharedFile == NULL) {
			return S_FALSE;
		}
		m_pSharedBuf = (SHARED_BUF*) MapViewOfFile(m_hSharedFile, FILE_MAP_WRITE, 0, 0, 0);
		if(m_pSharedBuf == NULL) {
			return E_UNEXPECTED;
		}
	}
	if(m_pMutex == NULL) {
		HRESULT hr = S_OK;
		m_pMutex = new YCMutex(m_readEventName.c_str(), false, & hr);
		if(FAILED(hr)) {
			delete m_pMutex;
			m_pMutex = NULL;
			return S_FALSE;
		}
	}
	return S_OK;
}
