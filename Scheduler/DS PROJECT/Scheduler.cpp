#include "Scheduler.h"
#include<iostream>
#include <random>
#include<iomanip>
Scheduler::Scheduler()  
{
	BLKsize = Clock = RRtime_slice = BLKsize = 0;
	psize = termsize = PRLsize = RTF = MaxW =TPRLsize = fork_prob = 0;
	STL = RRsize = SJFsize = FCFSsize =Waitingsize=finish= 0;
	PRL = 0;
	RTFcount = MaxWcount = STLcount = Forkcount = EDFsize= killcount = 0;
}
void Scheduler::readinputfile()
{
	int readnum, itmax, itmin; //itmax is the number of all processes  , itmin is the number of time the process requests I\O
	char tempchar;
	int Period;
	file.open("input file.txt", ios::in | ios::out);
	if (file.is_open())
	{
		PRLsize = 0;
		file >> readnum;
		PRLsize += readnum;
		FCFSsize = readnum;
		file >> readnum;
		PRLsize += readnum;
		SJFsize = readnum;
		file >> readnum;
		PRLsize += readnum;
		RRsize = readnum;
		file >> readnum;  
		PRLsize += readnum;
		EDFsize = readnum;
		file >> readnum;
		RRtime_slice = readnum;
		file >> readnum;
		Period = readnum;
		file >> readnum;
		RTF = readnum;
		file >> readnum;
		MaxW = readnum;
		file >> readnum;
		STL = readnum;
		file >> readnum;
		fork_prob = readnum;
		file >> itmax;
		psize = itmax;
		termsize = 0;
		PRL = new Processor * [PRLsize];
		for (int i = 0; i < FCFSsize; i++)
		{
			FCFSProcessor* P = new FCFSProcessor(this,MaxW, Period);
			PRL[i] = P;
		}
		for (int i = FCFSsize; i < SJFsize + FCFSsize; i++)
		{
			SJFProcessor* P = new SJFProcessor(this, Period);
			PRL[i] = P;
		}
		for (int i = SJFsize + FCFSsize; i < PRLsize; i++)
		{
			RRProcessor* P = new RRProcessor(RRtime_slice, this,RTF, Period);
			PRL[i] = P;
		}
		for (int i = SJFsize + FCFSsize + RRsize; i < PRLsize; i++)  
		{
			EDFProcessor* P = new EDFProcessor(this, Period);
			PRL[i] = P;
		}
		for (int i = 0; i < itmax; i++)
		{
			Process* processarr = new Process;
			file >> readnum;
			processarr->setAT(readnum);
			file >> readnum;
			processarr->setPID(readnum);
			file >> readnum;
			processarr->setCT(readnum);
			file >> itmin;
			processarr->setN(itmin);
			file >> readnum;
			processarr->setDealine(readnum);
			if (itmin)
			{
				int x, y;
				for (int j = 0; j < itmin - 1; j++)
				{
					file >> tempchar;
					file >> readnum;
					x = readnum;
					file >> tempchar;
					file >> readnum;
					y = readnum;
					file >> tempchar;
					file >> tempchar;
					Pair<int, int>arrforio(x, y);
					processarr->iooperin(arrforio);
				}
				file >> tempchar;
				file >> readnum;
				x = readnum;
				file >> tempchar;
				file >> readnum;
				y = readnum;
				file >> tempchar;
				Pair<int, int>arrforioe(x, y);
				processarr->iooperin(arrforioe);
			}
			New.enqueue(processarr);
		}
		int tempnum2;
		while (file >> readnum)
		{
			file >> tempnum2;
			TO_KILL.enqueue({ readnum,tempnum2 });
		}
		file.close();
	}
	TPRLsize = PRLsize;
}


