#pragma once
#include"LinkedQueue.h"
#include"Processor.h"
class RRProcessor : public Processor
{
private:
	LinkedQueue<Process*>Ready;
	int Slicetime;
	int count;
	int RTF;
public:
	RRProcessor(int ,Scheduler*,int,int);
	void SendToReady(Process*);
	bool RunToReady();
	void ReadyToRun(int);
	void schedule(int);
	void print();
	Process* Steal();
	bool Migrate(int);
	bool outofdate();
};




