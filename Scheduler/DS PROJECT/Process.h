#pragma once
#include"Pair.h"
#include"LinkedQueue.h"
class Process
{
private:
	int AT;        //arrival time
	int PID;       //id
	int CT;        //cpu time
	int N;         // # of pairs which request i/o
	int TT;        // time of termination
	int RT;        // response time
	bool Forked;     // for forking
	int Advance;   // # of achieved steps
	int AdvanceIO; //  # of achieved steps of IO request
	LinkedQueue<Pair<int, int>>IOoper;   //queue of pairs which request I/O
	int num_children; //num of children
	Process* firstchild;  //for Fork first child
	Process* secondchild;  //for Fork second child
	int Total_IO;          // counter we use in calculations
	int TIO;               // total IO period requests
	int DeadLine;         // expected deadline
public:
	Process(int at = 0, int pid = 0, int ct = 0, int n = 0,int d=0);  //default constructor
	Process(const Process&);    // copy constructor
	bool iooperin(const Pair<int, int>);   // enqueue IO request in IOoperation list 
	bool Gotoblock();                       // check if it is the time to request IO
	bool GetIOD();                          // check if it is the time to end the IO request
	void incrementadvance();                // increment achieved time steps from cpu time steps
	void increment_AdvanceIO();             // increment achieved time steps from cpu time steps
	void setTT(int);                        // set termination time
	void setRT(int);                        // set response time
	int GetIO()const;                       
	int GetTotal_IO()const;
	int T_IO()const;                        // get total IO period requests 
	int getAT()const;                       // get arrival time
	int getPID()const;                       // get ID
	int getCT()const;                        // get CPU time
	int getN()const;                         // get the number of IO requests
	void setAT(int);                            // get arrival time
	int getDeadline()const;                    // get expected dealine 
	void setDealine(int);                     // set expected dealine 
	Process* getfirstChild()const;            // get the first child 
	void setfirstChild(Process*);              // set the first child 
	Process* getsecondChild()const;			   // get the second child 
	void setsecondChild(Process*);             // set the second child 
	void setPID(int);                          // set the process ID
	void setCT(int);                           // set the CPu time
	void setN(int);                             // set the number of IO requests
	void setForked();                          // mark process as Forked
	int getTT()const;                             // get termination time
	int getcurrentWT(int clock)const;            // get current Waiting time
	int getRT()const;                            // get response time
	int getAdvance()const;                      // get how many time steps does the process achieved
	int getTRT()const;                          // get Turn around time
	int getWT()const;                           // get total waiting time
	int getRemainingTime()const;                // get the remaining time to finish CPU time
	bool isForked()const;                        // check if forked
	bool Can_Fork()const;                       // check for forking if process has less than two processes
	bool operator==(const Process& P);
	bool operator>(const Process& P);
	bool operator<(const Process& P);
	bool operator>=(const Process& P);
	bool operator<=(const Process& P);
	void print();
	~Process();
};


