#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"
#include<iostream>
using namespace std;
template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	//You can add tail pointer too (depending on your problem)
public:

	LinkedList()
	{
		Head = nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	void PrintList()    const
	{

		Node<T>* p = Head;

		while (p)
		{
			cout << p->getItem() << "->";
			p = p->getNext();
		}
		cout << "NULL\n";
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : The value to be stored in the new node.
	*/
	void InsertBeg(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/
	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		Head = 0;
	}


	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		Node<T>* new_node = new Node<T>(data);
		new_node->setNext(0);
		Node<T>* P = Head;
		if (!P)
			Head = new_node;
		else
		{
			while (P->getNext())
				P = P->getNext();
			P->setNext(new_node);
		}
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	bool Find(int Key)
	{
		Node<T>* P = Head;
		while (P)
		{
			if (P->getItem() == Key)
				return true;
			P = P->getNext();
		}
		return false;
	}

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	int CountOccurance(const T& value)
	{
		int count = 0;
		Node<T>* N = Head;
		while (N)
		{
			if (N->getItem() == value)
				count++;
			N = N->getNext();
		}
		return count;
	}

	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		if (!Head)
			return;
		Node<T>* P = Head->getNext();
		delete Head;
		Head = P;
	}


	//[5] DeleteLast
	//Deletes the last node in the list
	void DeleteLast()
	{
		if (!Head)
			return;
		if (!(Head->getNext()))
		{
			delete Head;
			Head = 0;
			return;
		}
		Node<T>* P = Head;
		while (P->getNext()->getNext())
		{
			P = P->getNext();
		}
		delete P->getNext();
		P->setNext(0);
	}

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value)
	{
		if (!Head)
			return false;
		bool found = false;
		Node<T>* P = Head;
		Node<T>* N = P->getNext();
		if (P->getItem() == value && !(P->getNext()))
		{
			delete P;
			Head = 0;
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


	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNodes(const T& value)
	{
		while (CountOccurance(value))
			DeleteNode(value);
		return true;
	}

	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(LinkedList& L)
	{
		if (!Head)
			Head = L.Head;
		else
		{
			Node<T>* P = Head;
			while (P->getNext())
				P = P->getNext();
			P->setNext(L.Head);
		}
		L.Head = 0;
	}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse()
	{
		if (!Head)
			return;
		Node<T>* P = Head;
		if (!(P->getNext()))
			return;
		while (P->getNext())
			P = P->getNext();
		while (Head != P)
		{
			Node<T>* N = Head;
			Head = Head->getNext();
			N->setNext(P->getNext());
			P->setNext(N);
		}
	}

};
#endif	


