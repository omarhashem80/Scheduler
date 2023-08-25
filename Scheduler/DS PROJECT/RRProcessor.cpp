#include "RRProcessor.h"
#include"Scheduler.h"
#include<iostream>
using namespace std;
RRProcessor::RRProcessor(int Time,Scheduler* ptr,int R, int Period) :Processor(ptr,Period)
{
	Slicetime = Time;
	count = 0;
	RTF = R;
}

void RRProcessor::SendToReady(Process* ptr)        //take process and put into ready list and increase the time load;
{
	if (ptr)                                        
	{
		Time += ptr->getRemainingTime();                // increase the time of processor
		Ready.enqueue(ptr);							    // put the process in ready list
		items++;									   // increase the num of processes in ready
	}
}

bool RRProcessor::RunToReady()  
{
	if (Run&&count==Slicetime)
	{
		Ready.enqueue(Run);
		Run = 0;
		items++;
		Busy = 0;
		count = 0;
		return true;
	}
	return false;
}

void RRProcessor::ReadyToRun(int Clock)
{
	if (!(Ready.isEmpty())&& !Run)       //check if there is any process in ready and there is not in rum
	{
		Run = 0;
		Ready.dequeue(Run);    // take one from ready
		items--;                //decrement the ready list size 
		Run->setRT(Clock);         //set response time of process
		Migrate(Clock);			 //check Migrate
	}
}


void RRProcessor::schedule(int Clock)
{
	outofdate();
	if (!(Ready.isEmpty()) || Run)
	{
		if (!Busy)                       // check if the processor is IDEL
		{								 
			Run = 0;                   	 // take one from ready
			Ready.dequeue(Run);			 // put it in run state  
			items--;					  //decrement the ready list size 
			Run->setRT(Clock);			    //set response time of process
			Migrate(Clock);				  //check Migrate
			Busy = (Run ? 1 : 0);		 // put the processor in busy state
			count = 0;                    // set count to zero
		}
		if (Run && !RunToReady())          // if there is a process in the run state
		{
			Run->incrementadvance();		    // increment its time step    
			count++;                               // increment count to check RR_SLICE 
			Time--;							        //decrement the time of the processor  
			RunToBlock();					    // handling if it request io  
			RunToTerm();                          // check if the process is finish  
		}									   
	}
	else
	{
		Busy = 0;                       // if not set to process to IDEL state
		Time = 0;
	}
	TBusy += Busy;						      // increment the total busy time
	TIDLE += !Busy;						      // increment the total IDEL time
}

void RRProcessor::print()
{
	cout << "[RR]: " << items << ": " << "RDY: ";
	LinkedQueue<Process*>printed = Ready;
	Process* p;
	while (printed.dequeue(p))
		if (printed.isEmpty())
			cout << p->getPID();
		else cout << p->getPID() << ", ";
	cout << endl;
	cout << "Time To finish: " << Time << "\tState: " << (OverHeat ? "OverHeated" : "Normal") << endl;
}

Process* RRProcessor::Steal()
{
	Process* P = 0;                       // Steal with Recursion to take the unforked Process if any
	Ready.dequeue(P);					   // remove one from ready 
	if (P)                     
	{
		items--;                             // increase the advance of process
		Time -= P->getRemainingTime();		// decrease the time for the processor to finish
	}
	return P;
}

bool RRProcessor::Migrate(int Clock)
{
	bool test = false;  //
	while (Run&&Run->getRemainingTime()< RTF)  // while run processes have  time more than RTF 
	{
		test = 1;
		if (Sptr->MigrationRRtoSJF(Run))        // check if it Migrated
		{
			Time -= Run->getRemainingTime();  // decreament the time of Processor
			Run = 0;
			Busy = 0;
			ReadyToRun(Clock);       // take one from ready to run and check migration condition again
			test = 1;
		}
		else                         // if not break
			break;
	}
	return test;//
}

bool RRProcessor::outofdate()
{
	if (OverHeat)
	{
		Overheat_Counter--;
		if (!Overheat_Counter)
			OverHeat = 0;
	}
	else
	{
		int Radix = getRandomNumber();
		if (Radix < 2 && (Run || !(Ready.isEmpty())))
		{
			OverHeat = 1;
			Overheat_Counter = Overheat_Period;
			if (Run)
				items++;
			while (Run || Ready.dequeue(Run))
			{
				Time -= Run->getRemainingTime();
				Sptr->MovetoWaiting(Run);
				Run = 0;
				items--;
			}
			Busy = 0;
		}
	}
		return OverHeat;
}



