#include "StdAfx.h"
#include "CaptureSource.h"
#include "ycapture.h"
#ifdef USE_NULL_RECEIVER
#include "NullReceiver.h"
#else	// USE_NULL_RECEIVER
#include "EventReceiver.h"
#endif	// USE_NULL_RECEIVER

// デフォルトサイズ定義です。
#define DEFAULT_WIDTH	640
#define DEFAULT_HEIGHT	480
#define MIN_WIDTH		160
#define MIN_HEIGHT		120
#define MAX_WIDTH		640
#define MAX_HEIGHT		480
#define MAX_FPS			15
#define MIN_FPS			1
#define MAX_FRAME_INTERVAL	10000000 / MIN_FPS
#define MIN_FRAME_INTERVAL	10000000 / MAX_FPS

#define DEFAULT_FPS		15

// メディアタイプ定義です。
const AMOVIESETUP_MEDIATYPE sudMediaTypesCaptureSourceOut = 
{ 
    &MEDIATYPE_Video, 
    &MEDIASUBTYPE_NULL 
};

// ピン定義です。
const AMOVIESETUP_PIN sudCaptureSourceOut =
{
    L"Output",             // Pin string name
    FALSE,                 // Is it rendered
    TRUE,                  // Is it an output
    FALSE,                 // Can we have none
    FALSE,                 // Can we have many
    &CLSID_NULL,           // Connects to filter
    NULL,                  // Connects to pin
    1,                     // Number of types
    &sudMediaTypesCaptureSourceOut      // Pin Media types
};

// フィルタ定義です。
const AMOVIESETUP_FILTER sudCaptureSource =
{
    &CLSID_CaptureSource,	// Filter CLSID
    NAME_CAPTURESOURCE,			// String name
    MERIT_DO_NOT_USE,		// Filter merit
    0,						// Number pins
    & sudCaptureSourceOut	// Pin details
};

// インスタンスを生成します。
CUnknown * WINAPI CaptureSource::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
    ASSERT(phr);
    
    CaptureSource *punk = new CaptureSource(lpunk, phr);
    if (punk == NULL) 
    {
        *phr = E_OUTOFMEMORY;
    }

#ifdef USE_NULL_RECEIVER
	HRESULT hr = punk->Init(new NullReceiver(0x00, 0x44, 0xff), DEFAULT_FPS);
	if(FAILED(hr)) {
		*phr = hr;
	}
#else	// USE_NULL_RECEIVER
	HRESULT hr = S_OK;
	EventReceiver* pRecv = new EventReceiver(CS_SHARED_PATH, CS_EVENT_WRITE, CS_EVENT_READ, & hr);
	if(FAILED(hr)) {
		*phr = hr;
		delete pRecv;
		pRecv = NULL;
		delete punk;
		punk = NULL;
		return NULL;
	}
	
	hr = punk->Init(pRecv, DEFAULT_FPS);
	if(FAILED(hr)) {
		*phr = hr;
		delete pRecv;
		pRecv = NULL;
		delete punk;
		punk = NULL;
		return NULL;
	}
#endif	// USE_NULL_RECEIVER
    return punk;
}

// 構築します。
CaptureSource::CaptureSource(LPUNKNOWN lpunk, HRESULT* phr)
: CSource(L"Source", lpunk, CLSID_CaptureSource, phr)
{
	;
}

CaptureSource::~CaptureSource(void)
{
	;
}

// インタフェースを取得します。
STDMETHODIMP CaptureSource::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	if(ppv == NULL) {
		return E_POINTER;
	}
	if(riid == IID_IQualityControl) {
		*ppv = (IQualityControl*) this;
		AddRef();
		return S_OK;
	}else if(riid == IID_IYCSource) {
		*ppv = (IYCSource*) this;
		AddRef();
		return S_OK;
	}
	return CSource::NonDelegatingQueryInterface(riid, ppv);
}

