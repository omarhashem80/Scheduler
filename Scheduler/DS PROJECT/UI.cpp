#include "UI.h"
#include<iostream>
#include<windows.h>
using namespace std;
int UI::mode = 0;
void UI::updatainterface(Scheduler* sptr, Processor** PRL, LinkedQueue<Process*>TERM, LinkedQueue<Process*>BLK, LinkedQueue<Process*>Waiting)
{
	if (!mode)
	{
		cout << "Please Enter the operating mode: ";
		cin >> mode;
		cout << "\n";
	}
	if (mode == 1 || mode == 2) {
		cout << "Current Timestep: " << sptr->getClock() << endl;
		cout << "\n--------------------------    RDY processes    --------------------------\n";
		for (int i = 0; i < sptr->getPRLsize(); i++)
		{
			cout << "Processor " << i + 1 << " ";
			PRL[i]->print();
		}
		cout << "\n--------------------------    BLK processes    --------------------------\n";
		cout << sptr->getBLKsize() << " BLK: ";
		for (int i = 0; i < sptr->getBLKsize(); i++)
		{
			if (i == sptr->getBLKsize() - 1)
			{
				Process* p;
				BLK.dequeue(p);
				cout << p->getPID();
				BLK.enqueue(p);
				continue;
			}
			Process* p;
			BLK.dequeue(p);
			cout << p->getPID() << ", ";
			BLK.enqueue(p);
		}
		cout << "\n--------------------------    WAITING processes    --------------------------\n";
		cout << sptr->getWaitingsize() << " Waiting: ";
		for (int i = 0; i < sptr->getWaitingsize(); i++)
		{
			if (i == sptr->getWaitingsize() - 1)
			{
				Process* p;
				Waiting.dequeue(p);
				cout << p->getPID();
				Waiting.enqueue(p);
				continue;
			}
			Process* p;
			Waiting.dequeue(p);
			cout << p->getPID() << ", ";
			Waiting.enqueue(p);
		}
		cout << "\n--------------------------    RUN processes    --------------------------\n";
		int sum = 0;
		for (int i = 0; i < sptr->getPRLsize(); i++)
			sum += PRL[i]->GetBusy();
		cout << sum << " RUN: ";
		for (int i = 0; i < sptr->getPRLsize(); i++)
		{
			if (i == sptr->getPRLsize() - 1)
			{
				if (PRL[i]->GETRUN())
					cout << PRL[i]->GETRUN()->getPID() << " " << "(P" << i + 1 << ")";
				continue;
			}
			if (PRL[i]->GETRUN())
				cout << PRL[i]->GETRUN()->getPID() << " " << "(P" << i + 1 << ")" << " , ";
		}
		cout << "\n--------------------------    TERM processes    --------------------------\n";
		cout << sptr->gettermsize() << " TRM: ";
		for (int i = 0; i < sptr->gettermsize(); i++)
		{
			if (i == sptr->gettermsize() - 1)
			{
				Process* p;
				TERM.dequeue(p);
				cout << p->getPID();
				TERM.enqueue(p);
				continue;
			}
			Process* p = 0;
			TERM.dequeue(p);
			cout << p->getPID() << " ,";
			TERM.enqueue(p);
		}
		cout << endl;
		cout <<"num of processes: "<<sptr->getpsize() << endl;
		if (mode == 1)
			 system("pause");
		else
			Sleep(1000);
	}
	else
	{
		if (sptr->getClock() == 1)
			cout << "Silent Mode..........   Simulation Starts......\n";
			if (sptr->gettermsize() == sptr->getpsize())
				cout << "Simulation ends, Output file created\n";
	}
}



