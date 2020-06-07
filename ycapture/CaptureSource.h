#pragma once

#include "ycapture.h"
#include "IYCSource.h"
class IReceiver;

/**
* �t�B���^��`�ł��B
*/
extern const AMOVIESETUP_FILTER sudCaptureSource;
extern const AMOVIESETUP_PIN sudCaptureSourceOut;

/**
* �L���v�`���\�[�X�t�B���^�ł��B
*/
class CaptureSource :
	public CSource, public IQualityControl, public IYCSource
{
public:

	/**
	* �C���X�^���X�𐶐����܂��B
	*/
    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

	/**
	* �\�z���܂��B
	*/
	CaptureSource(LPUNKNOWN lpunk, HRESULT* phr);

public:

	/**
	* �j�����܂��B
	*/
	virtual ~CaptureSource(void);

	DECLARE_IUNKNOWN;

	/**
	* �C���^�t�F�[�X���擾���܂��B
	*/
    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	/**
	* �i������ʒm���܂��B
	*/
	virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

	/**
	* �i�����Ǘ��I�u�W�F�N�g��ݒ肵�܂��B
	*/
	virtual STDMETHODIMP SetSink(IQualityControl *piqc);

private:

	/**
	* ���������s�Ȃ��܂��B
	*/
	HRESULT Init(IReceiver* pReceiver, int fps);

	/**
	* �L���v�`���X�g���[���ł��B
	*/
	class CaptureStream : public CSourceStream, public IKsPropertySet, public IAMStreamConfig
	{
	public:

		/**
		* �\�z���܂��B
		*/
		CaptureStream(CaptureSource* pOwner, IReceiver* pReceiver, int fps, HRESULT* phr);

		/**
		* �j�����܂��B
		*/
		virtual ~CaptureStream();

		DECLARE_IUNKNOWN;

		/**
		* �C���^�t�F�[�X���擾���܂��B
		*/
		virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

		/**
		* IKsPropertySet�C���^�t�F�[�X�̎����ł��B
		*/
		virtual STDMETHODIMP QuerySupported(REFGUID rguidPropSet, ULONG ulId, PULONG pulTypeSupport);

		/**
		* IKsPropertySet�C���^�t�F�[�X�̎����ł��B
		*/
		virtual STDMETHODIMP Get(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength, PULONG pulBytesReturned);

		/**
		* IKsPropertySet�C���^�t�F�[�X�̎����ł��B
		*/
		virtual STDMETHODIMP Set(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength);

		/**
		* �o�b�t�@�̓��e���X�V���܂��B
		*/
		virtual HRESULT FillBuffer(IMediaSample *pSamp);

		/**
		* �o�b�t�@�T�C�Y�����肵�܂��B
		*/
		virtual HRESULT DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pRequest);

		/**
		* �i������ʒm���܂��B
		*/
		virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

		/**
		* �i�����Ǘ��I�u�W�F�N�g��ݒ肵�܂��B
		*/
		virtual STDMETHODIMP SetSink(IQualityControl *piqc);

		/**
		* �t�H�[�}�b�g��ݒ肵�܂��B
		*/
        virtual STDMETHODIMP SetFormat(AM_MEDIA_TYPE *pmt);
        
		/**
		* �t�H�[�}�b�g���擾���܂��B
		*/
        virtual STDMETHODIMP GetFormat(AM_MEDIA_TYPE **ppmt);
        
		/**
		* �ݒ���̐����擾���܂��B
		*/
        virtual STDMETHODIMP GetNumberOfCapabilities(int *piCount, int *piSize);
        
		/**
		* �ݒ�����擾���܂��B
		*/
        virtual STDMETHODIMP GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC);

	protected:

		/**
		* ���f�B�A�^�C�v���擾���܂��B
		*/
	    virtual HRESULT GetMediaType(CMediaType *pMediaType);

		/**
		* �X���b�h�J�n��ʒm���܂��B
		*/
		virtual HRESULT OnThreadStartPlay(void);

	private:

		/**
		* �X�P�[�����O�֌W�p�����[�^���擾���܂��B
		*/
		HRESULT GetScaling(int width, int height, double* pScaling, double* pOffsetX, double* pOffsetY);

		/**
		* ������ێ����܂��B
		*/
		LONGLONG m_llTime;

		/**
		* �t���[����ێ����܂��B
		*/
		LONGLONG m_llFrame;

		/**
		* �t���[�����[�g��ێ����܂��B
		*/
		REFERENCE_TIME m_avgTimePerFrame;

		/**
		* ��������ێ����܂��B
		*/
		int m_iPrefWidth;

		/**
		* ����������ێ����܂��B
		*/
		int m_iPrefHeight;

		/**
		* �ŏI�T���v���̎�����ێ����܂��B
		*/
		LONGLONG m_llLastSampleTime;

		/**
		* ��M�I�u�W�F�N�g��ێ����܂��B
		*/
		IReceiver* m_pReceiver;

	};

};