// 品質情報を通知します。
STDMETHODIMP CaptureSource::Notify(IBaseFilter *pSelf, Quality q)
{
	return S_OK;
}

// 品質情報管理オブジェクトを設定します。
STDMETHODIMP CaptureSource::SetSink(IQualityControl *piqc)
{
	return S_OK;
}

// 初期化します。
HRESULT CaptureSource::Init(IReceiver* pReceiver, int fps)
{
	HRESULT hr = S_OK;
	CaptureStream* pStream = new CaptureStream(this, pReceiver, fps, & hr);
	if(FAILED(hr)) {
		delete pStream;
		pStream = NULL;
		return hr;
	}
	return S_OK;
}

// 構築します。
CaptureSource::CaptureStream::CaptureStream(CaptureSource* pOwner, IReceiver* pReceiver, int fps, HRESULT* phr)
:CSourceStream(L"Stream", phr, pOwner, L"Output")
{
	m_avgTimePerFrame = 10000000 / fps;
	m_iPrefWidth = DEFAULT_WIDTH;
	m_iPrefHeight = DEFAULT_HEIGHT;
	m_llTime = 0;
	m_llFrame = 0;
	m_pReceiver = pReceiver;
	m_llLastSampleTime = -1;
}

// 破棄します。
CaptureSource::CaptureStream::~CaptureStream()
{
	if(m_pReceiver != NULL) {
		m_pReceiver->Destroy();
		m_pReceiver = NULL;
	}
}

// インタフェースを取得します。
STDMETHODIMP CaptureSource::CaptureStream::NonDelegatingQueryInterface(REFIID riid, void ** ppv)
{
	if(ppv == NULL) {
		return E_POINTER;
	}
	if(riid == IID_IKsPropertySet) {
		*ppv = (IKsPropertySet*) this;
		AddRef();
		return S_OK;
	}else if(riid == IID_IQualityControl) {
		*ppv = (IQualityControl*) this;
		AddRef();
		return S_OK;
	}else if(riid == IID_IAMStreamConfig) {
		*ppv = (IAMStreamConfig*) this;
		AddRef();
		return S_OK;
	}
	return CSourceStream::NonDelegatingQueryInterface(riid, ppv);
}

// IKsPropertySetインタフェースの実装です。
STDMETHODIMP CaptureSource::CaptureStream::QuerySupported(REFGUID rguidPropSet, ULONG ulId, PULONG pulTypeSupport)
{
	if(rguidPropSet == AMPROPSETID_Pin && ulId == AMPROPERTY_PIN_CATEGORY) {
		*pulTypeSupport = KSPROPERTY_SUPPORT_GET;
		return S_OK;
	}
	return E_NOTIMPL;
}

// IKsPropertySetインタフェースの実装です。
STDMETHODIMP CaptureSource::CaptureStream::Get(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength, PULONG pulBytesReturned)
{
	if(rguidPropSet == AMPROPSETID_Pin && ulId == AMPROPERTY_PIN_CATEGORY) {
		if(pPropertyData == NULL) {
			return E_POINTER;
		}
		if(ulDataLength != sizeof(GUID)) {
			return E_INVALIDARG;
		}
		memcpy(pPropertyData, &PIN_CATEGORY_CAPTURE, sizeof(GUID));
		*pulBytesReturned = sizeof(GUID);
		return S_OK;
	}
	return E_NOTIMPL;
}

// IKsPropertySetインタフェースの実装です。
STDMETHODIMP CaptureSource::CaptureStream::Set(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength)
{
	return E_NOTIMPL;
}

