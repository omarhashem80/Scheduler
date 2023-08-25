#pragma once
#include "Processor.h"
//#include"PriorityProcess.h"
#include"PriorityQueue.h"
#include"Pair.h"
class SJFProcessor :public Processor
{
	PriorityQueue<Pair<Process*,int>> Ready;   //PriorityQueue sorted by CT
public:
	SJFProcessor(Scheduler*,int);
	void SendToReady(Process*);
	void schedule(int);
	void print();
	Process* Steal();
	bool outofdate();
};




