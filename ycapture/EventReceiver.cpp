#include "StdAfx.h"
#include "EventReceiver.h"
#include "YCMutex.h"
#include "YCCritSection.h"

// 構築します。
EventReceiver::EventReceiver(const wchar_t* memName, const wchar_t* writeEventName, const wchar_t* readEventName, HRESULT* phr)
{
	m_pMutex = NULL;
	m_iLocalBufSize = 0;
	m_pLocalBuf = NULL;
	m_hSharedFile = NULL;
	m_hWriteEvent = NULL;
	m_pSharedBuf = NULL;

	HRESULT hr = S_OK;
	if(memName == NULL || writeEventName == NULL || readEventName == NULL) {
		*phr = E_POINTER;
		return;
	}

	m_pMutex = new YCMutex(readEventName, TRUE, & hr);
	if(FAILED(hr)) {
		*phr = hr;
		return;
	}

	YCCritSection cs(m_pMutex);
	m_hSharedFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, NULL, sizeof(SHARED_BUF) + MAX_BUF_SIZE, memName);
	if (m_hSharedFile == NULL) {
		*phr = E_UNEXPECTED;
		return;
	}

	m_pSharedBuf = (SHARED_BUF*) MapViewOfFile(m_hSharedFile, FILE_MAP_WRITE, 0, 0, 0);
	if(m_pSharedBuf == NULL) {
		*phr = E_UNEXPECTED;
		return;
	}
	ZeroMemory(m_pSharedBuf, sizeof(SHARED_BUF) + MAX_BUF_SIZE);
	m_pSharedBuf->maxSize = MAX_BUF_SIZE;
	m_pSharedBuf->actualSize = sizeof(BUFFER_DESC) + 3;
	BUFFER_DESC* pBufDesc = (BUFFER_DESC*) m_pSharedBuf->data;
	pBufDesc->width = 1;
	pBufDesc->height = 1;
	pBufDesc->time = 0;

	m_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, writeEventName);
	if (m_hWriteEvent == NULL) {
		*phr = E_UNEXPECTED;
		return;
	}
}

// 破棄します。
EventReceiver::~EventReceiver(void)
{
	if(m_pMutex != NULL) {
		delete m_pMutex;
		m_pMutex = NULL;
	}
	if(m_pLocalBuf != NULL) {
		delete[] m_pLocalBuf;
		m_pLocalBuf = NULL;
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

// 破棄します。
void EventReceiver::Destroy()
{
	delete this;
}

// データを受信します。
HRESULT EventReceiver::Receive(BUFFER_DESC* pDesc, unsigned char** ppBuffer)
{
	if(pDesc == NULL || ppBuffer == NULL) {
		return E_POINTER;
	}
	DWORD dwEvent = WaitForSingleObject(m_hWriteEvent, 0);
	if(dwEvent == WAIT_OBJECT_0 || m_pLocalBuf == NULL) {
		YCCritSection cs(m_pMutex);
		if(m_iLocalBufSize != m_pSharedBuf->actualSize) {
			if(m_pLocalBuf != NULL) {
				delete[] m_pLocalBuf;
				m_pLocalBuf = NULL;
			}
			m_pLocalBuf = new unsigned char[m_pSharedBuf->actualSize];
			m_iLocalBufSize = m_pSharedBuf->actualSize;
		}
		memcpy(m_pLocalBuf, m_pSharedBuf->data, m_iLocalBufSize);
	}

	memcpy(pDesc, m_pLocalBuf, sizeof(BUFFER_DESC));
	*ppBuffer = m_pLocalBuf + sizeof(BUFFER_DESC);
	return S_OK;
}

// データの処理完了を通知します。
HRESULT EventReceiver::NotifyProcessed(unsigned char* pBuffer)
{
	if(pBuffer == NULL) {
		return E_POINTER;
	}
	return S_OK;
}