void Scheduler::preprocessing(Process* p = 0) // handling processes "two ways" and send it to shortest processor 
{
	if (p)                          //check if P point to any process
	{
		int i = getmin(0, PRLsize);  //check if there is anyprocessor not overheated
		if (i != -1)                     // if any processor isnot overheated
			PRL[i]->SendToReady(p);  // send the process to the shortest ready queue
		else
			MovetoWaiting(p);      //if all processors are overheated send it to Waiting list  
	}
	while (New.peek(p) && p->getAT() == Clock)    // loop on new list check if any process arrives to AT 
	{
		New.dequeue(p);                // remove it from new list
		int i = getmin(0, PRLsize);   //check if there is anyprocessor not overheated
		if (i != -1)                   // if any processor isnot overheated
			PRL[i]->SendToReady(p);   // send the process to the shortest ready queue
		else
			MovetoWaiting(p);      //if all processors are overheated send it to Waiting list
	}
}
void Scheduler::advance()
{
	readinputfile();   // read the data from inputfile
	while (termsize < psize)   // check if all processes finish or not
	{
		Clock++;            // increment the clock
		preprocessing();     // send the processes from (new or block or forked process)  to the shortest processor
		for (int i = 0; i < PRLsize; i++)   // loop on all processors and do them schedule
			PRL[i]->schedule(Clock);    // send the clock to use in scheduling
		AdvanceWaiting();      // waiting list Scheduling
		StealWork();           // steal Scheduling
		Kill_Process();       //  Kill Scheduling
		AdvanceBLK();		  // Block list Scheduling
		UI::updatainterface(this, PRL, TERM, BLK,Waiting);  // User interface 
	}
}
void Scheduler::AdvanceBLK()
{
	Process* P = 0;
	if (!BLK.isEmpty())                //check the BLock list 
	{
		BLK.peek(P);                   // if not empty peek the first
		if(!(P->GetIOD()))            // check if itdoes not finish the io request
		   P->increment_AdvanceIO();           // increment the IO request
		else
		{
			BLK.dequeue(P);                 // if it finishs the io request
			if (P->getAdvance() < P->getCT())   // if the process does not finish
				preprocessing(P);             // send it to the shortest Ready if any processors in Normal state
			else
				MovetoTerm(P);               // if the process finishs move it to term list
			BLKsize--;                      // decrement the BLock size
		}
	}

}

void Scheduler::MovetoTerm(Process* p)
{
	if (p)
	{
		p->setTT(Clock);                              //set TT time
		termsize++;                                    // decrement the BLock size
		finish += (p->getTT() < p->getDeadline());      // count the process finish before deadline
		TERM.enqueue(p);                                // add it to term list
		if (p->getfirstChild())                         // check if it has anychild and kill it
			KILL(p->getfirstChild()->getPID());          
		if (p->getsecondChild())
			KILL(p->getsecondChild()->getPID());

	}
}

void Scheduler::MovetoBLK(Process* p)
{
	if (p)
	{
		BLKsize++;             // decrement the BLock size
		BLK.enqueue(p);          // enqueue it to Block list
	}
}

void Scheduler::StealWork()
{
	 int a, b, c, d,s=0;     // declare integers to use in calculations
	 a = getmax(0, PRLsize);    // get the index the Longest processor
	 b = getmin(0, PRLsize);    // get the index  the shortest processor
	 if (a != -1 && b != -1 && a != b)   // check if there is Two distinct processors to steal from one to anothor 
	 {
		 c = PRL[a]->GetTime();         // get the time of the Longest processor
		 d = PRL[b]->GetTime();			// get the time of the shortest processor
		 s = (c - d) * 100;             // s is the ratio which we use to check balance
		 if (c)                         // check if c equal zero
			 s /= c;                    // get the ratio
		 while (STL && !(Clock % STL) && s > 40)       // loop while the condition is two if stl time is now and the ratio is more than 40
		 {
			 Process* P = PRL[a]->Steal();         // make p point to a process 
			 if (P)                                 // if it point to a process 
			 {
				 PRL[b]->SendToReady(P);       // send it to the shortest ready queue
				 c = PRL[a]->GetTime();       // get the time of the Longest processor  
				 d = PRL[b]->GetTime();		  // get the time of the shortest processor
				 s = (c - d) * 100;			  // s is the ratio which we use to check balance
				 if (c)						  // check if c equal zero
					 s /= c;				  // get the ratio
				 STLcount++;                  // increment counter for statistics 
			 }
			 else
				 s = 0;       // if P does not point to make s equal zero to break the loop 
		 }
	 }
}

