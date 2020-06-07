#pragma once

#include "ycapture.h"
#include "IYCSource.h"
class IReceiver;

/**
* フィルタ定義です。
*/
extern const AMOVIESETUP_FILTER sudCaptureSource;
extern const AMOVIESETUP_PIN sudCaptureSourceOut;

/**
* キャプチャソースフィルタです。
*/
class CaptureSource :
	public CSource, public IQualityControl, public IYCSource
{
public:

	/**
	* インスタンスを生成します。
	*/
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

	/**
	* 構築します。
	*/
	CaptureSource(LPUNKNOWN lpunk, HRESULT* phr);

public:

	/**
	* 破棄します。
	*/
	virtual ~CaptureSource(void);

	DECLARE_IUNKNOWN;

	/**
	* インタフェースを取得します。
	*/
    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	/**
	* 品質情報を通知します。
	*/
	virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

	/**
	* 品質情報管理オブジェクトを設定します。
	*/
	virtual STDMETHODIMP SetSink(IQualityControl *piqc);

private:

	/**
	* 初期化を行ないます。
	*/
	HRESULT Init(IReceiver* pReceiver, int fps);

	/**
	* キャプチャストリームです。
	*/
	class CaptureStream : public CSourceStream, public IKsPropertySet, public IAMStreamConfig
	{
	public:

		/**
		* 構築します。
		*/
		CaptureStream(CaptureSource* pOwner, IReceiver* pReceiver, int fps, HRESULT* phr);

		/**
		* 破棄します。
		*/
		virtual ~CaptureStream();

		DECLARE_IUNKNOWN;

		/**
		* インタフェースを取得します。
		*/
		virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

		/**
		* IKsPropertySetインタフェースの実装です。
		*/
		virtual STDMETHODIMP QuerySupported(REFGUID rguidPropSet, ULONG ulId, PULONG pulTypeSupport);

		/**
		* IKsPropertySetインタフェースの実装です。
		*/
		virtual STDMETHODIMP Get(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength, PULONG pulBytesReturned);

		/**
		* IKsPropertySetインタフェースの実装です。
		*/
		virtual STDMETHODIMP Set(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength);

		/**
		* バッファの内容を更新します。
		*/
		virtual HRESULT FillBuffer(IMediaSample *pSamp);

		/**
		* バッファサイズを決定します。
		*/
		virtual HRESULT DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pRequest);

		/**
		* 品質情報を通知します。
		*/
		virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

		/**
		* 品質情報管理オブジェクトを設定します。
		*/
		virtual STDMETHODIMP SetSink(IQualityControl *piqc);

		/**
		* フォーマットを設定します。
		*/
        virtual STDMETHODIMP SetFormat(AM_MEDIA_TYPE *pmt);
        
		/**
		* フォーマットを取得します。
		*/
        virtual STDMETHODIMP GetFormat(AM_MEDIA_TYPE **ppmt);
        
		/**
		* 設定情報の数を取得します。
		*/
        virtual STDMETHODIMP GetNumberOfCapabilities(int *piCount, int *piSize);
        
		/**
		* 設定情報を取得します。
		*/
        virtual STDMETHODIMP GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC);

	protected:

		/**
		* メディアタイプを取得します。
		*/
	    virtual HRESULT GetMediaType(CMediaType *pMediaType);

		/**
		* スレッド開始を通知します。
		*/
		virtual HRESULT OnThreadStartPlay(void);

	private:

		/**
		* スケーリング関係パラメータを取得します。
		*/
		HRESULT GetScaling(int width, int height, double* pScaling, double* pOffsetX, double* pOffsetY);

		/**
		* 時刻を保持します。
		*/
		LONGLONG m_llTime;

		/**
		* フレームを保持します。
		*/
		LONGLONG m_llFrame;

		/**
		* フレームレートを保持します。
		*/
		REFERENCE_TIME m_avgTimePerFrame;

		/**
		* 推奨幅を保持します。
		*/
		int m_iPrefWidth;

		/**
		* 推奨高さを保持します。
		*/
		int m_iPrefHeight;

		/**
		* 最終サンプルの時刻を保持します。
		*/
		LONGLONG m_llLastSampleTime;

		/**
		* 受信オブジェクトを保持します。
		*/
		IReceiver* m_pReceiver;

	};

};