// バッファの内容を更新します。
HRESULT CaptureSource::CaptureStream::FillBuffer(IMediaSample *pSamp)
{
    CAutoLock lock(m_pFilter->pStateLock());

	HRESULT hr = S_OK;
	VIDEOINFO *pvi = (VIDEOINFO*) m_mt.Format();
	hr = pSamp->SetActualDataLength(pvi->bmiHeader.biSizeImage);
	if(FAILED(hr)) {
		return hr;
	}
	BYTE* pBuf = NULL;
	hr = pSamp->GetPointer(& pBuf);
	if(FAILED(hr)) {
		return hr;
	}
	ZeroMemory(pBuf, pvi->bmiHeader.biSizeImage);
	unsigned char* pRecvBuf = NULL;
	BUFFER_DESC recvDesc;
	ZeroMemory(& recvDesc, sizeof(recvDesc));
	LONGLONG avgTimePF = m_avgTimePerFrame / 10000;
	hr = m_pReceiver->Receive(& recvDesc, & pRecvBuf);
	if(FAILED(hr)) {
		return hr;
	}
	if(m_llLastSampleTime >= 0 && (recvDesc.time - m_llLastSampleTime) < avgTimePF / 2 && hr == S_OK) {
		// いったん待機
		hr = m_pReceiver->NotifyProcessed(pRecvBuf);
		pRecvBuf = NULL;
		if(FAILED(hr)) {
			return hr;
		}
		Sleep((DWORD) avgTimePF / 2);
		hr = m_pReceiver->Receive(& recvDesc, & pRecvBuf);
		if(FAILED(hr)) {
			return hr;
		}
	}
	HRESULT retCode = S_OK;
	if(hr == S_FALSE) {
		retCode = S_FALSE;
	}
	m_llLastSampleTime = recvDesc.time;

	int destbytes = pvi->bmiHeader.biBitCount / 8;
	int destwidthstep = pvi->bmiHeader.biSizeImage / pvi->bmiHeader.biHeight;
	double scaling = 0.0, ax = 0.0, ay = 0.0;
	hr = GetScaling(recvDesc.width, recvDesc.height, & scaling, & ax, & ay);
	if(FAILED(hr)) {
		return hr;
	}

	for(int y = 0; y < pvi->bmiHeader.biHeight; y ++) {
		for(int x = 0; x < pvi->bmiHeader.biWidth; x ++) {
			double sx = (x - ax) / scaling;
			double sy = (y - ay) / scaling;
			int isx = (int) sx;
			int isy = (int) sy;
			int destoffs = x * destbytes + y * destwidthstep;
			if(isx < 0 || isy < 0 || isx >= recvDesc.width || isy >= recvDesc.height) {
				pBuf[destoffs + 2] = 0x00;
				pBuf[destoffs + 1] = 0x00;
				pBuf[destoffs + 0] = 0x00;
				continue;
			}

			int srcoffs = isx * 3 + isy * (recvDesc.width * 3);
			pBuf[destoffs + 2] = pRecvBuf[srcoffs + 0];
			pBuf[destoffs + 1] = pRecvBuf[srcoffs + 1];
			pBuf[destoffs + 0] = pRecvBuf[srcoffs + 2];
		}
	}

	// クライアントに送信
	hr = m_pReceiver->NotifyProcessed(pRecvBuf);
	pRecvBuf = NULL;
	if(FAILED(hr)) {
		return hr;
	}

	REFERENCE_TIME startTime = m_llTime * 10000;
	REFERENCE_TIME endTime = startTime + m_avgTimePerFrame;
	hr = pSamp->SetTime(& startTime, & endTime);
	if(FAILED(hr)) {
		return hr;
	}
	LONGLONG mtStart = m_llFrame;
	LONGLONG mtEnd = mtStart + 1;
	hr = pSamp->SetMediaTime(& mtStart, & mtEnd);
	if(FAILED(hr)) {
		return hr;
	}
	m_llTime += avgTimePF;
	m_llFrame ++;
	return retCode;
}

