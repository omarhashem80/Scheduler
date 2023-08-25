#include "EDFProcessor.h"
#include"Scheduler.h"
#include<iostream>
using namespace std;
EDFProcessor::EDFProcessor(Scheduler* ptr, int Period):Processor(ptr,Period)
{}
void EDFProcessor::SendToReady(Process*ptr)
{
	if (ptr)
	{
		if (Run && ptr->getDeadline() < Run->getDeadline())
		{
			Time += ptr->getRemainingTime();
			Ready.enqueue(Run);
			Run = ptr;
			items++;
		}
		else
		{
			Pair<Process*, int> pa(ptr, ptr->getDeadline());
			Time += ptr->getRemainingTime();                // increase the time of processor
			Ready.enqueue(pa);							    // put the process in ready list
			items++;									   // increase the num of processes in ready
		}
	}

}

void EDFProcessor::schedule(int Clock)
{
	outofdate();
	if (!(Ready.isempty()) || Run)
	{
		if (!Busy)
		{
			Pair<Process*, int> pa(0, 0);
			Run = 0;
			Ready.dequeue(pa);
			Run = pa.getfirst();
			items--;
			Run->setRT(Clock);
			Busy = 1;
		}
		if (Run)
		{
			Run->incrementadvance();
			Time--;
			RunToBlock();
			RunToTerm();                        // check if the process is finish
		}
	}
	else
	{
		Busy = 0;
		Time = 0;
	}
	TBusy += Busy;
	TIDLE += !Busy;
}


void EDFProcessor::print()
{
	cout << "[EDF]: " << items << ": " << "RDY: ";
	PriorityQueue <Pair<Process*, int>> printed(Ready);
	Pair<Process*, int> pa(0,0);
	Process* p;
	while (printed.dequeue(pa))
	{
		p=pa.getfirst();
		if (printed.isempty())
			cout << p->getPID();
		else cout << p->getPID() << ", ";
	}
	cout << endl;
	cout << "Time To finish: " << Time << "\tState: " << (OverHeat ? "OverHeated" : "Normal") << endl;
}

Process* EDFProcessor::Steal()  
{
	Pair<Process*, int> pa(0,0);         // create pair 
	Process* P = 0;
	Ready.dequeue(pa);                   // dequeue one from ready
	P = pa.getfirst();                   // make P point to it
	if (P)								   // if P point to any process
	{
		items--;						 // decrement the num of items
		Time -= P->getRemainingTime();   // decreament the time of Processor
	}
	return P;
}

bool EDFProcessor::outofdate()
{
	if (OverHeat)                              // if the processor is overheated
	{
		Overheat_Counter--;					   // decreament the Overheat_Counter of Processor
		if (!Overheat_Counter)				      // check if the Overheat_Period is end
			OverHeat = 0;					   // if true set the processor normal
	}
	else
	{
		int Radix = getRandomNumber();                       // get random number
		if (Radix < 2 && (Run || !(Ready.isempty())))			        // if it is less  and  there is any process in the processor
		{
			OverHeat = 1;									 // if true set the processor Overheated          
			Overheat_Counter = Overheat_Period;				 // set over heated counter to Overheated peroid
			if (Run)										// if there is one in run
			{
				RunToTerm();								 //send it to secheduler to handle it
				Sptr->MovetoWaiting(Run);					   // decreament the time of Processor 
				Time -= Run->getRemainingTime();			 // set run to null
				Run = 0;
			}
			Pair<Process*, int>Pa(0, 0);					//send all processes in ready to  secheduler to handle them if any
			while (Ready.dequeue(Pa))
			{
				Run = Pa.getfirst();                           
				Time -= Run->getRemainingTime();
				Sptr->MovetoWaiting(Run);
				Run = 0;
				items--;
				Pa = { 0, 0 };
			}
			Busy = 0;                      //set the busy to false
		}
	}
	return OverHeat;				   //return the state 
}