bool Scheduler::MigrationRRtoSJF(Process*P)
{
	if (P&&SJFsize)                                     // check if there is SJF processors
	{ 
		int i = getmin(FCFSsize, SJFsize + FCFSsize);      // get the shortest SJF  processor
		if (i != -1)                        // if any send the process to it
		{
			PRL[i]->SendToReady(P);
			RTFcount++;                         // increment counter for statistics 
			return true;
		}
	}
	return false;
}
bool Scheduler::MigrationFCFStoRR(Process* P)
{
	if (P&&RRsize)                                       // check if there is RR processors
	{
		int i = getmin(SJFsize+FCFSsize, SJFsize + FCFSsize+RRsize);        // get the shortest RR processor
		if (i != -1)       // if any send the process to it
		{
			PRL[i]->SendToReady(P);      
			MaxWcount++;           // increment counter for statistics 
			return true;
		}
	}  
	return false;
}
void Scheduler::MovetoWaiting(Process*P)
{
	if (P)
	{
		Waiting.enqueue(P);    // add it to Waiting list
		Waitingsize++;     
	}
}
void Scheduler::AdvanceWaiting()
{
	if (Waiting.isEmpty())                // if waiting list is empty return
		return;
	Process* P = 0;
	if (!(Waiting.isEmpty()))              // if not
	{
		int k = getmin(0, PRLsize);       // get the shortest processor
		if (k != -1)                      // if any
		{
			Waiting.dequeue(P);            //remove one process from waiting 
			Waitingsize--;               
			if (P->isForked())             // if this process forked 
			{ 
				int i = getmin(0, FCFSsize);         // get the shortest FCFS processor
				if (i != -1)                       // if any
				{
					PRL[i]->SendToReady(P);          //send it to the shortest FCFS processor
					P = 0;
				}
			}
			else                     // if not forked
			{
				PRL[k]->SendToReady(P);             //send it to the shortest  processor     
				P = 0;
			}
			AdvanceWaiting();       // call itself again
		}
	}
	if (P)                           // if there is not Processor in Normal state
		MovetoWaiting(P);             // enqueue it to waiting list
}

void Scheduler::FORK(Process*P)
{
	int Radix = getRandomNumber();   //random number
	if (Radix < fork_prob)           // check if it less than fork_prob
	{
		if (P&&P->Can_Fork())        //check if the Process has less than two chidren or not
		{
			Process* child = new Process(Clock, ++psize, P->getRemainingTime(), 0,2*(P->getDeadline()));  //create a new process (child)
			child->setForked();                                          // mark it as a forked process
			if(!(P->getfirstChild()))                                    // if this is the first child put it in first child pointer
			   P->setfirstChild(child);
			else
				P->setsecondChild(child);                    // if this isnot the first child put it in second child pointer
			PRL[getmin(0, FCFSsize)]->SendToReady(child);    // send it to the shortest ready queue of FCFS Processors
			Forkcount++;                                   // increment the counter for statistics
		}
	}
}
void Scheduler::Kill_Process()
{
	Pair<int, int>P;    //Pair to fill
	while (TO_KILL.peek(P) && P.getfirst() == Clock)  //loop on To kill list if find anyprocess arrives to Kill time   
	{
		TO_KILL.dequeue(P);   // remove from the list
		KILL(P.getsecond());  // kill it
	}
}

void Scheduler::KILL(int id)
{
	Process* P = 0;                                      //process pointer to fill
	for (int i = 0; i < FCFSsize&&!P; i++)               //loop on FCFS Processors break if find it 
	{
		P = dynamic_cast<FCFSProcessor*>(PRL[i])->kill(id);  //search for it to kill it
	}
	if (P)  // if we found it 
	{
		Process* child = 0;           // check if it has anychild
		child = P->getfirstChild();
		MovetoTerm(P);                // move it to termlist
		killcount++;                  // increment the counter for statistics 
		if (child)
			KILL(child->getPID());    // kill it and children of it if any
		child = 0;
		child = P->getsecondChild();  // check if it has anychild
		if (child)
			KILL(child->getPID());   // kill it and children of it if any
	}
}

int Scheduler::getRandomNumber()
{
	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution for the random number 
	std::uniform_int_distribution<> dis(1, 100);

	// Generate and return the random number
	return dis(gen);
}

