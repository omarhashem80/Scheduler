#include "FCFSProcessor.h"
#include"Scheduler.h"
#include<iostream>
using namespace std;
FCFSProcessor::FCFSProcessor(Scheduler*ptr,int M, int Period):Processor(ptr,Period)
{
	MaxW = M;
}
void FCFSProcessor::SendToReady(Process* ptr)
{
	if (ptr)                                      
	{
		Time += ptr->getRemainingTime();           // increase the time of processor
		Ready.InsertEnd(ptr);                      // put the process in ready list
		items++;                                  // increase the num of processes in ready
	}
}

void FCFSProcessor::ReadyToRun(int Clock)
{
	if (!(Ready.isEmpty())&&!Run)
	{
		Run = 0;
		Ready.DeleteFirst(Run);
		Busy = 1;
		items--;
		Run->setRT(Clock);             // set response time
		Migrate(Clock);         // check migrate before run
		Busy = (Run ? 1 : 0);
	}
}


void FCFSProcessor::schedule(int Clock)
{
	outofdate();
	if (!(Ready.isEmpty()) || Run)         // check if it has any process or not
	{
		if (!Busy)                         // check if IDLE or not
		{
			Run = 0;
			Ready.DeleteFirst(Run);        // pick one from ready
			items--;                       // decrease the num of processes in ready 
			Run->setRT(Clock);             // set response time
			Migrate(Clock);                // check migrate before run
			Busy = (Run ? 1 : 0);
		}
		if (Run)
		{
			Run->incrementadvance();           // increase the advance of process
			Time--;                           // decrease the time for the processor to finish
			RunToBlock();                       // check if there is io request
			RunToTerm();                        // check if the process is finish
			Fork();                             // check forking
		}
	}
	else
	{
		Busy = 0;
		Time = 0;
	}
	TBusy += Busy;                        // increase total busy time
	TIDLE += !Busy;                       // increase total ideal time
}


Process*FCFSProcessor::Steal()
{
	Process* P=0,*N = 0;        // Steal with Recursion to take the unforked Process if any
	if (Ready.DeleteFirst(P))    // remove one from ready 
	{
		if (!(P->isForked()))                   //check if forked
		{
			items--;                               // decrement the num of items
			Time -= P->getRemainingTime();  // decreament the time of Processor
			return P;   //return it to Scheduler
		}
		else
		{
			N=Steal();   // recursion on Ready list
			Ready.InsertBeg(P);   // if it forked return it to ready list
		}
	}
	return N;    // return thr recusrion one
}

Process* FCFSProcessor::kill(int id)
{
	Process* P = 0;
	if (Run&&Run->getPID()==id)  // if the killed process in the run
	{
		Time -= Run->getRemainingTime(); // decreament the time of Processor
		P = Run;     //make p point to it
		Run = 0;   // make run point t null
		Busy = 0;    //set the processor to IDEL
		return P;    // return P to scheduler
	}
	else
	{
		if(Ready.Find(id, P))   // search for the killed Process
		{
		    Ready.DeleteNode(P);   // if found remove it from ready list
			items--;                // decrement the num of items
			Time -= P->getRemainingTime();  // decreament the time of Processor
		}
		return P;
	}
}

void FCFSProcessor::Fork()
{
	if (Run&&Run->Can_Fork())   //check here or there
		Sptr->FORK(Run);       // send ot to Scheduler
}

void FCFSProcessor::print()
{
	cout << "[FCFS]: " << items << ": " << "RDY: ";
	ProcessList printed = Ready;
	Process* p=0;
	while (printed.DeleteFirst(p))
		if (printed.isEmpty())
			cout << p->getPID();
		else cout << p->getPID() << ", ";
	cout << endl;
	cout << "Time To finish: " << Time <<"\tState: "<<(OverHeat?"OverHeated":"Normal") << endl;
}

bool FCFSProcessor::Migrate(int Clock)
{
	bool test = 0;     // bool to return it
    while(Run&&Run->getcurrentWT(Clock)>MaxW&&!(Run->isForked()))   // while run processes have waiting time more than MAxW and not waiting 
	{
		if (Sptr->MigrationFCFStoRR(Run))    // check if it Migrated
		{
			Time -= Run->getRemainingTime();  
			Run = 0;
			Busy = 0;
			if (Ready.DeleteFirst(Run))
			{ 
				Busy = 1;        //set the processor to Busy
				items--;      // decrement the num of items
			}
			test = 1;          // if migration is done
		}
		else
			break;
	}
	return test;     
}

bool FCFSProcessor::outofdate()
{
	if (OverHeat)                             // if the processor is overheated
	{ 
		Overheat_Counter--;					           // decreament the Overheat_Counter of Processor
		if (!Overheat_Counter)				           // check if the Overheat_Period is end
			OverHeat = 0;					           // if true set the processor normal
	}
	else
	{
		int Radix = getRandomNumber();                                 // get random number
		if (Radix < 2 && (Run || !(Ready.isEmpty())))				      // if it is less  and  there is any process in the processor
		{
			OverHeat = 1;											   // if true set the processor Overheated          
			Overheat_Counter = Overheat_Period;						     // set over heated counter to Overheated peroid
			if (Run)                                                     // if there is one in run
				items++;
			while (Run || Ready.DeleteFirst(Run))					     //send it to secheduler to handle it
			{														       // decreament the time of Processor 
				Time -= Run->getRemainingTime();					   // set run to null
				Sptr->MovetoWaiting(Run);
				Run = 0;
				items--;											 //  send all processes in ready to  secheduler to handle them if any
			}
			Busy = 0;                  //set the busy to false
		}
	}
	return OverHeat;			       //return the state 
}



