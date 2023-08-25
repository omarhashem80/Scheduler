#include "SJFProcessor.h"
#include"Scheduler.h"
#include<iostream>
using namespace std;
SJFProcessor::SJFProcessor(Scheduler*ptr,int Period):Processor(ptr,Period)
{}
void SJFProcessor::SendToReady(Process* ptr)
{
	if (ptr)
	{
		Pair<Process*, int>pa(ptr, ptr->getCT());
		Time += ptr->getRemainingTime();                // increase the time of processor
		Ready.enqueue(pa);							    // put the process in ready list
		items++;									   // increase the num of processes in ready
	}
}

void SJFProcessor::schedule(int Clock)
{
	outofdate();     // do overheating process
	if (!(Ready.isempty()) || Run)         // check if there is any process in the processor
	{
		if (!Busy)                     // if there is not a running process
		{
			Run = 0;
			Pair<Process*, int>pa(0, 0);
			Ready.dequeue(pa);                  // take one from ready
			Run = pa.getfirst();                // put it in run state
			items--;                           //decrement the ready list size
			Run->setRT(Clock);                   //set response time of process
			Busy = 1;                         // put the processor in busy state
		}
		if (Run)                         // if there is a process in the run state
		{
			Run->incrementadvance();             // increament its time step
			Time--;                               //decrement the time of the processor
			RunToBlock();                     // handling if it request io
			RunToTerm();                        // check if the process is finish
		}
	}
	else
	{
		Busy = 0;        // if not set to process to IDEL state
		Time = 0;
	}
	TBusy += Busy;             // increment the total busy time
	TIDLE += !Busy;            // increment the total IDEL time
}


void SJFProcessor::print()
{
	cout << "[SJF]: " << items << ": " << "RDY: ";
	PriorityQueue <Pair<Process*, int>> printed(Ready);
	Pair<Process*, int> pa(0, 0);
	Process* p;
	while (printed.dequeue(pa))
	{
		p = pa.getfirst();
		if (printed.isempty())
			cout << p->getPID();
		else cout << p->getPID() << ", ";
	}
	cout << endl;
	cout << "Time To finish: " << Time << "\tState: " << (OverHeat ? "OverHeated" : "Normal") << endl;
}

Process* SJFProcessor::Steal()
{
	Pair<Process*, int> pa(0, 0);                  // create pair 
	Process* P = 0;
	Ready.dequeue(pa);							   // dequeue one from ready
	P = pa.getfirst();							   // make P point to it
	if (P)										     // if P point to any process
	{
		items--;								   // decrement the num of items
		Time -= P->getRemainingTime();			   // decreament the time of Processor
	}
	return P;
}

bool SJFProcessor::outofdate()
{
	if (OverHeat)                    // if the processor is overheated
	{
		Overheat_Counter--;          // decreament the Overheat_Counter of Processor
		if (!Overheat_Counter)          // check if the Overheat_Period is end
			OverHeat = 0;            // if true set the processor normal
	}
	else
	{
		int Radix = getRandomNumber();                 // get random number
		if (Radix < 2 && (Run || !(Ready.isempty())))         // if it is less  and  there is any process in the processor
		{
			OverHeat = 1;                              // if true set the processor Overheated          
			Overheat_Counter = Overheat_Period;        // set over heated counter to Overheated peroid
			if (Run)                                  // if there is one in run
			{
				Sptr->MovetoWaiting(Run);              //send it to secheduler to handle it
				Time -= Run->getRemainingTime();         // decreament the time of Processor 
				Run = 0;                               // set run to null
			}
			Pair<Process*, int>Pa(0, 0);
			while (Ready.dequeue(Pa))                 //send all processes in ready to  secheduler to handle them if any
			{
				Run = Pa.getfirst();
				Time -= Run->getRemainingTime();
				Sptr->MovetoWaiting(Run);
				Run = 0;
				items--;
				Pa = { 0, 0 };
			}
			Busy = 0;        //set the busy to false
		}
	}
		return OverHeat;     //return the state 
}

