#pragma once
#include "YCMutex.h"

/**
* �N���e�B�J���Z�N�V�����ł��B
*/
class YCCritSection
{
public:

	/**
	* �\�z���܂��B
	*/
	YCCritSection(YCMutex* pMutex)
	{
		pMutex->Begin();
		m_pMutex = pMutex;
	};

	/**
	* �j�����܂��B
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
	* Mutex��ێ����܂��B
	*/
	YCMutex* m_pMutex;

};