// バッファサイズを決定します。
HRESULT CaptureSource::CaptureStream::DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pRequest)
{
	if(pAlloc == NULL || pRequest == NULL) {
		return E_POINTER;
	}
	HRESULT hr = NOERROR;
	VIDEOINFO *pvi = (VIDEOINFO*) m_mt.Format();
	pRequest->cBuffers = 1;

	// バッファサイズはビットマップ1枚分以上である必要がある
	if (pvi->bmiHeader.biSizeImage > (DWORD)pRequest->cbBuffer) {
		pRequest->cbBuffer = pvi->bmiHeader.biSizeImage;
	}
	// アロケータプロパティを設定しなおす
	ALLOCATOR_PROPERTIES actual;
	hr=pAlloc->SetProperties(pRequest, &actual);
	if (FAILED(hr)) {
		return hr;
	}
	if (actual.cbBuffer < pRequest->cbBuffer) {
		return E_FAIL;
	}
	return S_OK;
}

// 品質情報を通知します。
STDMETHODIMP CaptureSource::CaptureStream::Notify(IBaseFilter *pSelf, Quality q)
{
	return S_OK;
}

// 品質情報管理オブジェクトを設定します。
STDMETHODIMP CaptureSource::CaptureStream::SetSink(IQualityControl *piqc)
{
	return S_OK;
}

// フォーマットを設定します。
STDMETHODIMP CaptureSource::CaptureStream::SetFormat(AM_MEDIA_TYPE *pmt)
{
	if(pmt == NULL) {
		return E_POINTER;
	}
	VIDEOINFO* pvi = (VIDEOINFO*) pmt->pbFormat;
	if(pvi == NULL) {
		return E_UNEXPECTED;
	}
	m_avgTimePerFrame = pvi->AvgTimePerFrame;
	m_iPrefWidth = pvi->bmiHeader.biWidth;
	m_iPrefHeight = pvi->bmiHeader.biHeight;
	return S_OK;
}

// フォーマットを取得します。
STDMETHODIMP CaptureSource::CaptureStream::GetFormat(AM_MEDIA_TYPE **ppmt)
{
	if(ppmt == NULL) {
		return E_POINTER;
	}
	CMediaType mt;
	HRESULT hr = GetMediaType(& mt);
	if(FAILED(hr)) {
		return hr;
	}

	*ppmt = CreateMediaType(& mt);
	return S_OK;
}

// 設定情報の数を取得します。
STDMETHODIMP CaptureSource::CaptureStream::GetNumberOfCapabilities(int *piCount, int *piSize)
{
	if(piCount == NULL || piSize == NULL) {
		return E_POINTER;
	}
	*piCount = 1;
	*piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);
	return S_OK;
}

// 設定情報を取得します。
STDMETHODIMP CaptureSource::CaptureStream::GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC)
{
	if(ppmt == NULL || pSCC == NULL) {
		return E_POINTER;
	}

	CMediaType mt;
	HRESULT hr = GetMediaType(& mt);
	if(FAILED(hr)) {
		return hr;
	}
	*ppmt = CreateMediaType(& mt);

	VIDEO_STREAM_CONFIG_CAPS* pCaps = (VIDEO_STREAM_CONFIG_CAPS*) pSCC;
	ZeroMemory(pCaps, sizeof(VIDEO_STREAM_CONFIG_CAPS));

	pCaps->guid = FORMAT_VideoInfo;
	pCaps->VideoStandard = 0;
	pCaps->InputSize.cx = MAX_WIDTH;
	pCaps->InputSize.cy = MAX_HEIGHT;
	pCaps->MinCroppingSize.cx = MIN_WIDTH;
	pCaps->MinCroppingSize.cy = MIN_HEIGHT;
	pCaps->MaxCroppingSize.cx = MIN_WIDTH;
	pCaps->MaxCroppingSize.cy = MIN_HEIGHT;
	pCaps->CropGranularityX = MIN_WIDTH;
	pCaps->CropGranularityY = MIN_HEIGHT;
	pCaps->CropAlignX = 1;
	pCaps->CropAlignY = 1;
	pCaps->MinOutputSize.cx = MIN_WIDTH;
	pCaps->MinOutputSize.cy = MIN_HEIGHT;
	pCaps->MaxOutputSize.cx = MAX_WIDTH;
	pCaps->MaxOutputSize.cy = MAX_HEIGHT;
	pCaps->OutputGranularityX = 1;
	pCaps->OutputGranularityY = 1;
	pCaps->StretchTapsX = 0;
	pCaps->StretchTapsY = 0;
	pCaps->ShrinkTapsX = 0;
	pCaps->ShrinkTapsY = 0;
	pCaps->MinFrameInterval = MIN_FRAME_INTERVAL;
	pCaps->MaxFrameInterval = MAX_FRAME_INTERVAL;
	pCaps->MinBitsPerSecond = MIN_WIDTH * MIN_HEIGHT * 3 * 8 * MIN_FPS;
	pCaps->MaxBitsPerSecond = MAX_WIDTH * MAX_HEIGHT * 3 * 8 * MAX_FPS;

	return S_OK;
}

