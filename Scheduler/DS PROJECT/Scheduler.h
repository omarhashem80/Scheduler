#ifndef _SCHEDULER
#define _SCHEDULER
#include"Processor.h"
#include"EDFProcessor.h"
#include"FCFSProcessor.h"
#include"RRProcessor.h"
#include"SJFProcessor.h"                                
#include"LinkedQueue.h"                                 
#include"Process.h"
#include <fstream>
#include"UI.h"
class Processor;               // forward declaration
class Scheduler
{
private:
	Processor** PRL;  // list of processors           array for test in my opinion the array is the best choice WE DON'T NEED DELETE ONLY ARRAY OF POINTERS
	LinkedQueue<Process*>New;//New processes         //PIRORITY QUEUE WITH RESPECT TO TYPE OF PROCESS
	LinkedQueue<Process*>BLK;   //processes requested i/o
	LinkedQueue<Process*>TERM;  //Terminated processes
	int Clock;                  // TO USE IN CALCULATIONS
	fstream file;
	int TPRLsize;
	int PRLsize;
	int FCFSsize;
	int RRsize;
	int SJFsize;
	int EDFsize;
	int RRtime_slice;
	int psize;
	int BLKsize;
	int termsize;
	int RTF;
	int MaxW;
	int STL;
	int Waitingsize;
	int fork_prob;
	int RTFcount;
	int MaxWcount;
	int STLcount;
	int Forkcount;
	int killcount;
	int finish;
	LinkedQueue<Pair<int,int>>TO_KILL;
	LinkedQueue<Process*>Waiting;
	int getmin(int, int);
	int getmax(int, int);
	void KILL(int id);
	int getRandomNumber();
public:
	Scheduler();
	void readinputfile();
	void preprocessing(Process* p);
	int getClock();
	int getPRLsize();
	int getBLKsize();
	int gettermsize();
	int getpsize();
	int getWaitingsize();
	void advance();
	void AdvanceBLK();
	void MovetoTerm(Process*);
	void MovetoBLK(Process*);
	void StealWork();
	bool MigrationRRtoSJF(Process*);
	bool MigrationFCFStoRR(Process*);
	void MovetoWaiting(Process*);
	void AdvanceWaiting();
	void FORK(Process*);
	void Kill_Process();
	void makeoutputfile();
	~Scheduler();
};
#endif