//-----------------------------------------------------------------------------
// File: CGameTimer.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Timer.h"

CGameTimer::CGameTimer()
{
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerformanceFrequency)) 
    { 
		m_bHardwareHasPerformanceCounter = TRUE;
		QueryPerformanceCounter((LARGE_INTEGER *)&m_nLastTime); 
		m_fTimeScale = 1.0f / m_PerformanceFrequency;
	} 
    else 
    { 
		m_bHardwareHasPerformanceCounter = FALSE;
		m_nLastTime	= ::timeGetTime(); 
		m_fTimeScale = 0.001f;	
    }

    m_nSampleCount = 0;
	m_nCurrentFrameRate = 0;
	m_nFramePerSecond = 0;
	m_fFPSTimeElapsed = 0.0f;
}

CGameTimer::~CGameTimer()
{
}

void CGameTimer::Tick(float fLockFPS)
{
    float fTimeElapsed, fRealTimeElapsed;

	if (m_bHardwareHasPerformanceCounter) 
    {
		QueryPerformanceCounter((LARGE_INTEGER *)&m_nCurrentTime);
		m_nRealCurrentTime = m_nCurrentTime;
	} 
    else 
    {
		m_nCurrentTime = ::timeGetTime();
		m_nRealCurrentTime = m_nCurrentTime;
	} 

	fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
	fRealTimeElapsed = (m_nRealCurrentTime - m_nLastTime) *m_fTimeScale;

    if (fLockFPS > 0.0f)
    {
        while (fTimeElapsed < (1.0f / fLockFPS))
        {
	        if (m_bHardwareHasPerformanceCounter) 
            {
		        QueryPerformanceCounter((LARGE_INTEGER *)&m_nCurrentTime);
	        } 
            else 
            {
				m_nCurrentTime = ::timeGetTime();
	        } 
	        // Calculate elapsed time in seconds
	        fTimeElapsed = (m_nCurrentTime - m_nLastTime) * m_fTimeScale;
        }
    } 

	m_nLastTime = m_nCurrentTime;

    if (fabsf(fTimeElapsed - m_fTimeElapsed) < 1.0f)
    {
        // Wrap FIFO frame time buffer.
        memmove(&m_fFrameTime[1], m_fFrameTime, (MAX_SAMPLE_COUNT - 1) * sizeof(float));
        m_fFrameTime[0] = fTimeElapsed;
        if (m_nSampleCount < MAX_SAMPLE_COUNT) m_nSampleCount++;
    }

	// Calculate Frame Rate
	m_nFramePerSecond++;
	m_nRealFramePerSecond++;

	m_fFPSTimeElapsed += fTimeElapsed;
	m_fRealFPSTimeElapsed += fRealTimeElapsed;

	if (m_fFPSTimeElapsed > 1.0f) 
    {
		m_nCurrentFrameRate	= m_nFramePerSecond;
		m_nFramePerSecond = 0;
		m_fFPSTimeElapsed = 0.0f;
	} 

	if (m_fRealFPSTimeElapsed > 1.0f)
	{
		m_nRealFrameRate = m_nRealFramePerSecond;
		m_nRealFramePerSecond = 0;
		m_fRealFPSTimeElapsed = 0.0f;
	}

    // Count up the new average elapsed time
    m_fTimeElapsed = 0.0f;
    for (ULONG i = 0; i < m_nSampleCount; i++) m_fTimeElapsed += m_fFrameTime[i];
    if (m_nSampleCount > 0) m_fTimeElapsed /= m_nSampleCount;


	SCENE_MGR->fTimeElapsed = fTimeElapsed;
	SCENE_MGR->fFrameRate = GetFrameRate();
}

void CGameTimer::SetTitleName(wstring& wStr)
{
	wStr = PROJECT_NAME;

	WCHAR str[50];
	_itow(m_nCurrentFrameRate, str, 10);

	wStr += str;
	wStr += L" FPS, ";

	_itow(m_nRealFrameRate, str, 10);
	wStr += str;
	wStr += L" FPS)";
}