#include "VCriticalSection.h"




CVCriticalSection::CVCriticalSection(void)
{
	::InitializeCriticalSection(&m_cs);
}

CVCriticalSection::~CVCriticalSection(void)
{
	::DeleteCriticalSection(&m_cs);
}

void CVCriticalSection::enter()
{
	::EnterCriticalSection(&m_cs);
}

void CVCriticalSection::leave()
{
	::LeaveCriticalSection(&m_cs);
}

