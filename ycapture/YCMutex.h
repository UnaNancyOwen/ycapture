#pragma once

/**
* �����I�u�W�F�N�g�ł��B
*/
class YCMutex
{
public:

	/**
	* �\�z���܂��B
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
	* �j�����܂��B
	*/
	virtual ~YCMutex(void)
	{
		if(m_crit != NULL) {
			CloseHandle(m_crit);
			m_crit = NULL;
		}
	};
	
	/**
	* �J�n���܂��B
	*/
	void Begin()
	{
		WaitForSingleObject(m_crit, INFINITE);
	};
	
	/**
	* �I�����܂��B
	*/
	void End()
	{
		ReleaseMutex(m_crit);
	};

private:

	/**
	* �N���e�B�J���Z�N�V������ێ����܂��B
	*/
	HANDLE m_crit;

};