int Scheduler::getmin(int k, int size)       // get the shortest readylist Processor
{
	int i = k++;                                   //set i to the first element and start the loop from the next element
	while (i < size && PRL[i]->Is_OverHeat())  	       // get the first Normal processor
		i++;
	for (k; k < size&&i<size; k++)                      
	{
		if (!(PRL[k]->Is_OverHeat()) && PRL[i]->GetTime() > PRL[k]->GetTime())   // check if there is ready list normal Processor  shorter than current set it to i 
			i = k;
	}
	return (i>size-1?-1:i);                            // if it failed to find any processor in this range return -1 if done return it
}
int Scheduler::getmax(int k, int size)       // get the longest readylist Processor
{
	int i = k++;                              //set i to the first element and start the loop from the next element
	while (i < size && PRL[i]->Is_OverHeat())     // get the first Normal processor
		i++;
	for (k; k < size && i < size; k++)
	{
		if (!(PRL[k]->Is_OverHeat()) && PRL[i]->GetTime() < PRL[k]->GetTime())  // check if there is ready list normal Processor longer than current set it to i 
			i = k;
	}
	return (i > size-1 ? -1 : i);   // if it failed to find any processor in this range return -1 if done return it
}
int Scheduler::getClock()
{
	return Clock;
}

int Scheduler::getPRLsize()
{
	return PRLsize;
}

int Scheduler::getBLKsize()
{
	return BLKsize;
}

int Scheduler::gettermsize()
{
	return termsize;
}
int Scheduler::getpsize()
{
	return psize;
}
int Scheduler::getWaitingsize()
{
	return Waitingsize;
}
void Scheduler::makeoutputfile()
{
	file.open("OutPut_File.txt", ios::out);
	if (file.is_open())
	{
		LinkedQueue<Process*> temp;
		Process* p = 0;
		float avgutiliz = 0;
		float swt = 0, srt = 0, strt = 0;
		file << "   TT       PID      AT       CT       IO_D      WT        RT       TRT      DL\n";
		while (TERM.dequeue(p))
		{
			file << setw(4) << p->getTT() << "     " << setw(4) << p->getPID() << "     " << setw(4) << p->getAT() << "     " << setw(4) << p->getCT() << "     " << setw(4) << p->T_IO() << "      " << setw(4) << p->getWT() << "      " << setw(4) << p->getRT() << "     " << setw(4) << p->getTRT() << "     " << setw(4) << p->getDeadline() << "\n";
			temp.enqueue(p);
		}
		while (temp.dequeue(p))
			TERM.enqueue(p);
		file << "Processes :" << getpsize() << "\n";
		while (TERM.dequeue(p))
		{
			swt += p->getWT();
			srt += p->getRT();
			strt += p->getTRT();
			temp.enqueue(p);
		}
		while (temp.dequeue(p))
			TERM.enqueue(p);
		file << "Avg WT = " << swt / psize << ",      " << "Avg RT = " << srt / psize << ",      " << "Avg TRT = " << strt / psize << "\n";
		file << "Megration % = " << ((RTFcount + MaxWcount) * 100) / (float)psize << "       " << "RTF = " << RTFcount * 100 / (float)psize << " %        " << "MaxW = " << MaxWcount * 100 / (float)psize << " %\n";
		file << "Work Steal % = " << (STLcount * 100) / (float)psize << "%\n";
		file << "Forked Process :" << (Forkcount * 100) / (float)psize << "%\n";
		file << "Killed Process :" << (killcount * 100) / (float)psize << "%\n\n";
		file << "Processors :" << TPRLsize << " [ " << FCFSsize << " FCSF " << SJFsize << " SJF " << RRsize << " RR " << EDFsize << " EDF] \n";
		file << "Processors Load\n";
		for (int i = 0; i < TPRLsize - 1; i++)
			file << "P" << i + 1 << " = " << PRL[i]->Getload(strt) << " %,      ";
		file << "P" << TPRLsize << " = " << PRL[TPRLsize - 1]->Getload(strt) << " % \n\n";
		file << "Processors Utiliz\n";
		for (int i = 0; i < TPRLsize - 1; i++)
		{
			file << "P" << i + 1 << " = " << PRL[i]->Getutil() << " %,      ";
			avgutiliz += PRL[i]->Getutil();
		}
		file << "P" << TPRLsize << " = " << PRL[TPRLsize - 1]->Getutil() << " %\n";
		avgutiliz += PRL[TPRLsize - 1]->Getutil();
		file << "Avg utilization = " << avgutiliz / TPRLsize << " % \n";
		file << "EDF persentage = " << (finish * 100.0) / psize << " % \n";
		file.close();
	}
}
Scheduler::~Scheduler()
{
	makeoutputfile();
	Process* P = 0;
	while (TERM.dequeue(P))
		delete P;
	for (int i = 0; i < PRLsize; i++)
		delete PRL[i];
}
