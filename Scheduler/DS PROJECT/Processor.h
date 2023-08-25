#ifndef _PROCESSOR
#define _PROCESSOR
#include"Process.h"
class Scheduler;
class Processor
{
protected:
	Scheduler* Sptr;
	bool Busy;
	Process* Run;
	int Time;
	int Type;
	int items;
	int TBusy;
	int TIDLE;
	bool OverHeat;
	int getRandomNumber();
	int Overheat_Period;
	int Overheat_Counter;
public:
	Processor(Scheduler*,int);
	bool GetBusy();
	void SetBusy(bool b);
	virtual bool RunToTerm();
	virtual bool RunToBlock();
	int GetTime();
	int GetTBusy();
	int GetTIDLE();
	bool Is_OverHeat();
	virtual void SendToReady(Process*) = 0;
	virtual void schedule(int) = 0;
	virtual void print()=0;
	virtual bool outofdate() = 0;
	virtual Process* Steal() = 0;
	int Gettype();
	float Getload(float);
	float Getutil();
	Process* GETRUN();
};
#endif




