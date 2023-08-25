#include "Process.h"
#include<iostream>
using namespace std;
Process::Process(int at, int pid, int ct, int n,int d)
{
	AT = at;
	PID = pid;
	CT = ct;
	N = n;
	TT = 0;
	RT = 0;
	Advance = 0;
	AdvanceIO = 0;
	Forked = 0;
	Total_IO = 0;
	firstchild=secondchild = 0;
	TIO = 0;
	num_children = 0;
	DeadLine = d;
}
Process::Process(const Process& p)
{
	AT = p.AT;
	PID = p.PID;
	CT = p.CT;
	N = p.N;
	TT = p.TT;
	RT = p.RT;
	Forked = p.Forked;
	Advance = p.Advance;
	AdvanceIO = p.AdvanceIO;
	IOoper = p.IOoper;
	Total_IO = p.Total_IO;
}
bool Process::iooperin(const Pair<int, int> p)   //enqueue for IO REQUESTS
{
	Total_IO += p.getsecond();
	TIO = Total_IO;
	return IOoper.enqueue(p);
}



bool Process::Gotoblock()
{
	Pair<int, int>p;
	IOoper.peek(p);
	if (Advance == p.getfirst())
		return true;
	return false;
}
bool Process::GetIOD()
{
	Pair<int, int>p;
	IOoper.peek(p);
	if (AdvanceIO == p.getsecond())
	{
		IOoper.dequeue(p);
		IOoper.enqueue(p);
		AdvanceIO = 0;
		return true;
	}
	return false;
}
int Process::GetIO()const
{
	Pair<int, int>p;
	IOoper.peek(p);
	return p.getsecond();
}
int Process::GetTotal_IO() const
{
	return Total_IO;
}
int Process::T_IO() const
{
	return TIO;
}
void Process::incrementadvance()
{
	Advance++;
}
void Process::increment_AdvanceIO()
{
	Total_IO--;
	AdvanceIO++;
}
void Process::setTT(int t)
{
	TT = t;
}

void Process::setRT(int t)
{
	if (!RT)
		RT = t;
}

void Process::setAT(int a)
{
	AT = a;
}
int Process::getDeadline() const
{
	return DeadLine;
}
void Process::setDealine(int D)
{
	DeadLine = D;
}
Process* Process::getfirstChild() const
{
	return firstchild;
}
void Process::setfirstChild(Process* P)
{
	num_children++;
	firstchild = P;
}
Process* Process::getsecondChild() const
{
	return secondchild;
}
void Process::setsecondChild(Process* P)
{
	num_children++;
	secondchild = P;
}
void Process::setPID(int b)
{
	PID = b;
}
void Process::setCT(int c)
{
	CT = c;
}
void Process::setN(int d)
{
	N = d;
}

void Process::setForked()
{
	Forked = 1;
}

int Process::getAT()const
{
	return AT;
}

int Process::getPID()const
{
	return PID;
}

int Process::getCT()const
{
	return CT;
}

int Process::getN()const
{
	return N;
}

int Process::getTT()const
{
	return TT;
}

int Process::getcurrentWT(int clock) const
{
	return clock - AT - Advance;
}

int Process::getRT()const
{
	return RT;
}

int Process::getAdvance()const
{
	return Advance;
}

int Process::getTRT()const
{
	return TT - AT+1;
}

int Process::getWT()const
{
	int w = getTRT() - CT-TIO;
	if (Advance < CT)
		w = TT - AT+Advance;
	return w;
}

int Process::getRemainingTime() const
{
	return getCT()-getAdvance();
}

bool Process::isForked()const
{
	return Forked;
}
bool Process::Can_Fork() const
{
	return num_children<2;
}
bool Process::operator==(const Process& P)
{
	return getRemainingTime() == P.getRemainingTime();
}
bool Process::operator>(const Process& P)
{
	return getRemainingTime() > P.getRemainingTime();
}
bool Process::operator<(const Process& P)
{
	return getRemainingTime() < P.getRemainingTime();
}

bool Process::operator>=(const Process& P)
{
	return *this > P && *this == P;
}

bool Process::operator<=(const Process& P)
{
	return *this < P && *this == P;
}
void Process::print()
{
	cout << TT << "  " << PID << "  " << AT << "  " << CT;
	for (int i = 0; i < N; i++)
	{
		Pair<int, int> temp;
		IOoper.dequeue(temp);
		cout << "  " << temp.getsecond();
		IOoper.enqueue(temp);
	}
	cout << "  " << getWT() << "  " << RT << "  " << getTRT() << endl;
}
Process::~Process()
{
	
}