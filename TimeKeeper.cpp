/**
 *  LennonLam @ https://github.com/lennonlam
 */

#include <Windows.h>
#include "TimeKeeper.h"

#define SAFE_DEL(ptr)       { if (ptr) {delete ptr; ptr = NULL;} }

/// <!--====================================================================-->
///     A rough timer using C-library <time> as core timer
/// <!--====================================================================-->
SimpleTimer::SimpleTimer() 
{ 
    m_st = 0; 
};
SimpleTimer::SimpleTimer(const SimpleTimer& timer) 
{ 
    this->m_st = timer.m_st;
};
SimpleTimer& SimpleTimer::operator= (const SimpleTimer& timer) 
{ 
    if (this != &timer)
        this->m_st = timer.m_st; 
    return *this; 
} 
SimpleTimer* SimpleTimer::Clone() const 
{ 
    return new SimpleTimer(*this); 
};
SimpleTimer::~SimpleTimer() {};
void   SimpleTimer::Start() 
{ 
    m_st = clock(); 
};
double SimpleTimer::Lap()
{ 
    return m_st > 0 ? (double)(clock()-m_st) / (double)CLOCKS_PER_SEC : 0; 
};
double SimpleTimer::Stop()  
{ 
    double t = Lap();
    m_st = 0;
    return t; 
};

/// <!--====================================================================-->
///     A more precise timer (<1ms) using WinAPI as core timer 
/// <!--====================================================================-->
PreciseTimer::PreciseTimer()
{
	QueryPerformanceFrequency(&m_freq);
    memset(&m_st, 0, sizeof(LARGE_INTEGER));
}
PreciseTimer::PreciseTimer(const PreciseTimer& timer) 
{ 
    this->m_st   = timer.m_st; 
    this->m_freq = timer.m_freq; 
};
PreciseTimer& PreciseTimer::operator= (const PreciseTimer& timer) 
{ 
    if (this != &timer)
    {
        this->m_st   = timer.m_st; 
        this->m_freq = timer.m_freq; 
    }
    return *this; 
} 
PreciseTimer* PreciseTimer::Clone() const 
{ 
    return new PreciseTimer(*this); 
};
void PreciseTimer::Start()
{
	QueryPerformanceCounter(&m_st);
}
double PreciseTimer::Lap()
{
    LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
    return m_st.QuadPart > 0 ? (double)((end.QuadPart-m_st.QuadPart) / (double)m_freq.QuadPart) : 0.0;
}
double PreciseTimer::Stop()
{
    double t = Lap();
    memset(&m_st, 0, sizeof(LARGE_INTEGER));
    return t;
}

/// <!--====================================================================-->
///     A stopwatch provides more function than a timer.
///     Stopwatch allows user to keep the result time and get the time later 
/// <!--====================================================================-->
Stopwatch:: Stopwatch(VirtualTimer* timer)
{
    if (timer == NULL)
        timer = new PreciseTimer();
	m_timer = timer;
    Reset();
}
Stopwatch::Stopwatch(const Stopwatch& sw)
{
    this->m_timer = sw.m_timer->Clone();
    this->m_time = sw.m_time;
}
Stopwatch& Stopwatch::operator= (const Stopwatch& sw)
{
    if (this != &sw)
    {
        SAFE_DEL(this->m_timer);
        this->m_timer = sw.m_timer->Clone();
        this->m_time = sw.m_time;
    }
    return *this;
}
Stopwatch::~Stopwatch()     { SAFE_DEL(m_timer); }
void   Stopwatch::Start()   { m_timer->Start(); };
double Stopwatch::Lap()     { return m_time + m_timer->Lap(); };
double Stopwatch::Stop()    { return m_time += m_timer->Stop(); };
void   Stopwatch::Reset()   { m_time = 0; };

/// <!--====================================================================-->
///     Time keeper is simply a collection of stopwatches. 
///     User can manipulate the stopwatch by token.
/// <!--====================================================================-->
TimeKeeper::TimeKeeper() 
{
};
TimeKeeper::TimeKeeper(const TimeKeeper& tk)
{
    for (map<string, Stopwatch*>::const_iterator it = tk.m_sw.begin(); it != tk.m_sw.end(); ++it)
        this->m_sw[(*it).first] = new Stopwatch(*((*it).second)); ;
}
TimeKeeper& TimeKeeper::operator= (const TimeKeeper& tk)
{
    if (this != &tk)
    {
        for (map<string, Stopwatch*>::const_iterator it = tk.m_sw.begin(); it != tk.m_sw.end(); ++it)
            this->m_sw[(*it).first] = new Stopwatch(*((*it).second)); ;
    }
    return *this;
}
TimeKeeper::~TimeKeeper()
{
    for (m_it = m_sw.begin(); m_it != m_sw.end(); ++m_it)
        SAFE_DEL((*m_it).second);
};
void TimeKeeper::Start(const string token, VirtualTimer* specTimer)
{
    m_it = m_sw.find(token);
    if (m_it == m_sw.end())
        m_sw[token] = new Stopwatch(specTimer);
    else
        SAFE_DEL(specTimer);

    m_sw[token]->Start();
};
double TimeKeeper::Lap(const string token)
{
    m_it = m_sw.find(token);
    return m_it == m_sw.end() ? -1 : (*m_it).second->Lap();
};
double TimeKeeper::Stop(const string token)
{
    m_it = m_sw.find(token);
    return m_it == m_sw.end() ? -1 : (*m_it).second->Stop();
};
void TimeKeeper::Reset(const string token)
{
    m_it = m_sw.find(token);
    if (m_it != m_sw.end())
        (*m_it).second->Reset();
};
