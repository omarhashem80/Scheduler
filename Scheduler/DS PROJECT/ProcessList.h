#ifndef _PROCESSLIST
#define _PROCESSLIST
#include"Process.h"
#include"LinkedList.h"
class ProcessList:virtual public LinkedList<Process*>
{
private:
	Node<Process*>* Head;
public:
	ProcessList()
	{
		Head = 0;
	}
	ProcessList(ProcessList& P)
	{
		Head = 0;
		if (P.Head)
		{
			Node<Process*>* temp = P.Head;
			while (temp)
			{
				InsertEnd(temp->getItem());
				temp = temp->getNext();
			}
		}
	}
	~ProcessList()
	{
		DeleteAll();
	}
	void InsertEnd(Process* data)
	{
		Node<Process* >* new_node = new Node<Process*>(data);
		new_node->setNext(0);
		Node<Process*>* P = Head;
		if (!P)
			Head = new_node;
		else
		{
			while (P->getNext())
				P = P->getNext();
			P->setNext(new_node);
		}
	}
	void InsertBeg(Process*& data)
	{
		Node<Process*>* R = new Node<Process*>(data);
		R->setNext(Head);
		Head = R;

	}
	bool Find(int Key, Process*& Temp)
	{
		Temp = 0;
		Node<Process*>* P = Head;
		while (P)
		{
			if (P->getItem()->getPID() == Key)
			{
				Temp = P->getItem();
				return true;
			}
			P = P->getNext();
		}
		return false;
	}
	/*bool Find(int index, Process*& Temp)
	{
		Temp = 0;
		Node<Process*>* P = Head;
		while (P&&index--)
		{
			P = P->getNext();
		}
		if (P)
		{
			Temp = P->getItem();
			return true;
		}
		return false;
	}*/
	bool isEmpty()
	{
		return !Head;
	}
	bool DeleteFirst(Process*&temp)
	{
		temp = 0;
		if (!Head)
			return false;
		Node<Process*>* P = Head->getNext();
		temp = Head->getItem();
		delete Head;
		if (P)
			Head = P;
		else
			Head = 0;
		return true;
	}
	bool DeleteNode( Process* value)
	{
		if (!Head)
			return false;
		bool found = false;
		Node<Process*>* P = Head;
		Node<Process*>* N = P->getNext();
		if (P->getItem()->getPID() == value->getPID())
		{
			Head = Head->getNext();
			delete P;
			return true;
		}
		while (N)
		{
			if (N->getItem() == value)
			{
				found = true;
				break;
			}
			P = N;
			N = N->getNext();
		}
		if (found)
		{
			P->setNext(N->getNext());
			N->setNext(0);
			delete N;
			N = 0;
		}
		return found;
	}

};
#endif	