// メディアタイプを取得します。
HRESULT CaptureSource::CaptureStream::GetMediaType(CMediaType *pMediaType)
{
	if(pMediaType == NULL) {
		return E_POINTER;
	}
	VIDEOINFO *pvi = (VIDEOINFO *)pMediaType->AllocFormatBuffer(sizeof(VIDEOINFO));
	ZeroMemory(pvi, sizeof(VIDEOINFO));
	pvi->AvgTimePerFrame = m_avgTimePerFrame;
	BITMAPINFOHEADER *pBmi = &(pvi->bmiHeader);
	pBmi->biSize = sizeof(BITMAPINFOHEADER);
	pBmi->biWidth = m_iPrefWidth;
	pBmi->biHeight = m_iPrefHeight;
	pBmi->biPlanes = 1;
	pBmi->biBitCount = 24;
	pBmi->biCompression = BI_RGB;
	pvi->bmiHeader.biSizeImage = DIBSIZE(pvi->bmiHeader);

	pMediaType->SetType(&MEDIATYPE_Video);
	pMediaType->SetFormatType(&FORMAT_VideoInfo);
	const GUID subtype=GetBitmapSubtype(&pvi->bmiHeader);
	pMediaType->SetSubtype(&subtype);
	pMediaType->SetSampleSize(DIBSIZE(*pBmi));
	pMediaType->SetTemporalCompression(FALSE);
	return S_OK;
}

// スレッド開始を通知します。
HRESULT CaptureSource::CaptureStream::OnThreadStartPlay(void)
{
	m_llTime = 0;
	m_llFrame = 0;
	m_llLastSampleTime = -1;

	return CSourceStream::OnThreadStartPlay();
}

// スケーリング関係パラメータを取得します。
HRESULT CaptureSource::CaptureStream::GetScaling(int width, int height, double* pScaling, double* pOffsetX, double* pOffsetY)
{
	if(pScaling == NULL || pOffsetX == NULL || pOffsetY == NULL) {
		return E_POINTER;
	}
	VIDEOINFO *pvi = (VIDEOINFO*) m_mt.Format();
	BITMAPINFOHEADER *pBmi = &(pvi->bmiHeader);
	
	double aw = pBmi->biWidth;
	double ah = pBmi->biHeight;
	double sw = width;
	double sh = height;
	double wscaling = aw / sw;
	double hscaling = ah / sh;
	double scaling = min(wscaling, hscaling);
	if(scaling < 0.0001) {
		return E_UNEXPECTED;
	}

	double dw = scaling * sw;
	double dh = scaling * sh;
	double ax = (aw - dw) / 2.0;
	double ay = (ah - dh) / 2.0;
	*pScaling = scaling;
	*pOffsetX = ax;
	*pOffsetY = ay;
	return S_OK;
}
