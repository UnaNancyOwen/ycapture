#pragma once
#include "YCMutex.h"

/**
* クリティカルセクションです。
*/
class YCCritSection
{
public:

	/**
	* 構築します。
	*/
	YCCritSection(YCMutex* pMutex)
	{
		pMutex->Begin();
		m_pMutex = pMutex;
	};

	/**
	* 破棄します。
	*/
	virtual ~YCCritSection()
	{
		if(m_pMutex != NULL) {
			m_pMutex->End();
			m_pMutex = NULL;
		}
	};
	
private:

	/**
	* Mutexを保持します。
	*/
	YCMutex* m_pMutex;

};
