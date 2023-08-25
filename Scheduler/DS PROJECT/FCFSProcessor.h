#pragma once
#include "Processor.h"
#include"ProcessList.h"
class FCFSProcessor :public Processor
{
private:
	ProcessList Ready;
	int MaxW;
public:
	FCFSProcessor(Scheduler*,int,int);
	void SendToReady(Process*);
	void ReadyToRun(int);
	void schedule(int);
	Process* Steal();
	Process* kill(int id);
	void Fork();
	void print();
	bool Migrate(int Clock);
	bool outofdate();
};




