#pragma once
#include "Processor.h"
#include"PriorityQueue.h"
#include"Pair.h"
class EDFProcessor :public Processor
{
private:
	PriorityQueue<Pair<Process*,int>> Ready;
public:
	EDFProcessor(Scheduler*,int);
	void SendToReady(Process*);
	void schedule(int);
	void print();
	Process* Steal();
	bool outofdate();
};

