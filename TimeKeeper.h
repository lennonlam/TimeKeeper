/**
 *  LennonLam @ https://github.com/lennonlam
 */
#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <WinNT.h>

using namespace std;

/// <!--====================================================================-->
///     Pure virtual timer base class
/// <!--====================================================================-->
class VirtualTimer
{
public:
	VirtualTimer() {};
    virtual VirtualTimer* Clone() const = 0;    // Virtual (copy) constructor
	virtual ~VirtualTimer() {};

    //! Start counting
    virtual void   Start() = 0;

    //! Get lap time (in sec)
    virtual double Lap()   = 0;

    //! Get final time (in sec) and stop counting
    virtual double Stop()  = 0;

private: 
};


/// <!--====================================================================-->
///     A rough timer using C-library <time> as core timer
/// <!--====================================================================-->
class SimpleTimer : public VirtualTimer
{
public:
    SimpleTimer();
    SimpleTimer(const SimpleTimer& timer);
    SimpleTimer& operator= (const SimpleTimer& timer);
    SimpleTimer* Clone() const;
    ~SimpleTimer();

    void   Start();
    double Lap();
    double Stop();
private:
    time_t m_st;
};

/// <!--====================================================================-->
///     A more precise timer (<1ms) using WinAPI as core timer 
/// <!--====================================================================-->
class PreciseTimer : public VirtualTimer
{
public:
    PreciseTimer();
    PreciseTimer(const PreciseTimer& timer);
    PreciseTimer& operator= (const PreciseTimer& timer);
    PreciseTimer* Clone() const;
	~PreciseTimer() {};

    void   Start();
    double Lap();
    double Stop();
private:
	LARGE_INTEGER m_st, m_freq; 
};

/// <!--====================================================================-->
///     A stopwatch provides more function than a timer.
///     Stopwatch allows user to keep the result time and get the time later 
///     And it allows user to start timing after Stop(). 
/// <!--====================================================================-->
class Stopwatch
{
public:
    Stopwatch(VirtualTimer* timer = new PreciseTimer());
    Stopwatch(const Stopwatch& sw);
    Stopwatch& operator= (const Stopwatch& sw);
    ~Stopwatch();

    //! Start counting (again)
    void   Start();

    //! Get total time (in sec) in count
    double Lap();

    //! Get time (in sec) and stop counting, but the counted time will NOT be erased
    //! Counting can be resumed by Start()
    double Stop();

    //! Erased counted time
    void   Reset();

private:
	VirtualTimer* m_timer;
    double        m_time;
};


/// <!--====================================================================-->
///     Time keeper is simply a collection of stopwatches. 
///     User can manipulate the stopwatch by token.
/// <!--====================================================================-->
class TimeKeeper
{
public: 
    TimeKeeper();
    TimeKeeper(const TimeKeeper& tk);
    TimeKeeper& operator= (const TimeKeeper& tk);
    ~TimeKeeper();
    
    //! Start stopwatch with a token
    //! Timekeeper will create a new stopwatch for this token if token is new
    void   Start  (const string token, VirtualTimer* specTimer = new PreciseTimer());

    //! Get lap time of a token stopwatch. 
    //! -1 indicates invalid token
    double Lap    (const string token);

    //! Get final time of a token stopwatch and stop it. 
    //! -1 indicates invalid token
    double Stop   (const string token); 

    //! Reset a token stopwatch
    void   Reset  (const string token);  

private:
    map<string, Stopwatch*>             m_sw;
    map<string, Stopwatch*>::iterator   m_it;
};




#endif // _TIME_KEEPER_H_
