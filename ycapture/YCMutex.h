#pragma once

/**
* 同期オブジェクトです。
*/
class YCMutex
{
public:

	/**
	* 構築します。
	*/
	YCMutex(const wchar_t* name, bool create, HRESULT* phr)
	{
		if(create) {
			m_crit = CreateMutex(NULL, FALSE, name);
		}else{
			m_crit = OpenMutex(SYNCHRONIZE, FALSE, name);
		}
		if(m_crit == NULL) {
			*phr = E_UNEXPECTED;
		}
	};

	/**
	* 破棄します。
	*/
	virtual ~YCMutex(void)
	{
		if(m_crit != NULL) {
			CloseHandle(m_crit);
			m_crit = NULL;
		}
	};
	
	/**
	* 開始します。
	*/
	void Begin()
	{
		WaitForSingleObject(m_crit, INFINITE);
	};
	
	/**
	* 終了します。
	*/
	void End()
	{
		ReleaseMutex(m_crit);
	};

private:

	/**
	* クリティカルセクションを保持します。
	*/
	HANDLE m_crit;

};
