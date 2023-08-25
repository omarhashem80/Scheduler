#include "Processor.h"
#include"Scheduler.h"
#include<random>
Processor::Processor(Scheduler* ptr,int Period) 
{
	Sptr = ptr;
	Busy = 0;
	Run = NULL;
	Time = 0;
	TBusy = 0;
	TIDLE = 0;
	items = 0;
	Type = 0;
	OverHeat = 0;
	Overheat_Counter = 0;
	Overheat_Period = Period;
}
bool Processor::GetBusy()
{
	return Busy;
}
void Processor::SetBusy(bool b)
{
	Busy = b;
}

bool Processor::RunToTerm()
{
	if (Run && Run->getCT() <= Run->getAdvance())
	{
		Sptr->MovetoTerm(Run);
		Run = 0;
		Busy = 0;
		return true;
	}
	return false;
}

bool Processor::RunToBlock()
{
	if (Run && Run->Gotoblock())
	{
		Time -= Run->getRemainingTime();
		Sptr->MovetoBLK(Run);
		Run = 0;
		Busy = 0;
		return true;
	}
	return false;
}

int Processor::GetTime()
{
	
	return Time; 
}
int Processor::GetTBusy()
{
	return TBusy;
}
int Processor::GetTIDLE()
{
	return TIDLE;
}

bool Processor::Is_OverHeat()
{
	return OverHeat;
}

int Processor::Gettype()
{
	return Type;
}

float Processor::Getutil()
{
	return ((TBusy*100) /(float)(TBusy + TIDLE));
}
float Processor::Getload(float trt)
{
	return (trt?(TBusy*100)/ trt:0);
}

Process* Processor::GETRUN()
{
	return Run;
}

int Processor::getRandomNumber()
{

	// Seed the random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution for the random number 
	std::uniform_int_distribution<> dis(1, 100);

	// Generate and return the random number
	return dis(gen);
}
